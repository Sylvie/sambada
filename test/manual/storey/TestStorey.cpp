#include "Archiviste.h"
#include <limits>
#include <ctime>

using namespace std;


typedef struct
{
	int nbPvalStorey;
	vector<int> nbModelesValides;

	vector<reel> pval;
	vector<reel> seuilScore;

	vector<vector<int>> compteurG, compteurGOrphelins;
	vector<vector<int>> compteurWald, compteurWaldOrphelins;
} donneesFDR;


int main(int argc, char *const argv[])
{
	time_t temps_debut(time(NULL));

	int nbBlocs(1), colG(3), colWald(4), validiteModele, dimensionMax(1), dimCourante(1), tailleBlocs(660000);
	pair<string, string> nomFichierMarq;

	//nomFichierMarq.first="../../../TestsDivers/Storey/Data/part-";
//	nomFichierMarq.first="/Volumes/Elements/Maroc/Results/sheep/sambada/sheep-sambada-mark";
//	nomFichierMarq.first="/Volumes/Elements/Maroc/Results/goats/sambada/goats-sambada-mark";

//	nomFichierMarq.first="/Volumes/Elements/Ouganda/AnalyseResultats/Storey-Avril14/res";

	nomFichierMarq.first = "/Users/Sylvie/Documents/Bac-à-Sable/Divers_sambada/Tests/Storey/SubsetCattleSNP/extrait_marq";


//	nomFichierMarq.second=".txt";
	nomFichierMarq.second = "-Out-1.txt";

	char delimMots(' ');

	vector<string> nomsFichiers(nbBlocs);
	std::ostringstream oss, ossMark;

	for (int j(0); j < nbBlocs; ++j)
	{
		oss.str("");
		ossMark.str("");
		oss.clear();
		ossMark.clear();
		oss << j;
		ossMark << j * tailleBlocs;
		cout << oss.str() << " " << ossMark.str() << endl;
//		nomsFichiers[j]=(nomFichierMarq.first+"-"+oss.str()+"-"+ossMark.str()+nomFichierMarq.second);
		nomsFichiers[j] = (nomFichierMarq.first + nomFichierMarq.second);
		cout << nomsFichiers[j] << endl;
	}

	Lecteur entree;
	entree.setDelimMots(delimMots);
	entree.initialise(nomsFichiers);
	if (!entree.ouverture())
	{
		cerr << "Error while opening files." << endl;
	}


	donneesFDR storey;



	// Paramètres pour la FDR : 0.01, 0.02, ..., 0.95
	// Les p-valeurs sont dans l'ordre décroissant
	storey.nbPvalStorey = 96;    // Tient compte de la dernière valeur (p=0, score=inf)
	for (int i(storey.nbPvalStorey - 1); i > 0; --i)
	{
		storey.pval.push_back(0.01 * i);
		storey.seuilScore.push_back(toolbox::invCDF_ChiSquare(1. - 0.01 * i, 1, sqrt(std::numeric_limits<reel>::epsilon())));
		cout << 0.01 * i << " " << toolbox::invCDF_ChiSquare(1. - 0.01 * i, 1, sqrt(std::numeric_limits<reel>::epsilon())) << endl;
	}
	storey.pval.push_back(0.);
	storey.seuilScore.push_back(std::numeric_limits<reel>::infinity());

	storey.compteurG.resize(dimensionMax + 1, vector<int>(storey.nbPvalStorey, 0));
	storey.compteurWald.resize(dimensionMax + 1, vector<int>(storey.nbPvalStorey, 0));

	storey.compteurGOrphelins.resize(dimensionMax + 1, vector<int>(storey.nbPvalStorey, 0));
	storey.compteurWaldOrphelins.resize(dimensionMax + 1, vector<int>(storey.nbPvalStorey, 0));

	storey.nbModelesValides.resize(dimensionMax + 1, 0);


	vector<string> texte;
	vector<reel> valeurs(2);
	vector<int> numErreur(1);
	for (int i(0); i < nbBlocs; ++i)
	{
		entree.lecture<string>(i, texte, delimMots);

		while (!entree.finFichier(i))
		{
			entree.lectureGroupe<string>(i, texte, colG);
			entree.lectureGroupe<reel>(i, valeurs, 2);
			entree.lectureGroupe<int>(i, numErreur, 1);
			entree.lecture<string>(i, texte);

			if (numErreur[0] == 0 || numErreur[0] == 6 || numErreur[0] == 7)
			{
				++storey.compteurG[dimCourante][(upper_bound(storey.seuilScore.begin(), storey.seuilScore.end(), valeurs[0]) - storey.seuilScore.begin())];
				++storey.compteurWald[dimCourante][(upper_bound(storey.seuilScore.begin(), storey.seuilScore.end(), valeurs[1]) - storey.seuilScore.begin())];

				++storey.nbModelesValides[dimCourante];
				if (storey.nbModelesValides[dimCourante] % (int) pow(10., 6) == 0)
				{
					cout << storey.nbModelesValides[dimCourante] << endl;
				}
			}
		}
	}

	cout << storey.nbModelesValides[dimCourante] << endl;





	// Storey
	for (int i(1); i < (dimensionMax + 1); ++i)
	{
		cout << "Nombre de modèles valides (Storey) dim=" << i << " : " << storey.nbModelesValides[i] << "\n";
	}
	//	ofstream sortieStorey("res-Storey.txt");
	ofstream sortieStorey((nomFichierMarq.first + "-storey" + nomFichierMarq.second).c_str());

	sortieStorey << "P-valeurs" << delimMots;
	for (int i(0); i < storey.nbPvalStorey; ++i)
	{
		sortieStorey << storey.pval[i] << delimMots;
	}
	sortieStorey << endl;

	sortieStorey << "Scores" << delimMots;
	for (int i(0); i < storey.nbPvalStorey; ++i)
	{
		sortieStorey << storey.seuilScore[i] << delimMots;
	}
	sortieStorey << endl;

	for (int i(1); i < (dimensionMax + 1); ++i)
	{
		sortieStorey << "G" << i << delimMots;
		for (int j(0); j < storey.nbPvalStorey; ++j)
		{
			sortieStorey << storey.compteurG[i][j] << delimMots;
		}
		sortieStorey << endl;

		sortieStorey << "GOrphelins" << i << delimMots;
		for (int j(0); j < storey.nbPvalStorey; ++j)
		{
			sortieStorey << storey.compteurGOrphelins[i][j] << delimMots;
		}
		sortieStorey << endl;

		sortieStorey << "Wald" << i << delimMots;
		for (int j(0); j < storey.nbPvalStorey; ++j)
		{
			sortieStorey << storey.compteurWald[i][j] << delimMots;
		}
		sortieStorey << endl;

		sortieStorey << "WaldOrphelins" << i << delimMots;
		for (int j(0); j < storey.nbPvalStorey; ++j)
		{
			sortieStorey << storey.compteurWaldOrphelins[i][j] << delimMots;
		}
		sortieStorey << endl;


	}


	sortieStorey.close();
	time_t temps_fin(time(NULL));

	cout << "Temps écoulé: " << difftime(temps_debut, temps_fin) << endl;
	return 0;
}
