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

#ifndef JOURNALTEMPORAIRE_H
#define JOURNALTEMPORAIRE_H

#include <sstream>
#include <string>
#include <deque>
#include <iostream>

using namespace std;

class JournalTemporaire : public deque<string>
{
public:
	JournalTemporaire();

	virtual ~JournalTemporaire();


	template<class T>
	JournalTemporaire& operator<<(const T& token);

	// JournalTemporaire& operator<<(const string& s);

	JournalTemporaire& operator<<(JournalTemporaire& (*pf)(JournalTemporaire&));

	JournalTemporaire& operator<<(ostream& (*pf)(ostream&));

	JournalTemporaire& retourLigne();

	JournalTemporaire& synchronise();

	//  JournalTemporaire& flush(JournalTemporaire &jt);

protected:
	ostringstream *oss;

	void push(const string& s);

private:
	JournalTemporaire(const JournalTemporaire& jt);
};

template<class T>
JournalTemporaire& JournalTemporaire::operator<<(const T& token)
{
	if (oss == NULL)
	{
		oss = new ostringstream;
	}
	(*oss) << token;
	return *this;
}

JournalTemporaire& nl(JournalTemporaire& jt);

JournalTemporaire& endl(JournalTemporaire& jt);

JournalTemporaire& flush(JournalTemporaire& jt);


#endif // JOURNALTEMPORAIRE_H
