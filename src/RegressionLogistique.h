/*************************************************************************
 * Copyright (©) 2011-2019 EPFL (Ecole Polytechnique fédérale de Lausanne)
 * Laboratory of Geographic information systems (LaSIG)
 *
 * This file is part of Sambada.
 *
 * Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.
 * Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), Stéphane Joost (stephane.joost@epfl.ch)
 * Laboratory of Geographic information systems
 * EPFL ENAC IIE LASIG
 * Station 18
 * CH-1015 Lausanne
 * Web site : http://lasig.epfl.ch/sambada
 *
 * Sambada includes two libraries: Scythe Statistical Library (under GPL 3) and Shapefile C Library (under LGPL 2.1, courtesy of Frank Warmerdam).
 *
 * Scythe Statistical Library
 * Copyright (C) 2000-2002 Andrew D. Martin and Kevin M. Quinn;
 * 2002-2012 Andrew D. Martin, Kevin M. Quinn, and Daniel Pemstein.  All Rights Reserved.
 *
 * Shapefile C Library
 * Copyright (c) 1999, Frank Warmerdam
 *************************************************************************/


#ifndef	REGRESSIONLOGISTIQUE_H
#define REGRESSIONLOGISTIQUE_H

#include "matrix.h"
#include "Erreur.h"
#include "Toolbox.h"
#include "Archiviste.h"
#include "Journal.h"


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
	typedef enum {valloglikelihood, Gscore, WaldScore, validiteModele, Efron, McFadden, McFaddenAdj, CoxSnell, Nagelkerke, AIC, BIC, GscorePop, WaldScorePop} listeStats;
	typedef enum {pondDistanceMax, pondGaussienne, pondBicarree, pondPlusProchesVoisins} typePonderation;

	/* Type de sauvegarde
	 ALL : sauvegarde exhaustive
	 VALID: sauvegarde des modèles significatifs
	 BEST: sauvegarde des modèles significatifs ayant au moins un parent valide
	 */
	typedef enum {all, signif, best} typeSelectionModeles;

	/* Type de structure de population
 	pasStructurePop : structure de pop pas prise en compte
 	structurePopPremier: variables de pop avant les variables environnementales
 	structurePopDernier: variables de pop après les variables environnementales
 	*/
	typedef enum {pasStructurePop, structurePopPremier, structurePopDernier} typeStructurePop;

	typedef Matrix<reel, Col, Concrete> MatriceReels;
	typedef Matrix<bool, Col, Concrete> MatriceBools;
	typedef Matrix<string, Col, Concrete> MatriceStrings;


public:
	RegressionLogistique();
	virtual ~RegressionLogistique();

	static ostream& messageBienvenue(ostream& out, bool versionLongue=false);

	int initialisation(int argc, char *argv[]) throw(Erreur);

	int calculeCorrelations() const;
	int calculeAutocorrelations() throw(Erreur);

	int creeModelesGlobaux();

	void ecritResultat(int numFichier, const resModele& r) const;

	void ecritMessage(const string& s, bool nouvLigne = true);

	void terminaison();

	// Méthodes non utilisées pour le moment
	/*void ecritResultats() const;
	void ecritResultats(string nomFichier) const;
	ostream& ecritResultats(ostream& sortie) const;*/

protected:

	//void generePartitions();
	void construitModele(int numMarq, const set<int> & varContinues); //, const reel loglike_zero, reel& loglike_courante);
	//	void calculeStats(reel loglikeCourante, reel loglikeZero, int nbParamEstimes, vector<double>& statsCourantes, vector<double>& pseudosRcarresCourants);
	bool calculeStats(resModele& resultat, int nbParamEstimes);
	int calculeRegression(reel& loglikeCourante, reel& indiceEfron);
	void calculeGWR(int numMarq,  const set<int> & varContinues,   resModele& resultat);

	void calculePonderation() throw(Erreur);


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
	SpecificationsDonnees specDataEnv, specDataMarq;
	// Lien entre l'indice local et le numéro global de la variable.
	map<int, int> varEnvActives, varEnvPassives, marqActifs, marqPassifs;
	bool existeColID;
	int colIDEnv, colIDMarq;

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
	bool sauvegardeTempsReel;
	typeSelectionModeles selModeles;
	pair<string, string> nomFichierResultats;
	typeStructurePop structurePop;

	// Paramètres numériques
	const reel eps, convCrit, limiteNaN, limiteExp;
	reel seuilPValeur;
	vector<reel> seuilScore, seuilScoreMultivarie;
	const int limiteIter, limiteEcartType, nbStats, nbStatsAvecPop, nbStatsSansPseudos, nbPseudosRcarres, tailleEtiquetteInvar;
	int nbModelesParMarqueur;

	// Paramètres analyse spatiale
	bool analyseSpatiale, crdCartesiennes; // typeCoordonnees: 0 -> sphériques, 1 -> cartésiennes
	int longitude, latitude, nbPlusProchesVoisins, AS_nbPermutations;
	reel bandePassante;
	typePonderation choixPonderation;
	MatriceReels distances, coordonneesBrutes, coordonnees; // Les crd sont extraites lors du calcul de la pondération
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

    // Journal d'exécution
    Journal journal;
    // Journal des modèles divergents
    FluxSortie modelesDivergents;

    // Ecriture du journal temporaire en cas d'erreur fatale au début de l'initialisation
    void dumpJournalTemporaire();

    static vector<string> getMessageBienvenue(bool versionLongue=false);
    void messageBienvenue(bool versionLongue=false);

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
	ifstream& lectureParametres(ifstream& entree, const ParameterSetIndex& index, ParameterSet& parametres) throw(Erreur);

	// This method creates a new Erreur, write the description to the log and throw the Erreur
	void erreurDetectee(const string& nom="", const string& description="", bool arret=true) throw(Erreur);

	bool calculeStructurePop(int dimensionCourante) const;
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
