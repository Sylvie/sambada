#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

typedef enum { A, C, G, T, M } Base;

typedef struct 
{
	char premNuc;
	char secNuc;
	char etiquette;
	string nom;
}
Allele;

typedef map<char, Allele> Dictionnaire;

typedef struct
{
	vector<string> noms;
	vector<string> valeurs;
}
ElementEncodage;

typedef map< vector< Base >, ElementEncodage > EncodageBinaire;

/*void construitValeursPossibles(vector<string>& v, int nbCar, char sep)
 {
 v.clear();
 v.resize(nbCar);
 for (int i(0); i<nbCar; ++i)
 {
 for (int j(0); j<nbCar; ++j)
 {
 v[i]+="0 ";
 }
 v[i][2*i]='1';
 cout << v[i] << endl;
 }
 }*/

void construitValeursPossibles(vector<string>& v, int nbCar, char sep)
{
	v.clear();
	if (nbCar==3)
	{
		v.resize(nbCar);
		cout << "*" << "0"+sep <<"*"<< endl;
		v[0]=string("0");
		v[1]=string("1");
		v[2]=string("2");
	}
}


string construitValeurManquante(int nbCar, string valeurManquante, char sep)
{
	string res(valeurManquante);
	return res;
}

int main(int argc, char** argv)
{	
	char sep(' '), retourLigne('\n'), sepNoms('_');
	char premSymbole('a');
	string valeurManquante("-9");
	
	//cout << sizeof(char) << " " << sizeof(int) <<  endl;
	
	time_t temps1(time(NULL));
	
	// Création des infos alléliques
	
	// Calcul de la table de conversion pour la lecture
	const int nbBases(5);	// Il y a le caractère manquant
	const int nbAlleles(11);
	string listeAlleles[nbAlleles]=		{	"AA",	"CC",	"GG",	"TT",	"AC",	"AG",	"AT",	"CG",	"CT",	"GT",	"??" };
	//char listeEtiquette[nbAlleles] =	{	'#',	'$',	'%',	'&',	':',	';',	'<',	'=',	'>',	'?',	'M' };
	char listeEtiquette[nbAlleles] =	{	'a',	'b',	'c',	'd',	'e',	'f',	'g',	'h',	'i',	'j',	'k' };
	pair<char, Allele> alleleCourante;
	Dictionnaire code;
	// Le dictionnaire envoie un caractère (codage interne) sur les spécifications de codage de l'allèle
	
	for (int i(0); i<nbAlleles; ++i)
	{
		alleleCourante.first=listeEtiquette[i]; 
		alleleCourante.second.premNuc=listeAlleles[i][0];
		alleleCourante.second.secNuc=listeAlleles[i][1];
		alleleCourante.second.etiquette=listeEtiquette[i];
		alleleCourante.second.nom=listeAlleles[i];
		
		code.insert(alleleCourante);
	}
	
	char tabEncodage[nbBases][nbBases] = { { 'a', 'e', 'f', 'g', 'k' }, { 'e', 'b', 'h', 'i', 'k' }, { 'f', 'h', 'c', 'j', 'k' }, { 'g', 'i', 'j', 'd', 'k' }, { 'k', 'k', 'k', 'k', 'k' }  };
	for (int i(0); i< nbBases; ++i)
	{
		for (int j(0); j<nbBases; ++j)
		{
			cout << "(" << tabEncodage[i][j]  << " " << code[tabEncodage[i][j]].nom << ") "; 
		}
		cout << endl;
	}
	
	// Calcul des tables de conversion pour l'écriture
	EncodageBinaire transcription;
	vector< Base > etiquette;
	vector< string > valeursPossibles, valeursDepart;
	ElementEncodage motif;
	int tailleRecodage(0);
	
	// Deux allèles
	tailleRecodage=3;
	construitValeursPossibles(valeursPossibles, tailleRecodage, sep);
	valeursDepart=vector<string> (nbAlleles, "");
	valeursDepart[nbAlleles-1]=construitValeurManquante(tailleRecodage, valeurManquante, sep);
	for (int i(0); i<T; ++i)	// On ne prend pas la valeur manquante "M" en compte pour l'étiquettage
	{
		for (int j(i+1); j<M; ++j)
		{
			etiquette.clear();
			etiquette.push_back((Base)i);
			etiquette.push_back((Base)j);
			
			motif.valeurs=valeursDepart;
			motif.noms.resize(tailleRecodage);
			// 11
			cout << etiquette[0] << endl;
			cout << premSymbole << endl;
			cout << tabEncodage[etiquette[0]][etiquette[0]] << endl;
			cout << tabEncodage[etiquette[0]][etiquette[0]]-premSymbole << endl;
			cout << valeursPossibles[0] << endl;
			motif.valeurs[tabEncodage[etiquette[0]][etiquette[0]]-premSymbole]=valeursPossibles[0];
			motif.noms[0]=code[tabEncodage[etiquette[0]][etiquette[0]]].nom;
			// 12
			motif.valeurs[tabEncodage[etiquette[0]][etiquette[1]]-premSymbole]=valeursPossibles[1];
			motif.noms[1]=code[tabEncodage[etiquette[0]][etiquette[1]]].nom;
			// 22
			motif.valeurs[tabEncodage[etiquette[1]][etiquette[1]]-premSymbole]=valeursPossibles[2];
			motif.noms[2]=code[tabEncodage[etiquette[1]][etiquette[1]]].nom;
			
			transcription.insert(make_pair(etiquette, motif));
			
			EncodageBinaire::iterator iter(transcription.find(etiquette));
			for (int k(0); k<nbAlleles; ++k)
			{
				cout << listeAlleles[k] << " : " << iter->second.valeurs[k] << endl; 
			}
			cout << endl;
		}
	}
	/*
	// Trois allèles
	tailleRecodage=6;
	construitValeursPossibles(valeursPossibles, tailleRecodage, sep);
	valeursDepart=vector<string> (nbAlleles, "");
	valeursDepart[nbAlleles-1]=construitValeurManquante(tailleRecodage, valeurManquante, sep);
	// On supprime le nucléotide exédentaire de l'étiquette
	for (int i(T); i>=A; --i)	// On ne prend pas la valeur manquante "M" en compte pour l'étiquettage
	{
		etiquette.clear();
		for (int j(A); j<M; ++j)
		{
			if(j!=i)
			{
				etiquette.push_back((Base)j);
			}
		}
		
				
		motif.valeurs=valeursDepart;
		motif.noms.resize(tailleRecodage);
		// 11
		motif.valeurs[tabEncodage[etiquette[0]][etiquette[0]]-premSymbole]=valeursPossibles[0];
		motif.noms[0]=code[tabEncodage[etiquette[0]][etiquette[0]]].nom;
		// 22
		motif.valeurs[tabEncodage[etiquette[1]][etiquette[1]]-premSymbole]=valeursPossibles[1];
		motif.noms[1]=code[tabEncodage[etiquette[1]][etiquette[1]]].nom;
		// 33
		motif.valeurs[tabEncodage[etiquette[2]][etiquette[2]]-premSymbole]=valeursPossibles[2];
		motif.noms[2]=code[tabEncodage[etiquette[2]][etiquette[2]]].nom;
		
		// 12
		motif.valeurs[tabEncodage[etiquette[0]][etiquette[1]]-premSymbole]=valeursPossibles[3];
		motif.noms[3]=code[tabEncodage[etiquette[0]][etiquette[1]]].nom;
		// 13
		motif.valeurs[tabEncodage[etiquette[0]][etiquette[2]]-premSymbole]=valeursPossibles[4];
		motif.noms[4]=code[tabEncodage[etiquette[0]][etiquette[2]]].nom;
		// 23
		motif.valeurs[tabEncodage[etiquette[1]][etiquette[2]]-premSymbole]=valeursPossibles[5];
		motif.noms[5]=code[tabEncodage[etiquette[1]][etiquette[2]]].nom;
		
		transcription.insert(make_pair(etiquette, motif));
		
		EncodageBinaire::iterator iter(transcription.find(etiquette));
		for (int k(0); k<nbAlleles; ++k)
		{
			cout << listeAlleles[k] << " : " << iter->second.valeurs[k] << endl; 
		}
		cout << endl;
	}
	
	// Quatre allèles
	tailleRecodage=10;
	construitValeursPossibles(valeursPossibles, tailleRecodage, sep);
	valeursDepart=vector<string> (nbAlleles, "");
	valeursDepart[nbAlleles-1]=construitValeurManquante(tailleRecodage, valeurManquante, sep);
	// Ici toutes les combinaison sont possibles
	motif.valeurs=valeursPossibles;
	motif.noms.resize(tailleRecodage);
	copy(listeAlleles, listeAlleles+(nbAlleles-1), motif.noms.begin());
	transcription.insert(make_pair(etiquette, motif));
	*/
	
	// Lecture de l'encodage des nucléotides choisi par l'utilisateur
	map<char, Base> tableNuc;
	//ifstream listeNucleotides("../../listeNucleotides.txt");
	ifstream listeNucleotides("listeNucleotides.txt");
	
	if (listeNucleotides.fail())
	{
		// 11 fév 2014: liste des nucléotides facultative -> valeur par défaut
		//cerr << "Erreur lors de la lecture des nucléotides." << endl;
		//return 3;
		char lu[nbBases] = {'A','C','G','T','0'};
		for (int i(0); i<nbBases; ++i)
		{
			listeNucleotides>> lu >> ws;
			tableNuc.insert(make_pair(lu[i], (Base)i));
		}
	}
	else
	{
		char lu;
		for (int i(0); i<nbBases; ++i)
		{
			listeNucleotides>> lu >> ws;
			tableNuc.insert(make_pair(lu, (Base)i));
		}
		listeNucleotides.close();
	}
	
	//cout << tableNuc['A'] << " "  << tableNuc['C'] << " " << tableNuc['G'] << " " << tableNuc['T'] << " " <<tableNuc['0'] << endl;
	
	string nomFichierPlink, nomFichierSel, nomFichierSortie, nomFichierPed, nomFichierMap;
	
	bool prendTousEch(true);
	
	if (argc < 5)
	{
		cerr << "At least 4 arguments required." << endl;
		return 1;
	}
	
	// nbEch, nbSNP, nomFichierPlink, nomFichierSortie, nomFichierSel
	
	int nbEch(atoi(argv[1]));
	int nbSNP(atoi(argv[2]));
	cout << nbEch << " £ " << nbSNP << endl;
	nomFichierPlink=argv[3];
	nomFichierSortie=argv[4];
	
	if (argc==6)
	{
		nomFichierSel=argv[5];
		prendTousEch=false;
	}
	
	if (argc>6)
	{
		cerr << "Up to 5 arguments needed." << endl;
		return 1;
	}
	
	int position(nomFichierPlink.find(".ped"));
	if (position!=string::npos)
	{
		nomFichierPed=nomFichierPlink;
		nomFichierMap=nomFichierPlink.substr(0,position)+".map";		
	}
	else
	{
		position=nomFichierPlink.find(".map");
		if (position!=string::npos)
		{
			nomFichierMap=nomFichierPlink;
			nomFichierPed=nomFichierPlink.substr(0,position)+".ped";		
		}
		else
		{
			nomFichierPed=nomFichierPlink+".ped";		
			nomFichierMap=nomFichierPlink+".map";
		}
	}
	cout << nomFichierPlink << " " << nomFichierPed << " " << nomFichierMap <<endl;
	
	
	//cout << nomFichierAnimaux << " " << nomFichierCrd << " " << nomFichierSortie << endl;
	
	ifstream entreePed(nomFichierPed.c_str()), /*entreeMap(nomFichierMap.c_str()),*/ entreeSel(NULL);
	if (!prendTousEch)
	{
		entreeSel.open(nomFichierSel.c_str());
	}
	ofstream sortie(nomFichierSortie.c_str());
	
	if (entreePed.fail() /*|| entreeMap.fail()*/ || (!prendTousEch && entreeSel.fail()) || sortie.fail())
	{
		cerr << "Error while opening files" << endl;
		return 2;
	}
	
	// Lecture des échantillons sélectionnés
	// On utilise un ensemble pour tester si un nom est présent
	set<string> nomsSel;
	int nbSel(0);
	if (prendTousEch)
	{
		nbSel=nbEch;
	}
	else
	{
		string lu;
		entreeSel >> ws;
		cout << "Reading samples names..." << endl;
		while (!entreeSel.eof())
		{
			entreeSel>>lu>> ws;
			nomsSel.insert(lu);
		}
		entreeSel.close();
		nbSel=nomsSel.size();
		cout << "..." << nbSel << " names read." << endl;		// N_g
	}
	
	// La taille du tableau est connue
	vector< vector< char > > data(nbSel, vector<char> (nbSNP));
	vector< vector< int > > comptage(nbSNP, vector<int> (nbBases, 0));
	
	// Lecture des données moléculaires
	const int nbColFixes(6);
	vector< vector<string> > description(nbSel, vector<string> (nbColFixes, ""));
	vector<string> descriptionCourante(nbColFixes, "");
	string lu("");
	char premNuc(' '), secNuc(' ');
	Base premBase, secBase;
	int echCourant(0);
	for (int i(0); i<nbEch; ++i)
	{
		// Lecture des colonnes fixes
		// Le nom de l'échantillon est dans la 2e colonne
		for (int j(0); j<nbColFixes; ++j)
		{
			entreePed >> lu >> ws;
			descriptionCourante[j]=lu;
		}
		//cout << description[echCourant][1] << endl;
		// Cas où l'échantillon est ignoré
		if (!prendTousEch && (nomsSel.find(descriptionCourante[1])==nomsSel.end()))
		{
			//cout << "H" ;
			getline(entreePed, lu);
			entreePed >> ws;
		}
		else
		{
			description[echCourant]=descriptionCourante;
			
			//cout << etiquette[1] << endl;
			for (int j(0); j<nbSNP; ++j)
			{
				//cout << "Truc " << i << " " << j << " ";
				entreePed >> premNuc >>ws >> secNuc >> ws;
				
				premBase=tableNuc[premNuc];
				secBase=tableNuc[secNuc];
				//cout << premBase << secBase << endl;
				
				data[echCourant][j] = tabEncodage[premBase][secBase];
				++comptage[j][premBase];
				++comptage[j][secBase];
			}
			++echCourant;
		}
	}
	entreePed.close();
	time_t temps2(time(NULL));
	cout << "Temps de lecture: "<< difftime(temps2,temps1) << endl;
	
	// Calibration des colonnes et écriture de l'en-tête
	// Il n'y a pas d'espaces dans les identifiants de marqueurs .map -> lecture basée sur les espaces et tabulations
	//sortie << "ID_indiv" << sep;
	vector < pair<bool, EncodageBinaire::iterator > > caracSNP(nbSNP);
	const int nbColMap(4);
	vector<string>ligneMap(nbColMap,"");
	
	for (int i(0); i<nbSNP; ++i)
	{
		etiquette.clear();
		
		for (int j(0); j<(nbBases-1); ++j)
		{
			if (comptage[i][j]>0)
			{
				etiquette.push_back((Base)j);
			}
		}
		if (etiquette.size()==2) // On admet que les SNPs sont bialléliques pour LFMM
		{
			caracSNP[i].first=true;
			caracSNP[i].second=transcription.find(etiquette);
		}
		else
		{
			caracSNP[i].first=false;
		}
		
		
		// Lecture du fichier map
		/*for (int j(0); j<nbColMap; ++j)
		{
			entreeMap >> ligneMap[j] >> ws;
		}
		
		if (caracSNP[i].first)
		{
			if (etiquette.size()>2)
			{
				cout << ligneMap[1] << endl;
			}
			
			
			for (vector<string>::iterator iter(caracSNP[i].second->second.noms.begin()); iter!=caracSNP[i].second->second.noms.end(); ++iter)
			{
				sortie << ligneMap[1]+sepNoms+*iter<<sep;
			}
		} */
	}
	//sortie << retourLigne;
	//entreeMap.close();
	
	// Ecriture des marqueurs
	cout<< description.size() << " " << description[0].size() << " " << description[0][1] << endl;
	for (int i(0); i<nbSel; ++i)
	{
		// nom de l'échantillon
		//sortie << description[i][1] << sep;
		
		for (int j(0); j<(nbSNP-1); ++j)
		{
			if (caracSNP[j].first)
			{
				sortie << caracSNP[j].second->second.valeurs[data[i][j]-premSymbole] << sep;
			}
		}
		sortie << caracSNP[nbSNP-1].second->second.valeurs[data[i][nbSNP-1]-premSymbole] ;
		sortie << retourLigne;
	}
	
	sortie.close();
	
	return 0;
	
}
