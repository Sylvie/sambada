/*************************************************************************
 * Copyright (©) 2011-2015 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

#include "Chronometre.h"
#include <cmath>
#include <iomanip>

using namespace std;

Chronometre::Chronometre()
:nbTotEvenements(0), nbMesures(0), prochaineMesure(10), tempsDebut(0),
journal(NULL), precision(0, 2, 2, 2), chablon(false, false), distanceColonnes(2), sepCol("  "),
tailleAffichageIterations(0), tailleAffichageDurees(0), tailleAffichageTotal(0)
{}

Chronometre::~Chronometre()
{}

int Chronometre::initialisation(Journal* j, int nbTotEv, int numPremMesure, const string& separateurCol)
{
	journal = j;
	nbTotEvenements=nbTotEv;
	tailleAffichageIterations=(int) ceil(log10(1.*nbTotEv));
	prochaineMesure=numPremMesure;
	//int nbCourant(numPremMesure);
	tempsDebut=time(NULL);
	
	sepCol=separateurCol;
	distanceColonnes=sepCol.size();
	
	(*journal) << "Start of measurement, " << nbTotEvenements << " iterations expected." << nl;	
	
	return numPremMesure;
}

int Chronometre::mesureEtAffiche(int numEvenement)
{
	if (numEvenement < 0)
	{
		numEvenement = prochaineMesure;
	}	
	mesure(numEvenement);
	if (mesures.size()==1)
	{
		// Première mesure
		ajusteAffichage(mesures[0].tempsTotEstime);
		tailleAffichageDurees = Duree::calculeLargeur(precision, chablon);

		//cout << "Taille : " << tailleAffichageDurees << endl;
		string texteMarqueurs((((nbTotEvenements < 100) && (sepCol.size()<3)) ? "markers" : "# markers"));
		vector<string> texteTemps(5, "");
//		texteTemps[0] = "Computation time (*=estimation)";
		texteTemps[0] = "Computation time";
		texteTemps[4] = "(*=estimation)";
		texteTemps[1] = "Elapsed";
		tailleAffichageDurees = max(tailleAffichageDurees, (int)texteTemps[1].size());
		texteTemps[2] = "Remaining*";
		tailleAffichageDurees = max(tailleAffichageDurees, (int)texteTemps[2].size());
		texteTemps[3] = "Total*";
		tailleAffichageDurees = max(tailleAffichageDurees, (int)texteTemps[3].size());
		
		tailleAffichageTotal = tailleAffichageIterations + 3*tailleAffichageDurees + 4*sepCol.size() + 4;
		
		// Ligne horizontale
		(*journal) << sepCol << setfill('-') << setw(tailleAffichageTotal) << "" << sepCol <<nl << setfill(' ');

		(*journal) << sepCol <<setw(tailleAffichageIterations + sepCol.size() + 4) << texteMarqueurs << sepCol
			<< setw(3*tailleAffichageDurees + 2*sepCol.size() - texteTemps[4].size()) << left << texteTemps[0] << texteTemps[4] << sepCol << nl;
		
		(*journal) << sepCol << setw(tailleAffichageIterations + sepCol.size() + 4) << left << "" << sepCol
			//<< setw(4) << "%" << sepCol
			<< setw(tailleAffichageDurees) << left << texteTemps[1] << sepCol
			<< setw(tailleAffichageDurees) << left << texteTemps[2] << sepCol
			<< setw(tailleAffichageDurees) << left << texteTemps[3] << sepCol << nl;
		
		// Ligne horizontale
		(*journal) << sepCol << setfill('-') << setw(tailleAffichageTotal) << "" << sepCol <<nl << setfill(' ');

	}
	
	(*journal) << sepCol << setw(tailleAffichageIterations) << right << numEvenement << sepCol 
		<< setw(3) << right << mesures[nbMesures-1].proportionAccomplieEnPourcent << "%" << sepCol
		<< setw(tailleAffichageDurees) << right <<  DureeFormatee(mesures[nbMesures-1].tempsEcoule, precision, chablon) << sepCol
		<< setw(tailleAffichageDurees) << right << DureeFormatee(mesures[nbMesures-1].tempsTotEstime, precision, chablon) << sepCol	
		<< setw(tailleAffichageDurees) << right << DureeFormatee(mesures[nbMesures-1].tempsRestantEstime, precision, chablon) << sepCol << nl;
	
	prochaineMesure=calculeProchaineMesure(numEvenement);
	
	return prochaineMesure;

}

void Chronometre::fin()
{
	// Ligne horizontale
	if (mesures.size() > 0)
	{
		(*journal) << sepCol << setfill('-') << setw(tailleAffichageTotal) << "" << sepCol <<nl << setfill(' ');
	}
	Duree d(difftime(time(NULL),tempsDebut));
	/*(*journal) << "End of measurement, total time: "; 
	affiche(d);
	(*journal) << nl;*/
	(*journal) << "End of measurement, total time: " << DureeFormatee(d, precision, chablon) << nl;
}

int Chronometre::calculeProportion(int numEv)
{
	double prop(100.*(numEv)/nbTotEvenements);
	return (int) floor(prop);
}


void Chronometre::mesure(int numEv)
{
	time_t tempsCourant(time(NULL));
	int tempsEcoule(difftime(tempsCourant, tempsDebut));
	int tempsTotEstime(ceil(1.*nbTotEvenements/numEv * tempsEcoule));
	
	Mesure m={numEv, calculeProportion(numEv), Duree(tempsEcoule), Duree(tempsTotEstime), Duree(tempsTotEstime-tempsEcoule)};
	/*m.iteration=numEv;
	m.proportionAccomplieEnPourcent=calculeProportion(numEv);
	m.tempsEcoule=Duree(tempsEcoule);
	m.tempsTotEstime=Duree(tempsTotEstime);
	m.tempsRestantEstime=Duree(tempsTotEstime-tempsEcoule);*/
	
	mesures.push_back(m);
	++nbMesures;
}

int Chronometre::calculeProchaineMesure(int numEv)
{
	double prop(1.*(numEv+1.)/nbTotEvenements);
	
	if (prop < 0.01)
	{
		prochaineMesure=ceil(0.01*nbTotEvenements);
	}
	else if (prop < 0.05)
	{
		prochaineMesure=ceil(0.05*nbTotEvenements);		
	}
	/*else if (prop < 0.1)
	{
		prochaineMesure=ceil(0.1*nbTotEvenements);
	}*/
	else
	{
		prochaineMesure = ceil(nbTotEvenements * (floor(prop/0.1+1.)/10.));
	}
	return prochaineMesure;
}


void Chronometre::affiche(const Duree& d)
{
	d.affiche((*journal), precision, chablon);
}

void Chronometre::ajusteAffichage(const Duree& dureeEstimee)
{
	// Si la durée est de moins d'une heure, on affiche les secondes
	if (dureeEstimee.plusCourte(Duree(0,1,0,0)))
	{
		chablon.zappeJours=true;
		chablon.zappeSecondes=false;
	}
	// Si la durée est en 1 et 20 heures, on affiche seulement les heures et les minutes
	else if (dureeEstimee.plusCourte(Duree(0,20,0,0)))	
	{
		chablon.zappeJours=true;
		chablon.zappeSecondes=true;		
	}
	else 
	{
		chablon.zappeJours=false;
		chablon.zappeSecondes=true;
		precision.jours=dureeEstimee.calculeTailleAffichageJours();
	}	
}

