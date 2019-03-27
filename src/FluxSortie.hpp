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

#ifndef FLUXSORTIE_H
#define FLUXSORTIE_H

#include <iostream>
#include <fstream>
#include <string>
#include "JournalTemporaire.h"

using namespace std;


class FluxSortie
{
public:
	FluxSortie();

	virtual ~FluxSortie();

	void setDelimLignes(const string& delim);

	string getDelimLignes() const;

	void setDelimMots(const string& delim);

	string getDelimMots() const;

	void setNomFichier(const string& nom);

	string getNomFichier() const;

	void setDelims(const string& delimL, const string& delimM);

	void setActiviteTerminal(bool b);

	bool getActiviteTerminal() const;

	void setActiviteFichier(bool b);

	bool getActiviteFichier() const;

	void setActivites(bool term, bool fichier);

	bool ouvertureFichier();

	void fermetureFichier();

	bool testeValiditeFichier();

	void erreurDetectee();

	FluxSortie& synchronise();

	template<class T>
	FluxSortie& ecrit(const T& token);

	FluxSortie& retourLigne();

	FluxSortie& nouvMot();

	FluxSortie& vidange();

	template<class T>
	FluxSortie& operator<<(const T& token);

	FluxSortie& operator<<(FluxSortie& (*pf)(FluxSortie&));

	FluxSortie& operator<<(ostream& (*pf)(ostream&));

	FluxSortie& operator<<(JournalTemporaire& j);


protected:
	string delimLignes; // caractère de retour ligne
	string delimMots; // caractère de séparation entre les mots
	bool terminalActif;
	bool fichierActif, fichierOperationnel;
	bool estMessageErreur;
	bool ligneVide;

	string nomFichier;
	ofstream sortie;

private:
	FluxSortie(const FluxSortie& fs);
};

FluxSortie& nl(FluxSortie& fs);

FluxSortie& nm(FluxSortie& fs);

FluxSortie& erreur(FluxSortie& fs);

template<class T>
FluxSortie& FluxSortie::ecrit(const T& token)
{
	if (terminalActif && !estMessageErreur)
	{
		cout << token;
	}
	else if (terminalActif && estMessageErreur)
	{
		cout << token << flush;
	}

	if (fichierActif && fichierOperationnel && !estMessageErreur)
	{
		sortie << token;
	}
	else if (fichierActif && fichierOperationnel && estMessageErreur)
	{
		sortie << token << flush;
	}

	estMessageErreur = false;

	return *this;
}

template<class T>
FluxSortie& FluxSortie::operator<<(const T& token)
{
	return ecrit(token);
}


#endif // FLUXSORTIE_H
