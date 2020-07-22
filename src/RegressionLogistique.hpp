/*************************************************************************
 * Copyright (©) 2011-2020 EPFL (Ecole Polytechnique fédérale de Lausanne)
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
#include "Erreur.hpp"
#include "Toolbox.hpp"
#include "Journal.hpp"
#include "histograms/StoreyHistograms.hpp"
#include "modeles/Modele.hpp"
#include "modeles/calibration/TableauNoir.hpp"
#include "modeles/calibration/RegresseurLogistique.hpp"
#include "modeles/scriptorium/scribe/Scribe.hpp"
#include "modeles/scriptorium/scribe/FlotSortieFichierFactory.hpp"
#include "modeles/scriptorium/scribe/ScribeModelesLineairesGeneralises.hpp"
#include "variables/CombinaisonVariables.hpp"
#include "variables/SpecificationsVariables.hpp"

#include <set>
#include <map>
#include <memory>

using namespace std;
using namespace scythe;

typedef pair<int, reel> Voisin;
typedef vector<vector<Voisin> > TableClassementsVoisins;


#if __cplusplus < 201103L
	#define CPPTHROW(X) throw(X)
#else
	#define CPPTHROW(X)
#endif


class RegressionLogistique
{
protected:
	//typedef enum {Efron, McFadden, McFaddenAdj, CoxSnell, Nagelkerke, AIC, BIC} listePseudosRcarres;
	//typedef enum {valloglikelihood, Gscore, pValueG, HoG, WaldScore, pValueWald, HoW, PearsonScore, pValuePearson, HoP} longueListeStats;
	typedef enum { valloglikelihood, Gscore, WaldScore, validiteModele, Efron, McFadden, McFaddenAdj, CoxSnell, Nagelkerke, AIC, BIC, GscorePop, WaldScorePop } listeStats;
	typedef enum { pondDistanceMax, pondGaussienne, pondBicarree, pondPlusProchesVoisins } typePonderation;

	/**
	 * Type de sauvegarde
	 */
	typedef enum {
		all,	/**< sauvegarde exhaustive */
		signif,	/**< sauvegarde des modèles significatifs */
		best	/**< sauvegarde des modèles significatifs ayant au moins un parent valide */
	} typeSelectionModeles;

	/**
	* Type de structure de population
	*/
	typedef enum {
		pasStructurePop,		/**< structure de pop pas prise en compte */
		structurePopPremier,	/**< variables de pop avant les variables environnementales */
		structurePopDernier		/**< variables de pop après les variables environnementales */
	} typeStructurePop;

	typedef Matrix<reel, Col, Concrete> MatriceReels;
	typedef Matrix<bool, Col, Concrete> MatriceBools;
	typedef Matrix<string, Col, Concrete> MatriceStrings;


public:
	RegressionLogistique();

	virtual ~RegressionLogistique();

	static ostream& messageBienvenue(ostream& out, bool versionLongue = false);

	int initialisation(int argc, char *argv[]) CPPTHROW(Erreur);

	int calculeCorrelations() const;
	int calculeAutocorrelations() CPPTHROW(Erreur);

	int creeModelesGlobaux();

	void ecritResultat(int numFichier, const sambada::Modele& r) const;

	void ecritMessage(const string& s, bool nouvLigne = true);

	void terminaison();

	// Méthodes non utilisées pour le moment
	/*void ecritResultats() const;
	void ecritResultats(string nomFichier) const;
	ostream& ecritResultats(ostream& sortie) const;*/

protected:

	//void generePartitions();
	void construitModele(int numMarq, const set<int>& varContinues); //, const reel loglike_zero, reel& loglike_courante);
	//	void calculeStats(reel loglikeCourante, reel loglikeZero, int nbParamEstimes, vector<double>& statsCourantes, vector<double>& pseudosRcarresCourants);
	bool calculeStats(sambada::Modele& resultat, int nbParamEstimes);
	
	void calculeGWR(int numMarq, const set<int>& varContinues, sambada::Modele& resultat);

	void calculePonderation() CPPTHROW(Erreur);

private:

	/**
	 * Domaine: sous-ensemble des points muni d'une pondération
	 */
	typedef struct
	{
	public:
		int nbPoints, taille;    // taille = nb points valides
		MatriceBools masque;    // taille du masque = nbPoints
		vector<int> pointsValides, indices; // pointsValides = liste de taille "taille", indices= indices locaux des points valides
		TableClassementsVoisins poids;

		// Mise à jour à partir du masque, recalcule les nombres de points totaux et valides, les vecteurs d'indiçage et redimensionne le poids
		bool miseAJour();
	} Domaine;

	void calculeDomaineGlobal(Domaine& pointsTot);

	void calculeDomaineMarqueur(const Domaine& pointsTot);

protected:

	MatriceReels dataEnv;
	MatriceBools dataMarq;
	MatriceStrings dataSupEnv, dataSupMarq;
	vector<set<int> > missingValuesEnv, missingValuesMarq;

