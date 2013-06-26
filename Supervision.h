#ifndef SUPERVISION_H
#define SUPERVISION_H

#include "Archiviste.h"

using namespace std;

typedef pair< vector< string >, vector< reel > > ligneResultat;
typedef vector< ligneResultat > listeResultats;

typedef enum {G, Wald, AIC, BIC, Both} typeScore;

namespace ParametresCluster {
	
	typedef enum {Windows, Unix} TypeFichier;
	
	// Architecture
	const string Intel("INTEL");
	const string Intel64("X86_64");
	const string PPC("PPC");	
	const string PPC64("PPC64");
	
	// Système d'exploitation
	const string Linux("LINUX");
	const string WinXP("WINNT51");

	// Gestion des formats
	const string sepExt(".");
	
	const string retourLigne("\n");
	const string retourLigne_Unix("\n");
	const string retourLigne_Win("\r\n");

	
	const string sepRep_Unix("/");
	const string sepRep_Win("\\");
	
	const string suffixeMarq("-mark-");
	const string suffixeEnv("-env");
	const string suffixeParam("-param");
	const string suffixeParamLastFile("-paramLast");
	const string suffixeRes("-res-");
	const string suffixeResPartiel("-Out-");
}



class Supervision
{
public:
	Supervision();
	
	virtual ~Supervision();
	
	int preparationsCalculs(const string& nomFichierParam);
	
	int fusionResultats(int argc, char* argv[]) throw();
	
protected:
	
	ParametresCluster::TypeFichier typeFichierOrigine, typeFichierCalcul;
	
	int nbEnv, nbMarq, nbLignes, tailleBlocs, nbBlocs, nbBlocsComplets, TailleDernierBloc, nbNoeudsWindows, nbNoeudsUnix;
	
	string chemin;
	pair<string, string> nomFichierParam, nomFichierMarq, nomFichierEnv;
	
	Supervision(const Supervision& s);
	
	Scribe sortie;
	
	Lecteur entree;
};

class ComparaisonLignesResultats
{
public:
	ComparaisonLignesResultats();
	virtual ~ComparaisonLignesResultats();
	
	static int getCase();
	static void setCase(int i);
	
	static bool plusPetitQue(const ligneResultat  &  r1, const ligneResultat  &  r2);
	
	static bool plusGrandQue(const ligneResultat  &  r1, const ligneResultat  &  r2);
	
protected:
	static int caseComparaisonResultats;
	
	ComparaisonLignesResultats(ComparaisonLignesResultats& c);
};


class ComparaisonTablesResultats
{
public:
	ComparaisonTablesResultats();
	virtual ~ComparaisonTablesResultats();
	
	//static int getCase();
	//static void setCase(int i);
	
	static bool plusPetitQue(const pair<int, reel>  &  r1, const pair<int, reel>  &  r2);
	
	static bool plusGrandQue(const pair<int, reel>  &  r1, const pair<int, reel>  &  r2);
	
protected:
	//static int caseComparaisonResultats;
	
	ComparaisonTablesResultats(ComparaisonTablesResultats& c);
};


#endif // SUPERVISION_H

