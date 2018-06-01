/*************************************************************************
* Copyright (©) 2011-2018 EPFL (Ecole Polytechnique fédérale de Lausanne)
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



#include "Archiviste.h"

using namespace std;

Archiviste::Archiviste()
:nbFichiers(0), nomsFichiers(0), fichiers(0), signeRetourLigne("\n"), delimMots(' ')
{
}

Archiviste::~Archiviste()
{
	fermeture();
/*	for (int i(0); i<nbFichiers; ++i)
	{
			if (fichiers[i]->is_open())
			{
				fichiers[i]->close();
				delete fichiers[i];
			}
	}
	enService=false;
 */
}

Archiviste::Archiviste(const Archiviste&)
{}

void Archiviste::initialise(const vector<string>& noms)
{
	nbFichiers=noms.size();
	nomsFichiers=noms;

}


void Archiviste::fermeture()
{
	if(enService)
	{
	for (int i(0); i<nbFichiers; ++i)
	{
		if (fichiers[i]->is_open())
		{
			fichiers[i]->close();
			delete fichiers[i];
		}
	}
	}
	enService=false;
}

void Archiviste::precision(int p)
{
	if (enService)
	{
	for (int i(0); i<nbFichiers; ++i)
	{
		fichiers[i]->precision(p);
	}
	}
}

int Archiviste::precision() const
{
	if (nbFichiers>0)
	{
		return fichiers[0]->precision();
	}
	else {
		return 0;
	}

}


string Archiviste::getRetourLigne() const
{
	return signeRetourLigne;
}


void Archiviste::setRetourLigne(const string& s)
{
	signeRetourLigne=s;
}

char Archiviste::getDelimMots() const
{
	return delimMots;
}


void Archiviste::setDelimMots(const char c)
{
	delimMots=c;
}



Scribe::Scribe()
:Archiviste()
{
}

Scribe::~Scribe()
{
}



bool Scribe::ouverture()
{
	int precisionRes(22);
	fichiers.resize(nbFichiers);
	for (int i(0); i<nbFichiers; ++i)
	{
		//cerr << "*" << fichiers[i] << endl;
		fichiers[i]=new fstream(nomsFichiers[i].c_str(), ios::out);
		fichiers[i]->precision(precisionRes);

		if (fichiers[i]->fail())
		{
			fichiers[i]->clear();
			return false;
		}

	}
	enService=true;
	return true;
}

Lecteur::Lecteur()
:Archiviste()
{
}


bool Lecteur::ouverture()
{
	fichiers.resize(nbFichiers);
	for (int i(0); i<nbFichiers; ++i)
	{
		fichiers[i]=new fstream(nomsFichiers[i].c_str(), ios::in);

		if (fichiers[i]->fail())
		{
			cout << fichiers[i]->rdstate() << endl;
			fichiers[i]->clear();
			return false;
		}

	}
	enService=true;
	return true;
}

Lecteur::~Lecteur()
{
}

bool Lecteur::finFichier(int numFichier)
{
	*fichiers[numFichier]>>ws;
	return(fichiers[numFichier]->eof());
}
