#include "Supervision.h"
#include "Toolbox.h"
#include "Erreur.h"
#include <ctime>
#include <algorithm>

using namespace std;

Supervision::Supervision()
{}

Supervision::~Supervision()
{}

int Supervision::preparationsCalculs(const string& nomFichier)
{
	time_t temps_start(time(NULL));
	
	sortie.setRetourLigne(&ParametresCluster::retourLigne[0]);
	entree.setRetourLigne(&ParametresCluster::retourLigne[0]);
	
	sortie.setDelimMots(' ');
	entree.setDelimMots(' ');
	
	cout << "Retour: " << (int)(entree.getRetourLigne()[0]) << endl;
	
	ifstream entreeFichier(nomFichier.c_str());
	if (entreeFichier.fail())
	{
		cerr << "Erreur dans la lecture des paramètres." << endl;
		cerr << nomFichier << endl;
		exit(2);
	}
	
	bool varEnvIncluses(true);
	
	string lu("");
	getline(entreeFichier, lu);
	
	toolbox::enleveEspaces(lu);
	const char guillemet('"');
	string::size_type position;
	if (lu[0]==guillemet)
	{
		position=lu.find(guillemet, 1);
		cout << "Truc "<< position << endl;
		
		nomFichierMarq.first=lu.substr(1, position-1);
		lu=lu.substr(position+1);
		toolbox::enleveEspaces(lu);
		if (lu.size()>0)
		{
			cout << "*" << lu << "*" << endl;
			nomFichierEnv.first=lu;
			toolbox::enleveEspaces(nomFichierEnv.first);
			varEnvIncluses=false;
		}
	}
	else
	{
		position=lu.find(' ');
		if (position==string::npos)
		{
			nomFichierMarq.first=lu;
		}
		else
		{
			nomFichierMarq.first=lu.substr(0, position);
			lu=lu.substr(position+1);
			toolbox::enleveEspaces(lu);
			if (lu.size()>0)
			{
				nomFichierEnv.first=lu;
				toolbox::enleveEspaces(nomFichierEnv.first);
				varEnvIncluses=false;
			}
		}
	}
	
	
	
	/*		string::size_type position(lu.find("\" \""));
	 
	 if (position != string::npos)
	 {
	 // Cas où les noms sont encadrés par des guillemets
	 nomFichierMarq.first=lu.substr(0, position);
	 while (nomFichierMarq.first[0]==' '
	 || nomFichierMarq.first[0]=='"')
	 {
	 nomFichierMarq.first=nomFichierMarq.first.substr(1);
	 }
	 
	 nomFichierEnv.first=lu.substr(position+3);
	 while (nomFichierEnv.first[nomFichierEnv.first.size()-1]==' ' || nomFichierEnv.first[nomFichierEnv.first.size()-1]=='"')
	 {
	 nomFichierEnv.first=nomFichierEnv.first.substr(0, nomFichierEnv.first.size()-1);
	 }
	 
	 
	 varEnvIncluses=false;
	 }
	 else  
	 {
	 position=lu.find(" ");
	 if (position != string::npos)
	 {
	 // Cas où les noms ne contiennent pas de guillemets
	 nomFichierMarq.first=lu.substr(0, position);
	 nomFichierEnv.first=lu.substr(position+1);
	 varEnvIncluses=false;
	 
	 }
	 else	// Cas d'un seul fichier
	 {
	 nomFichierMarq.first=lu;
	 nomFichierEnv.first="";
	 }
	 }*/
	
	entreeFichier >> ws >> nomFichierParam.first;
	
	entreeFichier >>ws >> nbEnv >> nbMarq >> nbLignes >> tailleBlocs;
	cout << nbEnv << " " << nbMarq << " " << nbLignes << " " << tailleBlocs << endl;
	
	vector<int> paramOS(0);
	
	toolbox::lectureLigne(entreeFichier, paramOS);
	
	entreeFichier.close();
	
	int nbCols(nbMarq);
	if (varEnvIncluses)
	{
		nbCols+=nbEnv;
	}
	
	// Ouverture du fichier d'entrée
	entreeFichier.open(nomFichierMarq.first.c_str());
	if (entreeFichier.fail())
	{
		cerr << "Erreur dans la lecture du fichier." << endl;
		cerr << nomFichierMarq.first << endl;
		exit(2);
	}
	
	
	// Préparation des flots de sortie
	string sepRep(ParametresCluster::sepRep_Unix);
#ifdef _WIN32
	sepRep=ParametresCluster::sepRep_Win;
#endif
	
	// Décomposition des noms de fichiers
	position=0;
	
	// Fichier des marqueurs
	position=nomFichierMarq.first.rfind(sepRep);
	if (position!=string::npos)
	{
		// On prend le chemin du fichier de param
		//		chemin=nomFichierMarq.first.substr(0, position+1);
		nomFichierMarq.first=nomFichierMarq.first.substr(position+1);
	}
	
	cout << position << endl /*<< chemin << endl*/ << nomFichierMarq.first << endl;
	
	position=nomFichierMarq.first.rfind(ParametresCluster::sepExt);
	if (position!=string::npos)
	{
		nomFichierMarq.second=nomFichierMarq.first.substr(position);
		nomFichierMarq.first=nomFichierMarq.first.substr(0, position);		
	}
	cout << nomFichierMarq.first << endl << nomFichierMarq.second << endl;
	
	// Fichier des variables environnementales
	
	if (!varEnvIncluses)
	{
		position=nomFichierEnv.first.rfind(sepRep);
		if (position!=string::npos)
		{
			nomFichierEnv.first=nomFichierEnv.first.substr(position+1);
		}
	}
	else 
	{
		nomFichierEnv.first=nomFichierMarq.first+ParametresCluster::suffixeEnv;
		nomFichierEnv.second=nomFichierMarq.second;
	}
	
	cout << position << endl << nomFichierEnv.first << endl;
	
	position=nomFichierEnv.first.rfind(ParametresCluster::sepExt);
	if (position!=string::npos)
	{
		nomFichierEnv.second=nomFichierEnv.first.substr(position);
		nomFichierEnv.first=nomFichierEnv.first.substr(0, position);		
	}
	cout << nomFichierEnv.first << endl << nomFichierEnv.second << endl;	
	
	// Fichier de paramètres
	position=nomFichierParam.first.rfind(sepRep);
	if (position!=string::npos)
	{
		nomFichierParam.first=nomFichierParam.first.substr(position+1);
	}
	
	cout << position << endl << nomFichierParam.first << endl;
	
	position=nomFichierParam.first.rfind(ParametresCluster::sepExt);
	if (position!=string::npos)
	{
		nomFichierParam.second=nomFichierParam.first.substr(position);
		nomFichierParam.first=nomFichierParam.first.substr(0, position);		
	}
	cout << nomFichierParam.first << endl << nomFichierParam.second << endl;
	
	// On crée le chemin à partir du nom du fichier de découpe
	position=nomFichier.rfind(sepRep);
	if (position!=string::npos)
	{
		chemin=nomFichier.substr(0, position+1);
	}
	else 
	{
		chemin="";
	}


	
	std::ostringstream oss, ossNumPremierMarq;
	oss << tailleBlocs;
	//string nomRep(chemin+nomFichierParam.first+"-"+oss.str()+sepRep);
	string nomRep(chemin);
	
	cout << nomRep << endl;
	
	
	int nbBlocs(0), nbBlocsComplets(0), tailleDernierBloc(0);
	tailleDernierBloc=nbMarq%tailleBlocs;
	nbBlocsComplets=nbMarq/tailleBlocs;
	if (tailleDernierBloc==0)
	{
		nbBlocs=nbBlocsComplets;
	}
	else 
	{
		nbBlocs=nbBlocsComplets+1;
	}
	
	// Noms des fichiers et flots
	vector<string> nomsFichiers(nbBlocs);
	for(int i(0); i<nbBlocs; ++i)
	{
		oss.str("");
		oss.clear();
		oss << i;
		ossNumPremierMarq.str("");
		ossNumPremierMarq.clear();
		ossNumPremierMarq << i*tailleBlocs;
		nomsFichiers[i]=(nomRep+nomFichierMarq.first+ParametresCluster::suffixeMarq+oss.str()+"-"+ossNumPremierMarq.str()+nomFichierMarq.second);
	}
	if (varEnvIncluses)
	{
		nomsFichiers.push_back(nomRep+nomFichierEnv.first+nomFichierEnv.second);
	}
	
	sortie.initialise(nomsFichiers);
	sortie.ouverture();
	
	// Début de la copie
	vector<string> ligne(nbCols, "");
	int rows(0);
	
	time_t temps_interm(time(NULL));
	cout << "Initialisation : " << difftime(temps_interm, temps_start) << " s." << endl;
	
	int decalageColonnes(0);
	if (varEnvIncluses)
	{
		decalageColonnes=nbEnv;
	}
	
	while (!entreeFichier.eof())
	{
		entreeFichier>> ws;
		
		if (rows%100==0)
		{
			cout <<"Row " <<rows<<endl;
		}
		
		toolbox::lectureLigne(entreeFichier, ligne);
		
		/*if (nbCols == -1)
		 nbCols = (unsigned int) line.size();*/
		
		if(nbCols != ligne.size())
		{
			oss.str("");
			oss.clear();
			oss << "Row " << (rows + 1) << " of input file has " << ligne.size() << " elements, but should have " << nbCols << ".";
			cerr << oss.str();
			exit(4);
		}
		
		if (rows >= nbLignes)
		{
			cerr << "Nombre de lignes incorrect" << endl;			
		}
		
		
		if (varEnvIncluses)
		{
			for (int j(0); j<nbEnv; ++j)
			{
				sortie.ecriture(nbBlocs, ligne[j]);
			}
			sortie.ecriture(nbBlocs, "", true);
		}
		for (int i(0); i<nbBlocsComplets; ++i)
		{
			
			for (int j(0); j<tailleBlocs; ++j)
			{
				sortie.ecriture(i, ligne[decalageColonnes+i*tailleBlocs+j]);
			}
			
			//			*(sorties[i]) << endl;
			sortie.ecriture(i, "", true);
		}
		// Dernier bloc
		if (tailleDernierBloc>0)
		{
			
			for(int i(0); i<tailleDernierBloc; ++i)
			{
				sortie.ecriture(nbBlocs-1, ligne[decalageColonnes+nbBlocsComplets*tailleBlocs+i]);				
			}
			
			//			*(sorties[nbBlocs-1]) << endl;
			sortie.ecriture(nbBlocs-1, "", true);
		}
		
		++rows;
		
		entreeFichier >> ws;
	}
	
	entreeFichier.close();
	sortie.fermeture();
	
	time_t temps_stop(time(NULL));
	
	cout << "Ecriture des résultats : " << difftime(temps_stop, temps_interm) << " s." << endl;
	
	
}

