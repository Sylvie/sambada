/*************************************************************************
 * Copyright (©) 2011-2022 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

#include "JournalTemporaire.hpp"

JournalTemporaire::JournalTemporaire()
		: deque<string>(), oss(NULL)
{
}

JournalTemporaire::~JournalTemporaire()
{
}

/*JournalTemporaire& JournalTemporaire::operator<<(const string& s)
{
    oss.str(oss.str()+s);
    return *this;
}*/

JournalTemporaire& JournalTemporaire::operator<<(JournalTemporaire& (*pf)(JournalTemporaire&))
{
	return pf(*this);
}

JournalTemporaire& JournalTemporaire::operator<<(ostream& (*pf)(ostream&))
{
	if (oss == NULL)
	{
		oss = new ostringstream;
	}
	pf(*oss);
	return *this;
}

JournalTemporaire& JournalTemporaire::retourLigne()
{
	if (oss != NULL)
	{
		this->push(oss->str());
		delete oss;
		oss = NULL;
	}
	return *this;
}

JournalTemporaire& JournalTemporaire::synchronise()
{
	if (oss != NULL && !(oss->str().empty()))
	{
		retourLigne();
	}
	return *this;
}

void JournalTemporaire::push(const string& s)
{
	deque<string>::push_back(s);
}

/*JournalTemporaire& JournalTemporaire::flush(JournalTemporaire &jt)
{
    synchronise();
    return *this;
}*/


JournalTemporaire& nl(JournalTemporaire& jt)
{
	jt.retourLigne();
	return jt;
}

JournalTemporaire& endl(JournalTemporaire& jt)
{
	jt << nl;
	return jt;
}

JournalTemporaire& flush(JournalTemporaire& jt)
{
	jt.synchronise();
	return jt;
}

JournalTemporaire::JournalTemporaire(const JournalTemporaire& jt)
{}
