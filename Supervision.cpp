#include "Supervision.h"
#include "Toolbox.h"
#include "Erreur.h"
#include <ctime>
#include <algorithm>
#include <map>

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
	time_t temps_start(time(NULL));
	
	
	// nomProg nomFichierInput nbBlocs tailleBlocs dimMax
	// nomProg nomFichierInput nbBlocs tailleBlocs dimMax choixScore seuilScore choixTri
	bool seuilSelection(false);
	reel seuilScore(0);
	typeScore scoreSel(Both);
	typeScore scoreTri(Wald);
	
	// Lien entre le type de score et le numéro de colonne
	map<typeScore, int> numColonnes;
	numColonnes.insert(make_pair(G,1));
	numColonnes.insert(make_pair(Wald,2));
	numColonnes.insert(make_pair(AIC,9));
	numColonnes.insert(make_pair(BIC,10));
	if (argc==7 || argc==8)
	{
		string lu(argv[5]);
		if (lu=="G")
		{
			scoreSel=G;
		}
		else if(lu=="Wald")
		{
			scoreSel=Wald;
		}
		
		seuilScore=atof(argv[6]);
		seuilSelection=true;
	}
	if (argc==8) 
	{
		string lu(argv[7]);
		if (lu=="G") 
		{
			scoreTri=G;
		}
		if (lu=="AIC") 
		{
			scoreTri=AIC;
		}
		if (lu=="BIC") 
		{
			scoreTri=BIC;
		}
		
	}
	else if (argc!=5 && argc!=7 && argc!=8)
	{
		cout << argc << endl;
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
	
//	time_t temps_start(time(NULL));
	
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
	pair<int, reel> indiceCourant;
	vector<pair<int,reel> > tableRes(0);
	
	typedef struct {int a; reel b;} chose;
	cout << "& " << sizeof(ligneResultat) << " " << sizeof(pair<int,reel>)<< " " << sizeof(int)<< " " << sizeof(double)<< " " << sizeof(reel) << endl ;
	ligneResultat v;
	v.first=vector<string> (2,"Abracadabra343Vercingetorix");
	v.second=vector<reel>(13, 83.6);
	cout << "& " << sizeof(v) << " "<< sizeof(vector<string>) << " "<< sizeof(vector<reel>) << " "<< sizeof(string("Abracadabra343VercingetorixAbracadabra343VercingetorixAbracadabra343Vercingetorix"))<< endl;
	int nombreRes(0);
	reel scoreCourant;
	time_t temps_fin_prep(time(NULL));
	cout << "Temps initialisation: " << difftime(temps_fin_prep, temps_start) << endl;
	for (int i(0); i<=dimensionMax; ++i)
	{
		time_t temps_debut_lecture(time(NULL));

		ossDim.str("");
		ossDim.clear();
		ossDim << i;
		
		resultats.clear();
		tableRes.clear();
		time_t t1(time(NULL));
		resultats.reserve(pow(10.0,7));
		tableRes.reserve(pow(10.0,7));
		time_t t2(time(NULL));
		cout << "Temps reserve : " << difftime(t2, t1)<< endl;
		nombreRes=0;
		
		int tailleNom(i+1);
		
		
		// Cas où il n'y a qu'un seul fichier
		if (nbBlocs==1) 
		{
			nomsFichiers[0]=(chemin+nomFichierMarq.first+ParametresCluster::suffixeResPartiel+ossDim.str()+nomFichierMarq.second);
			cout << nomsFichiers[0] << endl;
		}
		else 
		{
			
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
			if (!seuilSelection || i==0) 
			{
				
				while(!entree.finFichier(j))
				{
					entree.lectureGroupe(j, resCourant.first, tailleNom, ' ');
					entree.lecture(j, resCourant.second, ' ');
					resultats.push_back(resCourant);
					indiceCourant.first=nombreRes;
					tableRes.push_back(indiceCourant);
					++nombreRes;
				}
			}
			else
			{
				while(!entree.finFichier(j))
				{
					entree.lectureGroupe(j, resCourant.first, tailleNom, ' ');
					entree.lecture(j, resCourant.second, ' ');
					if (scoreSel==Both)
					{
						scoreCourant=min(resCourant.second[numColonnes[G]], resCourant.second[numColonnes[Wald]]);
					}
					else if (scoreSel==G)
					{
						//cout << numColonnes[G] << " " << resCourant.second.size() << endl;
						scoreCourant=resCourant.second[numColonnes[G]];
					}
					else if (scoreSel==Wald)
					{
						scoreCourant=resCourant.second[numColonnes[Wald]];						
					}

					/* Au cas où on veut sélectionner les pires modèles
					 if (scoreCourant<=seuilScore)*/
					
					if (scoreCourant>=seuilScore)
					{
						
						resultats.push_back(resCourant);
						indiceCourant.first=nombreRes;
						indiceCourant.second=scoreCourant;
						tableRes.push_back(indiceCourant);
						++nombreRes;
					}
				}
				
			}
			cout << "% "<<resultats.size() << endl; 
		}
		time_t temps_fin_lecture(time(NULL));
		cout << "Temps lecture (dim " << i << "):" << difftime(temps_fin_lecture, temps_debut_lecture) << endl;
		if (i>0)
		{
			ComparaisonLignesResultats::setCase(2);
			sort(tableRes.begin(), tableRes.end(), ComparaisonTablesResultats::plusGrandQue);
		}
		
		time_t temps_fin_tri(time(NULL));
		cout << "Temps tri (dim " << i << "):" << difftime(temps_fin_tri,temps_fin_lecture) << endl;

		
		int nbRes(resultats.size());
		sortie.ecriture(i, entete, true);
		for (int k(0); k<nbRes; ++k)
		{
			sortie.ecriture(i, resultats[tableRes[k].first].first, false);
			sortie.ecriture(i, resultats[tableRes[k].first].second, true);
		}
		
		time_t temps_fin_ecriture(time(NULL));
		cout << "Temps ecriture (dim " << i << "):" << difftime(temps_fin_ecriture, temps_fin_tri) << endl;

		
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



//int ComparaisonLignesResultats::caseComparaisonResultats=0;

ComparaisonTablesResultats::ComparaisonTablesResultats()
{}

ComparaisonTablesResultats::~ComparaisonTablesResultats()
{}

/*int ComparaisonLignesResultats::getCase()
{
	return caseComparaisonResultats;
}

void ComparaisonLignesResultats::setCase(int i)
{
	caseComparaisonResultats=i;
}*/

bool ComparaisonTablesResultats::plusPetitQue(const pair<int, reel>  &  r1, const pair<int, reel>  &  r2) 
{
	return ((r1.second)<(r2.second));
}

bool ComparaisonTablesResultats::plusGrandQue(const pair<int, reel>  &  r1, const pair<int, reel>  &  r2) 
{
	return ((r1.second)>(r2.second));
}


ComparaisonTablesResultats::ComparaisonTablesResultats(ComparaisonTablesResultats& c)
{}