private:
	sambada::SpecificationsVariables specVarEnv;
	sambada::SpecificationsVariables specMarq;

protected:
	bool existeColID;
	int colIDEnv, colIDMarq;

	int nbEnv;
	int nbEnvActives;
	int nbMarq;
	int nbMarqActifs;
	int nbMarqTot;
	int numPremierMarq;
	int nbPoints;    // Nombre points totaux dans l'échantillon
	int taille;        // Nombre de points valides-> dépend du marqueur considéré
	int nbParam;
	int dimensionMax;

	MatriceBools masqueX, masqueY, masque; // masqueX est le masque complet pour toutes les variables environnementales
	//	MatriceReels masqueX, masque; // masqueX est le masque complet pour toutes les variables environnementales
	//	MatriceBools masqueY; // masqueX est le masque complet pour toutes les variables environnementales

private:
	sambada::FamilleVariables familleVariables;
	sambada::TableauNoir tableauNoir;

	sambada::ConfigurationRegresseurLogistique configurationRegresseurLogistique;
	sambada::RegresseurLogistique regresseurLogistique;

	sambada::FamilleModeles resultats;

protected:
	bool sauvegardeTempsReel;
	typeSelectionModeles selModeles;
	pair<string, string> nomFichierResultats;
	typeStructurePop structurePop;
	set<int> variablesPop;

	// Paramètres numériques
	const reel eps, convCrit, limiteNaN, limiteExp;
	reel seuilPValeur;
	vector<reel> seuilScore, seuilScoreMultivarie;
	const int limiteIter, limiteEcartType, nbStats, nbStatsAvecPop, nbStatsSansPseudos, nbPseudosRcarres, tailleEtiquetteInvar;
	int nbModelesParMarqueur;

private:
	// Paramètres FDR selon Storey
	std::unique_ptr<sambada::StoreyHistograms> storey;

protected:
	bool calculeStorey;
	bool appliqueSeuilScoreStorey;
	reel seuilScoreStorey;

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
	bool AS_spatialLag;

private:
	// Variables pour la régression locale (et le jackknife)
	sambada::TableauNoir tableauNoirLocal;

	// Flots d'écriture des résultats
	sambada::FlotSortieFichierFactory flotSortieFichierFactory;
	sambada::Scribe sortie;
	sambada::ScribeModelesLineairesGeneralises scribeModelesLineairesGeneralises;
	string delimLignes; // caractère de retour ligne
	char delimMots; // caractère de séparation entre mots

protected:
	// Journal d'exécution
	Journal journal;
	// Journal des modèles divergents
	FluxSortie modelesDivergents;

	// Ecriture du journal temporaire en cas d'erreur fatale au début de l'initialisation
	void dumpJournalTemporaire();

	static vector<string> getMessageBienvenue(bool versionLongue = false);

	void messageBienvenue(bool versionLongue = false);

private :

	RegressionLogistique(const RegressionLogistique& lr);

	void affiche(const sambada::EtiquetteModele& label);

	void affiche(const sambada::Modele& res);

	void affiche(const sambada::GenerationModeles::iterator res);


	//	bool plusPetitQue(const groupeResultats::value_type* const &  r1, const groupeResultats::value_type* const &  r2);
	void trieEtEcritResultats();

	// Définition d'un argument pour le fichier de paramètres
	typedef struct
	{
		string name;
		bool mandatory;
		bool present;
		//bool tokenize;
		vector<string> prereq;    // Liste des pré-requis
		vector<string> contents;
	}
	ParameterSetData;

	typedef vector<ParameterSetData> ParameterSet;
	typedef map<string, int> ParameterSetIndex;


	void initialisationParametres(ParameterSet& listeParam, ParameterSetIndex& indexParam) const;

	// Cette méthode lit le fichier de paramètres et remplit la liste
	// Elle vérifie aussi si les paramètres obligatoires sont présents
	ifstream& lectureParametres(ifstream& entree, const ParameterSetIndex& index, ParameterSet& parametres) CPPTHROW(Erreur);

	// This method creates a new Erreur, write the description to the log and throw the Erreur
	void erreurDetectee(const string& nom="", const string& description="", bool arret=true) CPPTHROW(Erreur);

	bool calculeStructurePop(int dimensionCourante) const;

	bool estVariablePop(string variable) const;

	bool inclutToutesVariablesPop(set<int> variables) const;

	bool estModeleEligiblePourStructurePopulation(set<int> variables) const;
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

	static bool plusPetitQue(const sambada::GenerationModeles::value_type *const& r1, const sambada::GenerationModeles::value_type *const& r2);

	static bool plusGrandQue(const sambada::GenerationModeles::value_type *const& r1, const sambada::GenerationModeles::value_type *const& r2);

protected:
	static int caseComparaisonResultats;

	ComparaisonResultats(ComparaisonResultats& c);
};


#endif // REGRESSIONLOGISTIQUE_H
