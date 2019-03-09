#include "Archiviste.h"
#include <limits>
#include <ctime>
#include <string>
#include "distributions.h"
#include "matrix.h"

using namespace std;
using namespace scythe;

typedef struct
{
	int nbPvalStoreyWithZero, nbPvalStorey;
	int nbModelesValidesG, nbModelesValidesWald;

	vector<reel> pval;
	vector<reel> seuilScore;

	vector<int> compteurG, compteurGOrphelins;
	vector<int> compteurWald, compteurWaldOrphelins;

} donneesFDR;

typedef Matrix<reel, Col, Concrete> MatriceReels;


int main(int argc, char *const argv[])
{
	if (argc != 6)
	{
		cerr << "Usage: SplineStorey nomFichier-prefixe nomFichier-suffixe n numPremierFichier numDernierFichier ou \nSplineStorey nomFichier-prefixe nomFichier-suffixe s nbBlocs tailleBlocs" << endl;
	}

	pair<string, string> nomFichierMarq;
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

	vector<string> nomsFichiers;
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
	// Paramètres pour la FDR : 0.01, 0.02, ..., 0.95
	// Les p-valeurs sont dans l'ordre décroissant
	storey.nbPvalStoreyWithZero = 96;    // Tient compte de la dernière valeur (p=0, score=inf)
	storey.nbPvalStorey = storey.nbPvalStoreyWithZero - 1;
	for (int i(storey.nbPvalStoreyWithZero - 1); i > 0; --i)
	{
		storey.pval.push_back(0.01 * i);
		storey.seuilScore.push_back(toolbox::invCDF_ChiSquare(1. - 0.01 * i, 1, sqrt(numeric_limits<reel>::epsilon())));
		//cout << 0.01*i << " " << toolbox::invCDF_ChiSquare(1.-0.01*i, 1, sqrt(numeric_limits<reel>::epsilon())) << endl;
	}
	storey.pval.push_back(0.);
	storey.seuilScore.push_back(std::numeric_limits<reel>::infinity());

	storey.compteurG.resize(storey.nbPvalStoreyWithZero, 0);
	storey.compteurWald.resize(storey.nbPvalStoreyWithZero, 0);

	storey.compteurGOrphelins.resize(storey.nbPvalStoreyWithZero, 0);
	storey.compteurWaldOrphelins.resize(storey.nbPvalStoreyWithZero, 0);

	storey.nbModelesValidesG = 0;
	storey.nbModelesValidesWald = 0;


	ifstream entree;
	string label("");
	vector<int> ligne(0), validation(0);
	string retourLigne("");
	int nbLignesEntete(2), nbLignesLues(4);
	vector<pair<string, vector<int> *> > etiquettes(nbLignesLues);
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
			else if (ligne.size() != storey.nbPvalStoreyWithZero)
			{
				cout << "Fichier " << nomsFichiers[i] << ", ligne " << etiquettes[j].first << ", " << ligne.size() << " valeurs lues au lieu de " << storey.nbPvalStoreyWithZero << "." << endl;
				return 6;
			}
			else
			{
				for (int k(0); k < storey.nbPvalStoreyWithZero; ++k)
				{
					(*(etiquettes[j].second))[k] += ligne[k];
				}
			}
		}
		entree.close();
	}

	// Nombre de modèles
	for (int i(0); i < storey.nbPvalStoreyWithZero; ++i)
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

	int degreSpline(3);
	MatriceReels puissances(storey.nbPvalStorey, degreSpline + 1);
	cout << puissances.rows() << endl;
	puissances(_, 0) = 1.;
	cout << puissances(0, 0, 2, 3) << endl;

	for (int i(1); i <= degreSpline; ++i)
	{
		puissances(_, i) = puissances(_, (i - 1)) % pval(_, 0);
	}
	cout << puissances(0, 0, 2, 3) << puissances(93, 0, 94, 3) << endl;

	// coeff b : vecteur 4*95=380 cases
	// a1 b1 c1 d1 a2 b2 c2 d2 ... a95 b95 c95 d95
	vector<int> a(storey.nbPvalStoreyWithZero, 0), b(storey.nbPvalStoreyWithZero, 0), c(storey.nbPvalStoreyWithZero, 0), d(storey.nbPvalStoreyWithZero, 0);
	for (int i(1); i < nbPvalStoreyWithZero; ++i)
	{
		a[i] = 4 * (i - 1);
		b[i] = 4 * (i - 1) + 1;
		c[i] = 4 * (i - 1) + 2;
		d[i] = 4 * (i - 1) + 3;
	}
	MatriceReels A((degreSpline + 1) * nbPvalStorey)

}
