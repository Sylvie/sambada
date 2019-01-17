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


#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <numeric>
#include "matrix.h"

using namespace std;
typedef long double reel;
//typedef double reel;

namespace toolbox {

	const streamsize precisionLecture(22);

	reel invCDF_ChiSquare(reel pValeur, int deglib, reel seuilConv);

	/*template <class T> 
	 ifstream& lectureLigne(ifstream& entree, vector<T>& ligne, bool validation, vector<int>& lineValidation);
	 
	 template <class T> 
	 ifstream& lectureLigne(ifstream& entree, vector<T>& ligne, vector<int>& lineValidation);
	 
	 template <class T> 
	 ifstream& lectureLigne(ifstream& entree, vector<T>& ligne);
	 
	 template <class T>
	 T conversion(const string& lu);*/

	void segmentationString(string ligne, vector<string>& resultat, const string& separateur = " ", bool elimineEspaces = false);

	double combinaisons(int taille, int nb);

	//int calculeIndice(const vector<int>& v, int nbVar);

	template<typename T, scythe::matrix_order PO, scythe::matrix_style PS>
	reel sommeNumerique(const scythe::Matrix<T, PO, PS>& A)
	{
		return (std::accumulate(A.begin_f(), A.end_f(), (reel) 0));
	}

	template<typename T, scythe::matrix_order PO, scythe::matrix_style PS>
	scythe::Matrix<T, PO, PS> sommeNumeriqueCol(const scythe::Matrix<T, PO, PS>& A)
	{
		int nbColonnes(A.cols());
		scythe::Matrix<T, PO, PS> sommes(1, nbColonnes, true, 0);
		for (int i(0); i < nbColonnes; ++i)
		{
			sommes(0, i) = toolbox::sommeNumerique(A(scythe::_, i));
		}
		return sommes;
	}

	/*	
	 template <class T>
	 istream& lectureLigne(istream& entree, vector< T >& ligne, bool validation, vector<int>& lineValidation, bool recuperation, vector<string>& tokensRecuperes, char delimLignes='\n', char delimMots=' ')
	 {
	 
	 ligne.clear();
	 if (validation)
	 {
	 lineValidation.clear();
	 }
	 if (recuperation)
	 {
	 tokensRecuperes.clear();
	 }
	 
	 std::string row, dustbin, token;
	 T currentValue;
	 
	 int position(0);
	 
	 std::getline(entree, row, delimLignes);
	 entree >> ws;
	 //cout << "*" << row << "*" << endl;
	 
	 while(row.size()>0)
	 {
	 position = row.find(delimMots);
	 if (position == 0)
	 {
	 token = "";
	 row = row.substr(1);
	 }
	 else if (position != std::string::npos)
	 {
	 token = row.substr(0, position);
	 row = row.substr(position);
	 }
	 else 
	 {
	 token = row;
	 row = "";
	 }
	 
	 if (token.size()>0)
	 {
	 std::istringstream rowstream(token);
	 rowstream.precision(precisionLecture);
	 rowstream >> currentValue;
	 if (rowstream.fail())
	 {
	 rowstream.clear();
	 rowstream>>dustbin;
	 ligne.push_back(0);
	 if (validation)
	 {
	 lineValidation.push_back(ligne.size()-1);
	 }
	 if (recuperation)
	 {
	 tokensRecuperes.push_back(token);
	 }
	 }
	 else
	 {
	 ligne.push_back(currentValue);
	 }
	 
	 }
	 }
	 }
	 
	 template <class T>
	 istream& lectureLigne(istream& entree, vector< T >& ligne, vector<int>& lineValidation, vector<string>& tokensRecuperes, char delimLignes='\n', char delimMots=' ')
	 {
	 return lectureLigne(entree, ligne, true, lineValidation, true, tokensRecuperes, delimLignes, delimMots);
	 }
	 
	 
	 template <class T>
	 istream& lectureLigne(istream& entree, vector< T >& ligne, vector<int>& lineValidation,  char delimLignes='\n', char delimMots=' ')
	 {
	 vector<string> tokens(0);
	 return lectureLigne(entree, ligne, true, lineValidation, false, tokens, delimLignes, delimMots);
	 }
	 
	 template <class T>
	 istream& lectureLigne(istream& entree, vector< T >& ligne,  char delimLignes='\n', char delimMots=' ')
	 {
	 vector<int> truc(0);
	 vector<string> tokens(0);
	 return lectureLigne(entree, ligne, false, truc, false, tokens, delimLignes, delimMots);
	 }
	 */

	template<class T>
	T conversion(const string& lu, bool& echec)
	{
		T resultat;
		istringstream iss;
		iss.precision(precisionLecture);
		iss.str(lu);
		iss >> resultat;
		echec = iss.fail();
		return resultat;
	}

	template<class T>
	T conversion(const string& lu)
	{
		bool echec;
		return conversion<T>(lu, echec);
	}

	string conversion(reel nombre);

	void enleveEspaces(string& s);

	// Cette fonction repère le type de fin de ligne
	// Elle sert à formater les fichiers de résultats
	istream& chercheRetourLigne(istream& entree, string& retourLigne);


