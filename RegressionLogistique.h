#ifndef	REGRESSIONLOGISTIQUE_H
#define REGRESSIONLOGISTIQUE_H

#include "matrix.h"
//#include "Partition.h";
#include "Erreur.h"
#include "Toolbox.h"
#include "Archiviste.h"

#include <set>
#include <map>

using namespace std;
using namespace scythe;

typedef pair< int, set< int > > etiquetteModele;
typedef pair< etiquetteModele, vector< reel > > resModele;
typedef map< etiquetteModele , vector< reel > > groupeResultats;

typedef pair<int, reel> Voisin;
typedef vector< vector< Voisin > > TableClassementsVoisins;



class RegressionLogistique
{
protected:	
	//typedef enum {Efron, McFadden, McFaddenAdj, CoxSnell, Nagelkerke, AIC, BIC} listePseudosRcarres;
	//typedef enum {valloglikelihood, Gscore, pValueG, HoG, WaldScore, pValueWald, HoW, PearsonScore, pValuePearson, HoP} longueListeStats;
	typedef enum {valloglikelihood, Gscore, WaldScore, validiteModele, Efron, McFadden, McFaddenAdj, CoxSnell, Nagelkerke, AIC, BIC} listeStats;
	typedef enum {pondDistanceMax, pondGaussienne, pondBicarree, pondPlusProchesVoisins} typePonderation;
	
	//typedef vector< vector <double> > tableau;
	typedef Matrix<reel, Col, Concrete> MatriceReels;
	typedef Matrix<bool, Col, Concrete> MatriceBools;
	typedef Matrix<string, Col, Concrete> MatriceStrings;
	
	
public:
	RegressionLogistique();
	virtual ~RegressionLogistique();
	
	//int initialisation(string nomFichierParam, string nomFichierInput) throw();
	int initialisation(int argc, char *argv[]) throw(Erreur);
	
	//int analyseCategories();
	int calculeCorrelations() const;
	int calculeAutocorrelations() throw(Erreur);
	
	int creeModelesGlobaux();
	
	void ecritResultat(int numFichier, const resModele& r) const;
	
	void ecritResultats() const;
	void ecritResultats(string nomFichier) const;
	ostream& ecritResultats(ostream& sortie) const;
	
protected:
	
	//void generePartitions();
	void construitModele(int numMarq, const set<int> & varContinues); //, const reel loglike_zero, reel& loglike_courante);
	//	void calculeStats(reel loglikeCourante, reel loglikeZero, int nbParamEstimes, vector<double>& statsCourantes, vector<double>& pseudosRcarresCourants);
	bool calculeStats(resModele& resultat, int nbParamEstimes);
	int calculeRegression(reel& loglikeCourante, reel& indiceEfron);
	void calculeGWR(int numMarq,  const set<int> & varContinues,   resModele& resultat);
	
	bool calculePonderation() throw(Erreur);
	
	
private:
	
	// Définition des spécificités d'une variable
	typedef struct
	{
		int number;
		string name;
		bool isNumeric;
		bool isActive;
		int localIndex;
	}
	DetailsVariable;
	
	typedef vector<DetailsVariable> SpecificationsDonnees;

	// Domaine: sous-ensemble des points muni d'une pondération
	typedef struct
	{
	public:
		int nbPoints, taille;	// taille = nb points valides
		MatriceBools masque;	// taille du masque = nbPoints
		vector<int> pointsValides, indices; // pointsValides = liste de taille "taille", indices= indices locaux des points valides
		TableClassementsVoisins poids;
			
		// Mise à jour à partir du masque, recalcule les nombres de points totaux et valides, les vecteurs d'indiçage et redimensionne le poids
		bool miseAJour();
	} Domaine;
	
	
	
protected:
	
	MatriceReels dataEnv;
	MatriceBools dataMarq;
	MatriceStrings dataSupEnv, dataSupMarq;
	vector< set< int > > missingValuesEnv, missingValuesMarq;
	// vector< string > headerEnv, headerMarq;
	SpecificationsDonnees specDataEnv, specDataMarq;
	// Lien entre l'indice local et le numéro global de la variable.
	map<int, int> varEnvActives, varEnvPassives, marqActifs, marqPassifs;
	bool existeColID;
	int colIDEnv, colIDMarq;
	
	// On note les variables environnementales actives et les marqueurs inactifs!
	// -> On traverse plusieurs fois l'ensemble des variables env, mais une seule fois les marqueurs
	//set<int> variables, varActives, markInactifs;
	
	/*set<int> varSpeciales;
	 map < vector<int>, Partition> combinaisons;
	 map < int, vector<int> > etiquettesCombinaisons;*/
	
	int nbEnv;
	int nbEnvActives;
	int nbMarq;
	int nbMarqActifs;
	int nbMarqTot;
	int numPremierMarq;
	int nbPoints;	// Nombre points totaux dans l'échantillon
	int taille;		// Nombre de points valides-> dépend du marqueur considéré
	int nbParam;
	int dimensionMax;
	