int Supervision::fusionResultats(int argc, char* argv[]) throw()
{
	// nomProg nomFichierInput nbBlocs tailleBlocs dimMax
	if (argc!=5)
	{
		throw Erreur("Nombre d'arguments incorrect.");	
	}
	
	std::istringstream iss;
	iss.str(argv[2]);
	iss>>nbBlocs;
	iss.str("");
	iss.clear();
	iss.str(argv[4]);
	
	int dimensionMax(0);
	iss >> dimensionMax;
	
	iss.str("");
	iss.clear();
	iss.str(argv[3]);
	iss >>tailleBlocs;
	
	
	if (dimensionMax<0)
	{
		throw Erreur("Dimension des résultat incorrecte.");	
	}
	
	time_t temps_start(time(NULL));
	
	sortie.setRetourLigne(&ParametresCluster::retourLigne[0]);
	entree.setRetourLigne(&ParametresCluster::retourLigne[0]);

	sortie.setDelimMots(' ');
	entree.setDelimMots(' ');
	
	nomFichierMarq.first=argv[1];
	
	// Préparation des flots de sortie
	string sepRep(ParametresCluster::sepRep_Unix);
#ifdef _WIN32
	sepRep=ParametresCluster::sepRep_Win;
#endif
	
	// Décomposition des noms de fichiers
	int position(0);
	
	// Fichier des marqueurs
	position=nomFichierMarq.first.rfind(sepRep);
	if (position!=string::npos)
	{
		chemin=nomFichierMarq.first.substr(0, position+1);
		nomFichierMarq.first=nomFichierMarq.first.substr(position+1);
	}
	
	cout << position << endl << chemin << endl << nomFichierMarq.first << endl;
	
	position=nomFichierMarq.first.rfind(ParametresCluster::sepExt);
	if (position!=string::npos)
	{
		nomFichierMarq.second=nomFichierMarq.first.substr(position);
		nomFichierMarq.first=nomFichierMarq.first.substr(0, position);		
	}
	cout << nomFichierMarq.first << endl << nomFichierMarq.second << endl;
	
	
	std::ostringstream oss, ossDim, ossNumPremierMarq;
	//string nomRep(chemin+nomFichier+"-"+oss.str()+sepRep);
	//	string nomRep(chemin);
	
	//cout << nomRep << endl;
	
	// Il faut créer les fichiers d'entrée (un par noeud) et les fichiers de sortie (un par dimension)
	
	// Noms des fichiers et flots
	vector<string> nomsFichiers(dimensionMax+1);
	for(int i(0); i<=dimensionMax; ++i)
	{
		oss.str("");
		oss.clear();
		oss << i;
		
		nomsFichiers[i]=(chemin+nomFichierMarq.first+ParametresCluster::suffixeRes+oss.str()+nomFichierMarq.second);
	}
	
	sortie.initialise(nomsFichiers);
	sortie.ouverture();
	
	// Copie et tri des résultats
	nomsFichiers.resize(nbBlocs);
	
	// En-têtes
	vector<string> entete(0);
	listeResultats resultats(0);
	ligneResultat resCourant;
	
	for (int i(0); i<=dimensionMax; ++i)
	{
		ossDim.str("");
		ossDim.clear();
		ossDim << i;
		
		resultats.clear();
		
		int tailleNom(i+1);
		
		for(int j(0); j<nbBlocs; ++j)
		{
			oss.str("");
			oss.clear();
			oss << j;
			ossNumPremierMarq.str("");
			ossNumPremierMarq.clear();
			ossNumPremierMarq << j*tailleBlocs;
			nomsFichiers[j]=(chemin+nomFichierMarq.first+ParametresCluster::suffixeMarq+oss.str()+"-"+ossNumPremierMarq.str()+ParametresCluster::suffixeResPartiel+ossDim.str()+nomFichierMarq.second);
		}
		entree.initialise(nomsFichiers);
		bool etatFlot(true);
		etatFlot=entree.ouverture();
		if (!etatFlot)
		{
			throw Erreur("Problème lors de l'ouverture des fichiers pour la dimension "+ossDim.str()+".");
		}
		for (int j(0); j<nbBlocs; ++j)
		{
			entree.lecture(j, entete);
			
			// Pour chaque ligne, il faut lire le nom et les valeurs du modèle séparément
			while(!entree.finFichier(j))
			{
				entree.lectureGroupe(j, resCourant.first, tailleNom, ' ');
				entree.lecture(j, resCourant.second, ' ');
				resultats.push_back(resCourant);
			}
			cout << "% "<<resultats.size() << endl; 
		}
		
		if (i>0)
		{
			ComparaisonLignesResultats::setCase(2);
			sort(resultats.begin(), resultats.end(), ComparaisonLignesResultats::plusGrandQue);
		}
		
		int nbRes(resultats.size());
		sortie.ecriture(i, entete, true);
		for (int k(0); k<nbRes; ++k)
		{
			sortie.ecriture(i, resultats[k].first, false);
			sortie.ecriture(i, resultats[k].second, true);
		}
			
		entree.fermeture();
		
		
	}
	
	
	sortie.fermeture();
	
	time_t temps_stop(time(NULL));
	
	cout << "Ecriture des résultats : " << difftime(temps_stop, temps_start) << " s." << endl;
	
	
}

Supervision::Supervision(const Supervision& s)
{}

int ComparaisonLignesResultats::caseComparaisonResultats=0;

ComparaisonLignesResultats::ComparaisonLignesResultats()
{}

ComparaisonLignesResultats::~ComparaisonLignesResultats()
{}

int ComparaisonLignesResultats::getCase()
{
	return caseComparaisonResultats;
}

void ComparaisonLignesResultats::setCase(int i)
{
	caseComparaisonResultats=i;
}

bool ComparaisonLignesResultats::plusPetitQue(const ligneResultat  &  r1, const ligneResultat  &  r2) 
{
	return ((r1.second[caseComparaisonResultats])<(r2.second[caseComparaisonResultats]));
}

bool ComparaisonLignesResultats::plusGrandQue(const ligneResultat  &  r1, const ligneResultat  &  r2) 
{
	return ((r1.second[caseComparaisonResultats])>(r2.second[caseComparaisonResultats]));
}


ComparaisonLignesResultats::ComparaisonLignesResultats(ComparaisonLignesResultats& c)
{}