	// Lit les caractères du flots et forme un mot
	// La lecture s'arrête si le caractère séparateur ou un caractère invisible est trouvé 
	// Si un '"' est lu, la lecture continue jusqu'au '"' suivant, 
	// les caractères invisibles sont ignorés, mais les espaces sont conservés
	// La fonction retourne le dernier caractère lu
	char lectureMot(istream& entree, string& mot, char delimMots = ' ', bool gardeSignesInvisibles = false);

	// Lit une ligne mot-à-mot jusqu'à trouver un caractère de fin de ligne
	// Si la fin du fichier est détectée, la fonction renvoie TRUE
	// Les mots vides (0 car) ne sont pas enregistrés
	bool lectureLigne(istream& entree, vector<string>& ligne, char delimMots = ' ', bool gardeSignesInvisibles = false);


	// Lit une ligne puis tente de la convertir dans le type T
	// Le vecteur validation permet de connaître les indices des mots qui n'ont pas pu être convertis
	// Le vecteur ligneOriginale permet de récupérer la ligne telle qu'elle a été lue (sous forme de chaînes)
	// Les booléens "validation" et "recuperation" permettent de sélectionner les options
	template<class T>
	istream& lectureLigne(istream& entree, vector<T>& ligne, bool validation, vector<int>& lineValidation, bool recuperation, vector<string>& ligneOriginale, char delimMots = ' ')
	{

		ligne.clear();

		if (validation)
		{
			lineValidation.clear();
		}
		if (recuperation)
		{
			ligneOriginale.clear();
		}

		// Lecture de la ligne sous forme de mots
		lectureLigne(entree, ligneOriginale, delimMots);

		int tailleLigne(ligneOriginale.size());

		// Initialisation de la ligne de résultats, valeur par défaut: 0
		ligne.resize(tailleLigne, 0);

		T currentValue(0);
		bool echec;
		// Pour chaque mot, on essaie de le convertir, puis on le copie dans la ligne de résultats
		// Si la conversion échoue, le résultat vaut 0 (via l'initalisation de "ligne") et l'indice du mot inconvertible est ajouté dans "lineValidation"
		for (int i(0); i < tailleLigne; ++i)
		{
			currentValue = conversion<T>(ligneOriginale[i], echec);
			if (echec)
			{
				if (validation)
				{
					lineValidation.push_back(i);
				}
			}
			else
			{
				ligne[i] = currentValue;
			}
		}
		return entree;
	}

	// Lit une ligne et tente de convertir les mots en T
	// Surcharge de la fonction principale, plus pratique à utiliser, si les vecteurs sont présents, les options sont réglées sur TRUE
	template<class T>
	istream& lectureLigne(istream& entree, vector<T>& ligne, vector<int>& lineValidation, vector<string>& ligneOriginale, char delimMots = ' ')
	{
		return lectureLigne(entree, ligne, true, lineValidation, true, ligneOriginale, delimMots);
	}

	// Lecture et conversion d'une ligne sans récupérer les mots
	template<class T>
	istream& lectureLigne(istream& entree, vector<T>& ligne, vector<int>& lineValidation, char delimMots = ' ')
	{
		vector<string> tokens(0);
		return lectureLigne(entree, ligne, true, lineValidation, false, tokens, delimMots);
	}

	// Lecture et conversion d'une ligne sans récupérer ni les mots, ni les indices des mots n'ayant pas pu être convertis
	template<class T>
	istream& lectureLigne(istream& entree, vector<T>& ligne, char delimMots = ' ')
	{
		vector<int> truc(0);
		vector<string> tokens(0);
		return lectureLigne(entree, ligne, false, truc, false, tokens, delimMots);
	}

	/*
	// Méthodes d'affichage pour le débug
	template <class T>
	void affiche(const vector<T>& v, int n=-1)
	{
		int taille(v.size());
		if (n==-1)
		{
			n=taille;
		}
		for (int i(0); i<n; ++i)
		{
			cout << v[i] << " " ;
		}
		cout << endl;
	}
	
	template <class T>
	void affiche(const vector< vector< T > >& v, int n=-1, int m=-1)
	{
		int taille(v.size());
		if (n==-1)
		{
			n=taille;
		}
		for (int i(0); i<n; ++i)
		{
			affiche(v[i], m);
		}
		cout << endl;
	}
	
	void affiche(const vector< vector < pair<int, reel> > > v, int n=-1, int m=-1);
	*/

	void messageBienvenue(bool versionLongue = false);
}

class ComparaisonVecteurs
{
public:
	ComparaisonVecteurs();

	virtual ~ComparaisonVecteurs();

	static int getCase();

	static void setCase(int i);

	template<class T>
	static bool plusPetitQue(const vector<T>& v1, const vector<T>& v2)
	{
		return (v1[caseComparaisonVecteurs] < v2[caseComparaisonVecteurs]);
	}

	template<class T>
	static bool plusGrandQue(const vector<T>& v1, const vector<T>& v2)
	{
		return (v1[caseComparaisonVecteurs] > v2[caseComparaisonVecteurs]);
	}

protected:
	static int caseComparaisonVecteurs;

	ComparaisonVecteurs(ComparaisonVecteurs& c);
};

#endif // TOOLBOX_H
