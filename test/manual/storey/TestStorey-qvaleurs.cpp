#include "Archiviste.hpp"
#include <limits>
#include <ctime>
#include <string>
#include "distributions.h"

using namespace std;


typedef struct
{
	int numModele;
	reel pvaleur;
	reel qvaleur;

} trombone;


class ComparaisonTrombones
{
public:
	ComparaisonTrombones();

	virtual ~ComparaisonTrombones();

	static bool plusPetitQue(const trombone& t1, const trombone& t2)
	{
		return (t1.pvaleur < t2.pvaleur);
	}

	static bool plusGrandQue(const trombone& t1, const trombone& t2)
	{
		return (t1.pvaleur > t2.pvaleur);
	}

protected:

	ComparaisonTrombones(ComparaisonTrombones& c);
};


int main(int argc, char *const argv[])
{
	//			prog nbMod, pi0, nomFichier-first, nomFichier-sec, numPremierFichier, numDernierFichier

	if (argc < 7)
	{
		cout << "Usage: TestStorey-qval nbModelesTot pi0 nomFichier-prefixe nomFichier-suffixe numPremierFichier numDernierFichier (G ou Wald)" << endl;
		return 1;
	}
	time_t temps_debut(time(NULL));

	int nbModelesTot(toolbox::conversion<int>(argv[1])), numPremierFichier(toolbox::conversion<int>(argv[5])), numDernierFichier(toolbox::conversion<int>(argv[6]));
	reel piZero(toolbox::conversion<reel>(argv[2]));

	pair<string, string> nomFichierMarq;
	nomFichierMarq.first = argv[3];
	nomFichierMarq.second = argv[4];

	int nbBlocs(numDernierFichier - numPremierFichier + 1), colG(3), colWald(4), colScore(0), nbColLues(6), validiteModele, dimensionMax(1), dimCourante(1), tailleBlocs(660000);

	if (argc == 7)
	{
		cout << "Score G" << endl;
		colScore = colG;
	}
	else if (argc > 7)
	{
		string scoreSel(argv[7]);

		if (scoreSel == "G")
		{
			cout << "Score G" << endl;
			colScore = colG;
		}
		else if (scoreSel == "W" || scoreSel == "Wald")
		{
			cout << "Score Wald" << endl;
			colScore = colWald;
		}
		else
		{
			cout << "Score " << scoreSel << " non reconnu. Choix du score G." << endl;
			colScore = colG;
		}

	}


	//nomFichierMarq.first="../../../TestsDivers/Storey/Data/part";
	//nomFichierMarq.first="/Volumes/Elements/Maroc/Results/sheep/sambada/sheep-sambada-mark";
	//nomFichierMarq.first="/Volumes/Elements/Maroc/Results/goats/sambada/goats-sambada-mark";

	//nomFichierMarq.first="/Volumes/Elements/Ouganda/AnalyseResultats/Storey-Avril14/res";

//	nomFichierMarq.first="/Users/Sylvie/Documents/Bac-à-Sable/Divers_sambada/Tests/Storey/SubsetCattleSNP/extrait_marq";

//	nomFichierMarq.first="/Volumes/Elements/Maroc/setup/res-test/sheep-filtered-chr-1-mark";

//	nomFichierMarq.first="/local_temp/systucki/fusion-storey/sheep-filtered-chr";
//	nomFichierMarq.first="/local_temp/systucki/fusion-storey/goats-filtered-chr";

//	nomFichierMarq.first="/Volumes/Elements/Maroc/res-juin-pruned/res-sheep/sheep/sheep-sambada-mark";
//	nomFichierMarq.first="/Volumes/Elements/Maroc/res-juin-pruned/res-goats/goats/goats-sambada-mark";


//	nomFichierMarq.second=".txt";
//	nomFichierMarq.second="-Out-1.txt";
//	nomFichierMarq.second="-res-1.txt";

	char delimMots(' ');

	vector<string> nomsFichiers(nbBlocs);
	std::ostringstream oss, ossMark;

	for (int j(0); j < nbBlocs; ++j)
	{
		oss.str("");
		ossMark.str("");
		oss.clear();
		ossMark.clear();
		oss << j + numPremierFichier;
		ossMark << j * tailleBlocs;
		//cout << oss.str() << " " << ossMark.str() << endl;

//		nomsFichiers[j]=(nomFichierMarq.first+"-"+nomFichierMarq.second);
//		nomsFichiers[j]=(nomFichierMarq.first+nomFichierMarq.second);

		//nomsFichiers[j]=(nomFichierMarq.first+"-"+oss.str()+nomFichierMarq.second);
//		nomsFichiers[j]=(nomFichierMarq.first+"-"+oss.str()+"-"+ossMark.str()+nomFichierMarq.second);
		nomsFichiers[j] = (nomFichierMarq.first + oss.str() + nomFichierMarq.second);


		cout << nomsFichiers[j] << endl;
	}

	Lecteur entree;
	entree.setDelimMots(delimMots);
	entree.initialise(nomsFichiers);
	if (!entree.ouverture())
	{
		cerr << "Error while opening files." << endl;
		exit(1);
	}


//	reel seuilScore(toolbox::invCDF_ChiSquare(1.-0.01, 1, sqrt(std::numeric_limits<reel>::epsilon())));
	reel seuilScore(-1);

	cout << "Seuil score:" << seuilScore << " p-valeur:" << scythe::pchisq(seuilScore, 1) << endl;



	// Sheep pruned
	//int nbModelesTot(77705791);
	//reel piZero(0.9713547);

	// Goats pruned
	//int nbModelesTot(76335218);
	//reel piZero(0.9856094);


	vector<vector<string>> modeles;
	vector<string> ligne;

	reel valeur(0);
	int numErreur(0), nbModeles(0);
	vector<string> header, lu;
	trombone modele = {0, 0., 0.};
	vector<trombone> pile;

	for (int i(0); i < nbBlocs; ++i)
	{
		cout << "Fichier " << i << endl;
		entree.lecture<string>(i, header, delimMots);

		while (!entree.finFichier(i))
		{
			entree.lectureGroupe<string>(i, ligne, nbColLues);
			entree.lecture<string>(i, lu);

			valeur = toolbox::conversion<reel>(ligne[colScore]);
			numErreur = toolbox::conversion<int>(ligne[nbColLues - 1]);
			if (valeur >= seuilScore && (numErreur == 0 || numErreur == 6 || numErreur == 7))
			{
				modele.numModele = nbModeles;
				modele.pvaleur = 1 - scythe::pchisq(valeur, 1);
				pile.push_back(modele);
				modeles.push_back(ligne);
				++nbModeles;
			}

		}
	}

	cout << nbModeles << endl;

	sort(pile.begin(), pile.end(), ComparaisonTrombones::plusPetitQue);

	pile[nbModeles - 1].qvaleur = piZero * nbModelesTot * pile[nbModeles - 1].pvaleur / nbModeles;

	for (int i(nbModeles - 2); i >= 0; --i)
	{
		pile[i].qvaleur = min(pile[i + 1].qvaleur, piZero * nbModelesTot * pile[i].pvaleur / (i + 1));

	}
	cout << pile[0].numModele << " " << pile[0].pvaleur << " " << pile[0].qvaleur << endl;
	ofstream sortie((nomFichierMarq.first + "-qvaleurs" + nomFichierMarq.second).c_str());


	for (int i(0); i < nbModeles; ++i)
	{
		for (int j(0); j < nbColLues; ++j)
		{
			sortie << modeles[pile[i].numModele][j] << delimMots;
		}
		sortie << pile[i].qvaleur << endl;
	}

	time_t temps_fin(time(NULL));

	cout << "Temps écoulé: " << difftime(temps_debut, temps_fin) << endl;

	for (int i(0); i <= 100; ++i)
	{
		cout << i << "\t" << scythe::pchisq(i, 1) << "\t" << 1 - scythe::pchisq(i, 1) << endl;
	}
	return 0;
}
