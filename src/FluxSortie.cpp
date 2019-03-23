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

#include "FluxSortie.h"
#include <iomanip>

FluxSortie::FluxSortie()
		: delimLignes("\n"), delimMots(" "), terminalActif(true), fichierActif(true), fichierOperationnel(false), estMessageErreur(false), ligneVide(true)
{
}

FluxSortie::~FluxSortie()
{
	fermetureFichier();
}

void FluxSortie::setDelimLignes(const string& delim)
{
	delimLignes = delim;
}

string FluxSortie::getDelimLignes() const
{
	return delimLignes;
}

void FluxSortie::setDelimMots(const string& delim)
{
	delimMots = delim;
}

string FluxSortie::getDelimMots() const
{
	return delimMots;
}

void FluxSortie::setNomFichier(const string& nom)
{
	nomFichier = nom;
}

string FluxSortie::getNomFichier() const
{
	return nomFichier;
}

void FluxSortie::setDelims(const string& delimL, const string& delimM)
{
	delimLignes = delimL;
	delimMots = delimM;
}

void FluxSortie::setActiviteTerminal(bool b)
{
	terminalActif = b;
}

bool FluxSortie::getActiviteTerminal() const
{
	return terminalActif;
}

void FluxSortie::setActiviteFichier(bool b)
{
	fichierActif = b;
}

bool FluxSortie::getActiviteFichier() const
{
	return fichierActif;
}

void FluxSortie::setActivites(bool term, bool fichier)
{
	terminalActif = term;
	fichierActif = fichier;
}

bool FluxSortie::ouvertureFichier()
{
	if (testeValiditeFichier())
	{
		return true;
	}

	if (nomFichier.size() == 0)
	{
		return false;
	}

	sortie.close();
	sortie.open(nomFichier.c_str());
	if (sortie.fail())
	{
		return false;
	}

	fichierOperationnel = true;
	return true;

}

void FluxSortie::fermetureFichier()
{
	sortie.close();
	fichierOperationnel = false;
}

bool FluxSortie::testeValiditeFichier()
{
	if (sortie.is_open() && sortie.good())
	{
		fichierOperationnel = true;
	}
	else
	{
		fichierOperationnel = false;
	}


	return fichierOperationnel;
}

void FluxSortie::erreurDetectee()
{
	estMessageErreur = true;
}

FluxSortie& FluxSortie::nouvMot()
{
	ecrit(delimMots);
	return *this;
}

FluxSortie& FluxSortie::vidange()
{
	if (terminalActif)
	{
		cout << flush;
	}
	if (fichierActif)
	{
		sortie << flush;
	}
	return *this;
}

FluxSortie& FluxSortie::retourLigne()
{
	ecrit(delimLignes);
	ligneVide = true;
	return *this;
}

FluxSortie& FluxSortie::operator<<(FluxSortie& (*pf)(FluxSortie&))
{
	ligneVide = false;
	return pf(*this);
}

FluxSortie& FluxSortie::operator<<(ostream& (*pf)(ostream&))
{
	if (terminalActif)
	{
		pf(cout);
	}

	if (fichierActif && fichierOperationnel)
	{
		pf(sortie);
	}

	ligneVide = false;
	return *this;
}

FluxSortie& FluxSortie::operator<<(JournalTemporaire& jt)
{
	jt.synchronise();
	while (!jt.empty())
	{
		ecrit(jt.front());
		retourLigne();
		jt.pop_front();
	}
	ligneVide = true;
	return *this;
}

FluxSortie& nm(FluxSortie& fs)
{
	fs.nouvMot();
	return fs;
}


FluxSortie& nl(FluxSortie& fs)
{
	fs.retourLigne();
	return fs;
}


FluxSortie& erreur(FluxSortie& fs)
{
	fs.erreurDetectee();
	return fs;
}

FluxSortie::FluxSortie(const FluxSortie& fs)
{}
