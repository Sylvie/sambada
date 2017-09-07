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


#include "Erreur.h"

using namespace std;

Erreur::Erreur(const string& n, const string& p, bool arret) throw()
:nom(n), phrase(p), fatale(arret)
{}

const char* Erreur::what() const throw()
{
	return phrase.c_str();
}

bool Erreur::estFatale() const throw()
{
	return fatale;
}

string Erreur::getName() const
{
	return nom;
}

string Erreur::getPhrase() const
{
	return phrase;
}

Erreur::~Erreur() throw()
{}

