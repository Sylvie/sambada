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

#ifndef JOURNAL_H
#define JOURNAL_H

#include "FluxSortie.h"

using namespace std;

class Journal
{
public:
	Journal();
	virtual ~Journal();

	void metFlotEnPause();
	bool sortDeLaPause();
	bool estEnPause() const;

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

    void erreurDetectee();

	Journal& synchronise();

	bool estFonctionnel();

    template<class T>
    Journal& ecrit(const T& token);

    Journal& retourLigne();

    Journal& nouvMot();

    template<class T>
    Journal& operator<<(const T& token);

    Journal& operator<<(Journal& (*pf)(Journal&));

    Journal& operator<<(ostream& (*pf)(ostream&));

	void afficheJournalTemporaire();


protected:

	FluxSortie flux;
	JournalTemporaire temp;

	bool flotEnPause;


private:
	Journal(const Journal& j);

};

Journal& nl (Journal& j);

Journal& nm (Journal& j);

Journal& erreur (Journal& j);

template<class T>
Journal& Journal::operator<<(const T& token)
{
	if (flotEnPause)
	{
		temp << token;
	}
	else
	{
		flux << token;
	}
	return *this;
}


#endif // JOURNAL_H