	MatriceBools masqueX, masqueY, masque; // masqueX est le masque complet pour toutes les variables environnementales
	//	MatriceReels masqueX, masque; // masqueX est le masque complet pour toutes les variables environnementales
	//	MatriceBools masqueY; // masqueX est le masque complet pour toutes les variables environnementales
	
	
	MatriceReels X, Y, 
	beta_hat, nouv_beta_hat, diff_beta_hat,
	scores, J_info, inv_J_info,
	Xb, nouv_Xb, exp_Xb, pi_hat, interm, intermScores;
	
	vector< groupeResultats > resultats;
	bool sauvegardeExhaustive, sauvegardeTempsReel;
	pair<string, string> nomFichierResultats;
	
	// Paramètres numériques
	const reel eps, convCrit, limiteNaN, limiteExp;
	reel seuilPValeur;
	vector<reel> seuilScore, seuilScoreMultivarie;
	const int limiteIter, limiteEcartType, nbStats, nbStatsSansPseudos, nbPseudosRcarres, tailleEtiquetteInvar;
	
	// Paramètres analyse spatiale
	bool analyseSpatiale, crdCartesiennes; // typeCoordonnees: 0 -> sphériques, 1 -> cartésiennes
	int longitude, latitude, nbPlusProchesVoisins, AS_nbPermutations;
	reel bandePassante;
	typePonderation choixPonderation;
	MatriceReels distances, coordonnees; // Les crd sont extraites lors du calcul de la pondération
	// Voisinage : pour chaque point, liste triée des couples "autres points - distance"
	// Pondération: On stocke les voisins ayant un poids non nul
	// Pondération géo et marq : pondérations-type si pas de valeurs manquantes supplémentaires
	TableClassementsVoisins voisinage;//, ponderation, ponderationGeo, ponderationGeoMarq; 
	Domaine pointsGeo, pointsMarq; // Points aux crd valides avec une pondération "naturelle" (sans norme, un point n'est pas son propre voisin)
	//MatriceBools masqueGeo, masqueGeoMarq, masqueGWR;
	int nbPointsGeo; // Nombre de points avec des coordonnées valides (taille du masqueGeo)
	bool AS_GWR, AS_autocorrGlobale, AS_autocorrLocale, AS_autocorrVarEnv, AS_autocorrMarq, AS_shapefile;
	// Variables pour la régression locale (et le jackknife)
	MatriceReels X_l, Y_l, 
	beta_hat_l, nouv_beta_hat_l, diff_beta_hat_l,
	scores_l, J_info_l, inv_J_info_l,
	Xb_l, nouv_Xb_l, exp_Xb_l, pi_hat_l, interm_l, intermScores_l, hat_matrix_l;
	
	// Flots d'écriture des résultats
	Scribe sortie;
	string delimLignes; // caractère de retour ligne
	char delimMots; // caractère de séparation entre mots
	
	private :
	
	RegressionLogistique(const RegressionLogistique& lr);
	
	void affiche(const etiquetteModele& label);
	void affiche(const resModele& res);
	void affiche(const groupeResultats::iterator res);
	
	//	bool plusPetitQue(const groupeResultats::value_type* const &  r1, const groupeResultats::value_type* const &  r2);
	void trieEtEcritResultats();
	
	// Définition d'un argument pour le fichier de paramètres
	typedef struct 
	{
		string name;
		bool mandatory;
		bool present;
		//bool tokenize;
		vector<string> prereq;	// Liste des pré-requis
		vector<string> contents; 
	} 
	ParameterSetData;
	
	typedef vector<ParameterSetData> ParameterSet;
	typedef map<string, int> ParameterSetIndex;
	
	
	
	void initialisationParametres(ParameterSet& listeParam, ParameterSetIndex& indexParam) const;
	// Cette méthode lit le fichier de paramètres et remplit la liste
	// Elle vérifie aussi si les paramètres obligatoires sont présents
	ifstream& lectureParametres(ifstream& entree, const ParameterSetIndex& index, ParameterSet& parametres) throw();
	
	
	/*	Reel calculePondDistanceMax(Reel dcarre);
	 Reel calculePondGaussienne(Reel dcarre);
	 Reel calculePondBicarree(Reel dcarre);*/
	
	
};

class ComparaisonVoisins
{
public:
	ComparaisonVoisins();
	virtual ~ComparaisonVoisins();
	static bool plusPetitQue(const Voisin& v1, const Voisin& v2);
	static bool plusGrandQue(const Voisin& v1, const Voisin& v2);
protected:
	ComparaisonVoisins(const ComparaisonVoisins& c);
};

class ComparaisonResultats
{
public:
	ComparaisonResultats();
	virtual ~ComparaisonResultats();
	
	static int getCase();
	static void setCase(int i);
	
	static bool plusPetitQue(const groupeResultats::value_type* const &  r1, const groupeResultats::value_type* const &  r2);
	
	static bool plusGrandQue(const groupeResultats::value_type* const &  r1, const groupeResultats::value_type* const &  r2);
	
protected:
	static int caseComparaisonResultats;
	
	ComparaisonResultats(ComparaisonResultats& c);
};


#endif // REGRESSIONLOGISTIQUE_H
