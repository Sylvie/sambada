/*************************************************************************
 * Copyright (©) 2011-2023 EPFL (Ecole Polytechnique fédérale de Lausanne)
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


#ifndef SUPERVISION_H
#define SUPERVISION_H

#include "modeles/scriptorium/lecteur/FlotEntreeFactory.hpp"
#include "modeles/scriptorium/scribe/FlotSortieFactory.hpp"
#include "common/TypesCommuns.hpp"
#include "modeles/scriptorium/lecteur/Lecteur.hpp"
#include "modeles/scriptorium/scribe/Scribe.hpp"

using namespace std;

//typedef pair< vector< string >, vector< reel > > ligneResultat;
//typedef vector< ligneResultat > listeResultats;

typedef struct
{
	vector<string> etiquette;
	vector<string> valeurs;
	sambada::reel scoreSel;
	sambada::reel scoreTri;
}
		ModeleSupervision;

typedef vector<ModeleSupervision> ListeModelesSupervision;

typedef enum { G, Wald, AIC, BIC, Both } typeScore;


namespace ParametresCluster {

	typedef enum { Windows, Unix } TypeFichier;

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

	// Précision des flots
	const int precisionFlots(22);
}


class Supervision
{
public:
	Supervision();

	virtual ~Supervision();

	void preparationsCalculs(const string& nomFichierParam);

	void fusionResultats(int argc, char *argv[]) throw();

protected:

	ParametresCluster::TypeFichier typeFichierOrigine, typeFichierCalcul;

	int nbEnv, nbMarq, nbLignes, tailleBlocs, nbBlocs, nbBlocsComplets, TailleDernierBloc, nbNoeudsWindows, nbNoeudsUnix;

	string chemin;
	pair<string, string> nomFichierParam, nomFichierMarq, nomFichierEnv;

	Supervision(const Supervision& s) = delete;

private:
	std::unique_ptr<sambada::FlotEntreeFactory> flotEntreeFactory;
	std::unique_ptr<sambada::FlotSortieFactory> flotSortieFactory;

	sambada::Lecteur entree;
	sambada::Scribe sortie;
};

/*class ComparaisonLignesResultats
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
};*/

class ComparaisonModeles
{
public:
	ComparaisonModeles();

	virtual ~ComparaisonModeles();

	//static int getCase();
	//static void setCase(int i);

	static bool plusPetitQue(const ModeleSupervision& r1, const ModeleSupervision& r2);

	static bool plusGrandQue(const ModeleSupervision& r1, const ModeleSupervision& r2);

protected:
	//static int caseComparaisonResultats;

	ComparaisonModeles(ComparaisonModeles& c);
};


#endif // SUPERVISION_H
