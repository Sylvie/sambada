#include "Archiviste.h"
#include <limits>
#include <ctime>
#include <string>
#include "distributions.h"
#include "matrix.h"
#include "la.h"
#include "ide.h"

using namespace std;
using namespace scythe;

typedef struct
{
	int nbPvalStorey;
	int nbModelesValidesG, nbModelesValidesWald;

	vector <reel> pval;
	vector <reel> seuilScore;

	vector<int> compteurG, compteurGOrphelins;
	vector<int> compteurWald, compteurWaldOrphelins;

} donneesFDR;

typedef Matrix <reel, Col, Concrete> MatriceReels;


int main(int argc, char *const argv[])
{
	if (argc != 6)
	{
		cerr << "Usage: SplineStorey nomFichier-prefixe nomFichier-suffixe n numPremierFichier numDernierFichier ou \nSplineStorey nomFichier-prefixe nomFichier-suffixe s nbBlocs tailleBlocs" << endl;
	}

	pair <string, string> nomFichierMarq;
	nomFichierMarq.first = argv[1];
	nomFichierMarq.second = argv[2];

	string typeFichier(argv[3]);
	bool fichiersNumerotes(true);

	if (typeFichier == "n")
	{
		fichiersNumerotes = true;
	}
	else if (typeFichier == "s")
	{
		fichiersNumerotes = false;
	}
	else
	{
		cout << "L'argument 3 doit être \"n\" pour des fichiers numérotés ou \"s\" pour des fichiers générés avec Supervision." << endl;
		cout << "*" << argv[3] << "*" << endl;
		return 2;
	}

	vector <string> nomsFichiers;
	int nbFichiers(0);

	if (fichiersNumerotes)
	{
		int numPremierFichier(toolbox::conversion<int>(argv[4])), numDernierFichier(toolbox::conversion<int>(argv[5]));
		nbFichiers = numDernierFichier - numPremierFichier + 1;
		for (int i(numPremierFichier); i <= numDernierFichier; ++i)
		{
			ostringstream oss;
			oss << i;
			nomsFichiers.push_back(nomFichierMarq.first + oss.str() + nomFichierMarq.second);
			cout << nomsFichiers.back() << endl;
		}
	}
	else
	{
		int nbBlocs(toolbox::conversion<int>(argv[4])), tailleBlocs(toolbox::conversion<int>(argv[5]));
		nbFichiers = nbBlocs;
		for (int i(0); i < nbBlocs; ++i)
		{
			ostringstream oss, oss_mark;
			oss << i;
			oss_mark << i * tailleBlocs;
			nomsFichiers.push_back(nomFichierMarq.first + "-mark-" + oss.str() + "-" + oss_mark.str() + "-storey" + nomFichierMarq.second);
			cout << nomsFichiers.back() << endl;
		}
	}


	donneesFDR storey;
	// Paramètres pour la FDR :0, 0.01, 0.02, ..., 0.95
	// Les p-valeurs sont dans l'ordre décroissant
	storey.nbPvalStorey = 96;    // Tient compte de la dernière valeur (p=0, score=inf)
	for (int i(storey.nbPvalStorey - 1); i > 0; --i)
	{
		storey.pval.push_back(0.01 * i);
		storey.seuilScore.push_back(toolbox::invCDF_ChiSquare(1. - 0.01 * i, 1, sqrt(numeric_limits<reel>::epsilon())));
		//cout << 0.01*i << " " << toolbox::invCDF_ChiSquare(1.-0.01*i, 1, sqrt(numeric_limits<reel>::epsilon())) << endl;
	}
	storey.pval.push_back(0.);
	storey.seuilScore.push_back(std::numeric_limits<reel>::infinity());

	storey.compteurG.resize(storey.nbPvalStorey, 0);
	storey.compteurWald.resize(storey.nbPvalStorey, 0);

	storey.compteurGOrphelins.resize(storey.nbPvalStorey, 0);
	storey.compteurWaldOrphelins.resize(storey.nbPvalStorey, 0);

	storey.nbModelesValidesG = 0;
	storey.nbModelesValidesWald = 0;


	ifstream entree;
	string label("");
	vector<int> ligne(0), validation(0);
	string retourLigne("");
	int nbLignesEntete(2), nbLignesLues(4);
	vector < pair < string, vector < int > * > > etiquettes(nbLignesLues);
	etiquettes[0] = make_pair("G1", &(storey.compteurG));
	etiquettes[1] = make_pair("GOrphelins1", &(storey.compteurGOrphelins));
	etiquettes[2] = make_pair("Wald1", &(storey.compteurWald));
	etiquettes[3] = make_pair("WaldOrphelins1", &(storey.compteurWaldOrphelins));

	for (int i(0); i < nbFichiers; ++i)
	{
		entree.open(nomsFichiers[i].c_str());
		if (entree.fail())
		{
			cout << "Erreur dans l'ouverture du fichier: " << nomsFichiers[i] << endl;
			return 3;
		}

		if (i == 0)
		{
			toolbox::chercheRetourLigne(entree, retourLigne);
		}

		// Deux lignes d'entête
		for (int j(0); j < nbLignesEntete; ++j)
		{
			getline(entree, label);
			entree >> ws;
		}

		// Lignes de comptage
		for (int j(0); j < nbLignesLues; ++j)
		{
			entree >> label;
			if (label != etiquettes[j].first)
			{
				cout << "Fichier " << nomsFichiers[i] << " : l'étiquette devrait être " << etiquettes[j].first << ", mais " << label << " trouvé.";
				return 4;
			}
			toolbox::lectureLigne(entree, ligne, validation);
			if (validation.size() > 0)
			{
				cout << "Fichier " << nomsFichiers[i] << ", ligne " << etiquettes[j].first << ", valeur numéro " << validation[0] << " inconnue." << endl;
				return 5;
			}
			else if (ligne.size() != storey.nbPvalStorey)
			{
				cout << "Fichier " << nomsFichiers[i] << ", ligne " << etiquettes[j].first << ", " << ligne.size() << " valeurs lues au lieu de " << storey.nbPvalStorey << "." << endl;
				return 6;
			}
			else
			{
				for (int k(0); k < storey.nbPvalStorey; ++k)
				{
					(*(etiquettes[j].second))[k] += ligne[k];
				}
			}
		}
		entree.close();
	}

	// Nombre de modèles
	for (int i(0); i < storey.nbPvalStorey; ++i)
	{
		storey.nbModelesValidesG += storey.compteurG[i];
		storey.nbModelesValidesWald += storey.compteurWald[i];
	}
	cout << storey.nbModelesValidesG << " " << storey.nbModelesValidesWald << endl;


	MatriceReels pval(storey.nbPvalStorey, 1), valG(storey.nbPvalStorey, 1), valWald(storey.nbPvalStorey, 1);

	pval[storey.nbPvalStorey - 1] = storey.pval[0];
	valG[storey.nbPvalStorey - 1] = storey.compteurG[0];
	valWald[storey.nbPvalStorey - 1] = storey.compteurWald[0];
	for (int i(1); i < (storey.nbPvalStorey); ++i)
	{
		pval[storey.nbPvalStorey - 1 - i] = storey.pval[i];
		valG[storey.nbPvalStorey - 1 - i] = valG[storey.nbPvalStorey - i] + storey.compteurG[i];
		valWald[storey.nbPvalStorey - 1 - i] = valWald[storey.nbPvalStorey - i] + storey.compteurWald[i];

	}

	for (int i(0); i < (storey.nbPvalStorey); ++i)
	{
		valG[i] /= (storey.nbModelesValidesG * (1 - pval[i]));
		valWald[i] /= (storey.nbModelesValidesWald * (1 - pval[i]));
		cout << pval[i] << " " << valG[i] << " " << valWald[i] << endl;
	}

	int degreSpline(3), nbSplines(storey.nbPvalStorey - 1);
	MatriceReels puissances(storey.nbPvalStorey, degreSpline + 1);
	cout << puissances.rows() << endl;
	puissances(_, 0) = 1.;
	cout << puissances(0, 0, 2, 3) << endl;

	for (int i(1); i <= degreSpline; ++i)
	{
		puissances(_, i) = puissances(_, (i - 1)) % pval(_, 0);
	}
	cout << puissances(0, 0, 2, 3) << puissances(93, 0, 94, 3) << endl;

	// coeff x : vecteur 4*95=380 cases
	// a0 b0 c0 d0 a1 b1 c1 d1 a2 b2 c2 d2 ... a94 b94 c94 d94
	vector<int> a(nbSplines, 0), b(nbSplines, 0), c(nbSplines, 0), d(nbSplines, 0);
	for (int i(0); i < nbSplines; ++i)
	{
		a[i] = 4 * i;
		b[i] = 4 * i + 1;
		c[i] = 4 * i + 2;
		d[i] = 4 * i + 3;
	}
	cout << a[0] << " " << b[0] << " " << c[0] << " " << d[0] << " " << a[94] << " " << b[94] << " " << c[94] << " " << d[94] << endl;
	MatriceReels A((degreSpline + 1) * nbSplines, (degreSpline + 1) * nbSplines),
			b_G((degreSpline + 1) * nbSplines, 1), b_Wald((degreSpline + 1) * nbSplines, 1),
			res_G((degreSpline + 1) * nbSplines, 1), res_Wald((degreSpline + 1) * nbSplines, 1);

	int numContrainte(0);
	// Formulation des contraintes
	// Pour chaque ligne de la matrice, chaque colonne sera multipliée par le coefficient b correspondant -> Il faut mettre les bonnes valeurs (xi^3, xi^2, xi, 1) dans les colonnes

	//Contraintes sur les points
	for (int i(0); i < nbSplines; ++i)
	{
		// Point gauche 
		// ai * xi^3 + bi * xi^2 + ci * xi + di = yi
		A(numContrainte, a[i]) = puissances(i, 3);
		A(numContrainte, b[i]) = puissances(i, 2);
		A(numContrainte, c[i]) = puissances(i, 1);
		A(numContrainte, d[i]) = puissances(i, 0);
		b_G(numContrainte, 0) = valG[i];
		b_Wald(numContrainte, 0) = valWald[i];
		++numContrainte;

		// Point droite
		// ai * xi+1^3 + bi * xi+1^2 + ci * xi+1 + di = yi+1
		A(numContrainte, a[i]) = puissances(i + 1, 3);
		A(numContrainte, b[i]) = puissances(i + 1, 2);
		A(numContrainte, c[i]) = puissances(i + 1, 1);
		A(numContrainte, d[i]) = puissances(i + 1, 0);
		b_G(numContrainte, 0) = valG[i + 1];
		b_Wald(numContrainte, 0) = valWald[i + 1];
		++numContrainte;

	}

	//Contraintes sur les points intérieurs
	for (int i(1); i < (storey.nbPvalStorey - 1); ++i)
	{
		// Egalité des dérivées 
		// 3 ai-1 * xi^2 + 2 bi-1 * xi + ci-1 - ( 3 ai * xi^2 + 2 bi * xi + ci ) = 0
		A(numContrainte, a[i - 1]) = 3 * puissances(i, 2);
		A(numContrainte, b[i - 1]) = 2 * puissances(i, 1);
		A(numContrainte, c[i - 1]) = puissances(i, 0);
		A(numContrainte, a[i]) = -3 * puissances(i, 2);
		A(numContrainte, b[i]) = -2 * puissances(i, 1);
		A(numContrainte, c[i]) = -puissances(i, 0);
		++numContrainte;


		// Egalité des dérivées secondes 
		// 6 ai-1 * xi + 2 bi-1  - ( 6 ai * xi + 2 bi ) = 0
		A(numContrainte, a[i - 1]) = 6 * puissances(i, 1);
		A(numContrainte, b[i - 1]) = 2 * puissances(i, 0);
		A(numContrainte, a[i]) = -6 * puissances(i, 1);
		A(numContrainte, b[i]) = -2 * puissances(i, 0);
		++numContrainte;

	}

	// Contraintes sur les dérivées secondes pour le premier et le dernier point
	// 6 a0 * x0 + 2 b0 = 0
	A(numContrainte, a[0]) = 6 * puissances(0, 1);
	A(numContrainte, b[0]) = 2 * puissances(0, 0);
	++numContrainte;

	// 6 a94 * x95 + 2 b94 = 0
	A(numContrainte, a[(nbSplines - 1)]) = 6 * puissances((storey.nbPvalStorey - 1), 1);
	A(numContrainte, b[(nbSplines - 1)]) = 2 * puissances((storey.nbPvalStorey - 1), 0);
	++numContrainte;

	cout << "Nombre de contraintes: " << numContrainte << endl;


	MatriceReels L((degreSpline + 1) * nbSplines, (degreSpline + 1) * nbSplines), U((degreSpline + 1) * nbSplines, (degreSpline + 1) * nbSplines);
	Matrix<unsigned int, Col, Concrete> perm_vec((degreSpline + 1) * nbSplines);

	cout << "Décomposition LU" << endl;
	lu_decomp(A, L, U, perm_vec);

	cout << "Résolution pour G" << endl;
	res_G = lu_solve(A, b_G, L, U, perm_vec);

	cout << "Résolution pour Wald" << endl;
	res_Wald = lu_solve(A, b_Wald, L, U, perm_vec);

	cout << res_G << endl;
	cout << "***" << endl;



	// L'estimation est faite avec la dernière spline
	// L'interpolation est linéaire en dehors de la zone
	reel positionEstimation(1);


	reel estimationG(0);
	for (int i(0); i <= degreSpline; ++i)
	{
		estimationG += puissances(nbSplines, degreSpline - i) * res_G((nbSplines - 1) * (degreSpline + 1) + i, 0);
	}
	cout << estimationG << endl;
	cout << (positionEstimation - pval[storey.nbPvalStorey - 1]) << " " << 3 * res_G(a[nbSplines - 1], 0) * puissances(nbSplines, 2) + 2 * res_G(b[nbSplines - 1], 0) * puissances(nbSplines, 1) + res_G(c[nbSplines - 1], 0) * puissances(nbSplines, 0) << endl;


	estimationG = estimationG + (positionEstimation - pval[storey.nbPvalStorey - 1]) * (3 * res_G(a[nbSplines - 1], 0) * puissances(nbSplines, 2) + 2 * res_G(b[nbSplines - 1], 0) * puissances(nbSplines, 1) + res_G(c[nbSplines - 1], 0) * puissances(nbSplines, 0));

	cout << "Valeur pour G: " << estimationG << endl;


	// ***** Green et Silverman **** //

	int nbPoints(storey.nbPvalStorey);
	MatriceReels Q(nbPoints, nbPoints - 2), R(nbPoints - 2, nbPoints - 2), h(nbPoints - 1, 1);
	Q = 0.;
	R = 0.;
	for (int i(0); i < (h.size()); ++i)
	{
		h[i] = pval[i + 1] - pval[i];
	}
	//cout << h << endl;
	// p. 12 en bas
	for (int i(1); i < (nbPoints - 1); ++i)
	{
		Q(i - 1, i - 1) = 1. / h[i - 1];
		Q(i, i - 1) = -1. / h[i - 1] - 1. / h[i];
		Q(i + 1, i - 1) = 1. / h[i];
	}
	for (int i(1); i < (nbPoints - 2); ++i)
	{
		R(i - 1, i - 1) = (h[i - 1] + h[i]) / 3.;
		R(i, i - 1) = (h[i - 1]) / 6.;
		R(i - 1, i) = R(i, i - 1);
	}
	R(nbPoints - 3, nbPoints - 3) = (h[nbPoints - 3] + h[nbPoints - 2]) / 3.;

	MatriceReels x(nbPoints - 2, 1), gamma(nbPoints - 2, 1);
	x = t(Q) * valG;
	MatriceReels ch(cholesky(R));

	gamma = chol_solve(R, x, ch);

	cout << "Interpolation: " << valG[nbPoints - 1] + (positionEstimation - pval[nbPoints - 1]) * ((valG[nbPoints - 1] - valG[nbPoints - 2]) / (pval[nbPoints - 1] - pval[nbPoints - 2]) + (pval[nbPoints - 1] - pval[nbPoints - 2]) * gamma[nbPoints - 3] / 6.) << endl << "******" << endl;


	reel alpha(1.);
	for (int i(0); i <= 100; ++i)
	{
		alpha = i * 0.0001;

		MatriceReels Rprime(R + alpha * t(Q) * Q);
		ch = cholesky(Rprime);
		gamma = chol_solve(Rprime, x, ch);
		MatriceReels g(valG - alpha * Q * gamma);

		MatriceReels identity(eye(nbPoints));
		MatriceReels Aalpha(invpd(identity + (alpha * Q * invpd(R) * t(Q))));
		reel cv(0.);
		for (int j(0); j < nbPoints; ++j)
		{
			cv += pow(((valG(j, 0) - g(j, 0)) / ((1 - Aalpha(j, j)))), 2);
		}
		cout << "Lissage (alpha=" << alpha << "): " << g[nbPoints - 1] + (positionEstimation - pval[nbPoints - 1]) * ((g[nbPoints - 1] - g[nbPoints - 2]) / (pval[nbPoints - 1] - pval[nbPoints - 2]) + (pval[nbPoints - 1] - pval[nbPoints - 2]) * gamma[nbPoints - 3] / 6.) << " " << cv << endl;
		//cout << alpha << " " <<  g[nbPoints-1] + (positionEstimation - pval[nbPoints-1])* ((g[nbPoints-1]-g[nbPoints-2])/(pval[nbPoints-1] - pval[nbPoints-2]) + (pval[nbPoints-1] - pval[nbPoints-2]) * gamma[nbPoints-3] / 6.) << endl; 

	}
	cout << "******" << endl;

	return 0;
}
