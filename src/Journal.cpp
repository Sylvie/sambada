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

#include "Journal.hpp"

using namespace std;

Journal::Journal()
		: flotEnPause(true)
{}

Journal::~Journal()
{
	if (flux.getActiviteFichier())
	{
		flux.fermetureFichier();
	}
}

void Journal::metFlotEnPause()
{
	flotEnPause = true;
}

bool Journal::sortDeLaPause()
{
	if (flux.getActiviteFichier())
	{
		//cerr << "Opening file" << endl;
		flux.ouvertureFichier();
	}
	bool fonctionne((flux.getActiviteFichier() && flux.testeValiditeFichier()) || flux.getActiviteTerminal());
	if (fonctionne)
	{
		flux << temp;
		flux << flush;
		//cerr<< "Fin" << endl;
		flotEnPause = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool Journal::estEnPause() const
{
	return flotEnPause;
}

void Journal::setDelimLignes(const string& delim)
{
	flux.setDelimLignes(delim);
}

string Journal::getDelimLignes() const
{
	return flux.getDelimLignes();
}

void Journal::setDelimMots(const string& delim)
{
	flux.setDelimMots(delim);
}

string Journal::getDelimMots() const
{
	return flux.getDelimMots();
}

void Journal::setNomFichier(const string& nom)
{
	flux.setNomFichier(nom);
}

string Journal::getNomFichier() const
{
	return flux.getNomFichier();
}

void Journal::setDelims(const string& delimL, const string& delimM)
{
	flux.setDelims(delimL, delimM);
}

void Journal::setActiviteTerminal(bool b)
{
	flux.setActiviteTerminal(b);
}

bool Journal::getActiviteTerminal() const
{
	return flux.getActiviteTerminal();
}

void Journal::setActiviteFichier(bool b)
{
	flux.setActiviteFichier(b);
}

bool Journal::getActiviteFichier() const
{
	return flux.getActiviteFichier();
}

void Journal::setActivites(bool term, bool fichier)
{
	flux.setActivites(term, fichier);
}

void Journal::erreurDetectee()
{
	if (!flotEnPause)
	{
		flux.erreurDetectee();
	}
}

Journal& Journal::synchronise()
{
	if (flotEnPause)
	{
		temp.synchronise();
	}
	else
	{
		flux.retourLigne();
	}
	return *this;
}

bool Journal::estFonctionnel()
{
	bool fichierActif(flux.getActiviteFichier()),
			fichierValide(flux.testeValiditeFichier()),
			terminalActif(flux.getActiviteTerminal());
	bool fichierOK(!fichierActif || (fichierActif && fichierValide));
	return ((terminalActif && fichierOK) || (!terminalActif && fichierActif && fichierValide));
}

Journal& Journal::retourLigne()
{
	if (flotEnPause)
	{
		temp.synchronise();
	}
	else
	{
		flux.retourLigne();
		flux.vidange();
	}
	return *this;
}

Journal& Journal::nouvMot()
{
	if (!flotEnPause)
	{
		flux.nouvMot();
	}
	return *this;
}

Journal& Journal::operator<<(Journal& (*pf)(Journal&))
{
/*	if (flotEnPause)
	{
		pf(temp);
	}
	else
	{
		pf(flux);
	}*/
	return pf(*this);
}

Journal& Journal::operator<<(ostream& (*pf)(ostream&))
{
	if (flotEnPause)
	{
		temp << pf;
	}
	else
	{
		flux << pf;
	}
	return *this;
}

Journal& nm(Journal& j)
{
	j.nouvMot();
	return j;
}


Journal& nl(Journal& j)
{
	j.retourLigne();
	return j;
}


Journal& erreur(Journal& j)
{
	j.erreurDetectee();
	return j;
}

void Journal::afficheJournalTemporaire()
{
	cout << "Journal temporaire:" << endl;
	for (deque<string>::iterator i(temp.begin()); i != temp.end(); ++i)
	{
		cout << "(" << i - temp.begin() << ") " << *i << endl;
	}
}
