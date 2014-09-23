/*************************************************************************
* Copyright (©) 2011-2014 EPFL (Ecole Polytechnique fédérale de Lausanne)
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



#ifndef ARCHIVISTE_H
#define ARCHIVISTE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "Toolbox.h"

using namespace std;

class Archiviste
{
	public :
	Archiviste();
	virtual ~Archiviste();
	
	virtual void initialise(const vector<string>& noms);
	virtual bool ouverture()=0;
	
	
	virtual void fermeture();
	
	virtual void precision(int p);
	virtual int precision() const;
	
	virtual string getRetourLigne() const;
	virtual void setRetourLigne(const string& s);

	virtual char getDelimMots() const;
	virtual void setDelimMots(const char c);

protected:
	Archiviste(const Archiviste&);
	
	int nbFichiers;
	bool enService;
	vector<string> nomsFichiers;
	vector<fstream*> fichiers;
	
	string signeRetourLigne;
	char delimMots;
};

class Scribe : public Archiviste
{
	public :
	Scribe();
	virtual ~Scribe();
	
	virtual bool ouverture();
	
	template <typename T>
	bool ecriture(int numFichier, T element, bool retourLigne=false) const;
	
	template <class T>
	bool ecriture(int numFichier, const vector<T>& ligne, bool retourLigne=false) const;
	
	template <class T>
	bool ecriture(int numFichier, const set<T>& ligne, bool retourLigne=false) const;
	
	template <typename T>
	bool ecriture(int numFichier, const vector< vector< T > >& document) const;
	
	
	
	//virtual void fermeture();
	
	
	
protected:
	Scribe(const Scribe&);
	
	
	
};

class Lecteur : public Archiviste
{
	public :
	Lecteur();
	virtual ~Lecteur();
	
	virtual bool ouverture();
	
	// Lecture d'une ligne jusqu'au caractère de fin 
	
	// Retourne TRUE si tous les éléments ont pu être lus
	template <class T>
	bool lecture(int numFichier, vector<T>& ligne, char delimMots=' ') const;
		
	// Retourne TRUE si tous les éléments ont pu être lus
	template <class T>
	bool lecture(int numFichier, vector< vector<T> >& document, int nbLignes=-1, bool ajout=false, char delimMots=' ') const;
	
	// Lecture d'un groupe de mots
	
	// Retourne TRUE si tous les éléments ont pu être lus
	template <class T>
	bool lectureGroupe(int numFichier, vector<T>& groupe, int nombre, char delimMots=' ') const;
	
	bool finFichier(int numFichier);
		
	//virtual void fermeture();
	
	
protected:
	Lecteur(const Lecteur&);
	
	
};



template <typename T>
bool Scribe::ecriture(int numFichier, T element, bool retourLigne) const
{
	if (numFichier<0 || numFichier>=nbFichiers)
	{
		return false;
	}
	else 
	{
		
		*(fichiers[numFichier]) << element << delimMots;
		
		if (retourLigne)
		{
			*(fichiers[numFichier]) << signeRetourLigne;
		}
		return true;
	}
}	


template <class T>
bool Scribe::ecriture(int numFichier, const vector<T>& ligne, bool retourLigne) const
{
	if (numFichier<0 || numFichier>=nbFichiers)
	{
		return false;
	}
	else 
	{
		int taille(ligne.size());
		for (int i(0); i<taille; ++i)
		{
			*(fichiers[numFichier]) << ligne[i] << delimMots;
		}
		if (retourLigne)
		{
			*(fichiers[numFichier]) << signeRetourLigne;
		}
		return true;
	}
	
}

template <class T>
bool Scribe::ecriture(int numFichier, const set< T >& ligne, bool retourLigne) const
{
	if (numFichier<0 || numFichier>=nbFichiers)
	{
		return false;
	}
	else 
	{
		for (typename set< T >::iterator i(ligne.begin()); i!=ligne.end(); ++i)
		{
			*(fichiers[numFichier]) << *i << delimMots;
		}
		if (retourLigne)
		{
			*(fichiers[numFichier]) << signeRetourLigne;
		}
		return true;
	}
	
}

template <typename T>
bool Scribe::ecriture(int numFichier, const vector< vector< T > >& document) const
{
	if (numFichier<0 || numFichier>=nbFichiers)
	{
		return false;
	}
	else 
	{
		int nbLignes(document.size()), nbMots(0);
		for (int i(0); i<nbLignes; ++i)
		{
			nbMots=document[i].size();
			for (int j(0); j<nbMots; ++j)
			{
	
				*(fichiers[numFichier]) << document[i][j] << delimMots;
			}
			*(fichiers[numFichier]) << signeRetourLigne;
			
		}
		return true;
	}
	
}

// Retourne TRUE si tous les éléments ont pu être lus
template <class T>
bool Lecteur::lecture(int numFichier, vector<T>& ligne, char delimMots) const
{
	vector<string> ligneBrute;
	toolbox::lectureLigne(*fichiers[numFichier], ligneBrute, delimMots);
	ligne.clear();
	bool testEchec;
	T valeur(0);
	for (vector<string>::iterator i(ligneBrute.begin()); i!=ligneBrute.end(); ++i)
	{
		valeur=toolbox::conversion<T>(*i, testEchec);
		//cout << valeur << endl;
		if (!testEchec)
		{
			ligne.push_back(valeur);
		}
	}
	return ((ligne.size())==ligneBrute.size());
}

// Spécialisation pour lire une ligne de texte
template <>
inline
bool Lecteur::lecture<string>(int numFichier, vector<string>& ligne, char delimMots) const
{
	ligne.clear();
	toolbox::lectureLigne(*fichiers[numFichier], ligne, delimMots);
	return true;
}


// Retourne TRUE si tous les éléments ont pu être lus
template <class T>
bool Lecteur::lecture(int numFichier, vector< vector< T > >& document, int nbLignes, bool ajout, char delimMots) const
{
	int nbLignesLues(0);
	if (!ajout)
	{
		document.clear();
	}
	vector<T> ligne;
	bool validation(true);
	while (!fichiers[numFichier]->eof() && (nbLignes==-1 || nbLignesLues<nbLignes))
	{
		//document.push_back();
		cout << "    " << nbLignesLues << " " << fichiers[numFichier]->eof() << endl;
		validation=validation && lecture(numFichier, ligne, delimMots);
		document.push_back(ligne);
		++nbLignesLues;
	}
    return validation;
}

// Retourne TRUE si tous les éléments ont pu être lus
template <class T>
bool Lecteur::lectureGroupe(int numFichier, vector<T>& groupe, int nombre, char delimMots) const
{
	vector<string> groupeBrut;
	string mot("");
	for (int i(0); (!fichiers[numFichier]->eof()) && i<nombre; ++i)
	{
		toolbox::lectureMot(*fichiers[numFichier], mot, delimMots);
		groupeBrut.push_back(mot);
	}
	
	groupe.clear();
	bool testEchec;
	T valeur(0);
	for (vector<string>::iterator i(groupeBrut.begin()); i!=groupeBrut.end(); ++i)
	{
		valeur=toolbox::conversion<T>(*i, testEchec);
		cout << valeur << endl;
		if (!testEchec)
		{
			groupe.push_back(valeur);
		}
	}
	return ((groupe.size())==groupeBrut.size());
}

// Spécialisation pour lire une ligne de texte
template <>
inline
bool Lecteur::lectureGroupe<string>(int numFichier, vector<string>& groupe, int nombre, char delimMots) const
{
	groupe.clear();
	string mot("");
	for (int i(0); (!fichiers[numFichier]->eof()) && i<nombre; ++i)
	{
		toolbox::lectureMot(*fichiers[numFichier], mot, delimMots);
		groupe.push_back(mot);
	}
	return true;
}


#endif // ARCHIVISTE_H 
