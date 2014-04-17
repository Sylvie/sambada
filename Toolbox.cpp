/*************************************************************************
* Copyright (�) 2011-2014 EPFL (Ecole Polytechnique f�d�rale de Lausanne)
* Laboratory of Geographic information systems (LaSIG)
* 
* This file is part of Sambada.
*  
* Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.
* Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
* You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.
* 
* Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), St�phane Joost (stephane.joost@epfl.ch) 
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


#include "Toolbox.h"
#include "distributions.h"
using namespace std;
using namespace scythe;

void toolbox::segmentationString(string ligne, vector<string>& resultat, const string& separateur, bool elimineEspaces)
{
	resultat.clear();
	string dustbin, token, word;
	int position(0), tailleSep(separateur.size());
	cout << ligne <<  endl <<flush;
	//cout << "£" << tailleSep << "£" <<separateur << "£"<< endl;
	while(ligne.size()>0)
	{
		position = ligne.find(separateur);
		if (position == 0)
		{
			token = "";
			ligne = ligne.substr(tailleSep);
			cout << "*" << token <<"* *" << ligne << "*" << endl;

		}
		else if (position != std::string::npos)
		{
			token = ligne.substr(0, position);
			ligne = ligne.substr(position+tailleSep);
			cout << "=" << token <<"= =" << ligne << "=" << endl;
		}
		else 
		{
			token = ligne;
			enleveEspaces(token);
			if (token==separateur)
			{
				token ="";
			}
			ligne = "";
			//cout << "?1" << token <<"?2 ?3" << ligne << "?4 " << token.size() << endl << 'ç' << token << 'ç' << endl;

		}
		if (elimineEspaces)
		{
			while (token[0]==' ')
			{
				token=token.substr(1);	
			}
			while (token[token.size()-1] == ' ')
			{
				token=token.substr(0, token.size()-1);
			}
		}
		if (token.size()>0)
		{
			resultat.push_back(token);
		}
	}	
}

reel toolbox::invCDF_ChiSquare(reel pValeur, int deglib, reel seuilConv)
{
	reel score(1.0);
	reel residu(pchisq(score, deglib)-pValeur);
	int compteur(0), limiteIter(1000);
	do
	{
		score = score - (pchisq(score, deglib)-pValeur)/dchisq(score, deglib);
		residu=pchisq(score, deglib);
		++compteur;
		//cout << x << " " << chisq.prob(x)+chisq.valeur <<" " << valeur << endl;
	}
	while ((abs(residu)>seuilConv) && (compteur<limiteIter));
	
	return score;
}


double toolbox::combinaisons(int taille, int nb)
{
	if (taille==0 || nb ==0 || taille<nb)
	{
		return 1;
	}
	double resultat(1);
	for (int i(nb+1); i<=taille; ++i)
	{
		resultat*=i;
	}
	for (int i(1); i<=(taille-nb); ++i)
	{
		resultat/=i;
		
	}
	return resultat;
}


/*
int calculeIndice(const vector<int>& v, int nbVar)
{
	int dim(v.size());
	double dimCourante(dim-1);
	int indice(0), dernier(-1);
	
	for (int i(0); i<dim; ++i)
	{
		for (int j(dernier+1); j<v[i]; ++j)
		{
			indice+=combinaisons(nbVar-j-1, dimCourante);
		}
		dernier = v[i];
		--dimCourante;
	}
	return indice;
}
*/

string toolbox::conversion(reel nombre)
{
	ostringstream oss;
	oss.precision(precisionLecture);
	oss << nombre;
	return oss.str();
}


/*
 template <class T>
 ifstream& lectureLigne(ifstream& entree, vector< T >& ligne, bool validation, vector<int>& lineValidation)
 {
 ligne.clear();
 if (validation)
 {
 lineValidation.clear();
 }
 std::string row, dustbin, token;
 T currentValue;
 
 int position(0);
 
 std::getline(entree, row);
 
 while(row.size()>0)
 {
 position = row.find(" ");
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
 }
 else
 {
 ligne.push_back(currentValue);
 }
 
 }
 }
 }
 
 template <class T>
 ifstream& lectureLigne(ifstream& entree, vector< T >& ligne, vector<int>& lineValidation)
 {
 return lectureLigne(entree, ligne, true, lineValidation);
 }
 
 template <class T>
 ifstream& lectureLigne(ifstream& entree, vector< T >& ligne)
 {
 vector<int> truc(0);
 return lectureLigne(entree, ligne, false, truc);
 }*/

void toolbox::enleveEspaces(string& s)
{
	while (s[0]==' ')
	{
		s=s.substr(1);
	}
	int dernierCar(s.size()-1);
	while (s[dernierCar]==' ')
	{
		s=s.substr(0, dernierCar);
		--dernierCar;
	}
}	

// Cette fonction repère le type de fin de ligne
// Elle sert à formater les fichiers de résultats
istream& toolbox::chercheRetourLigne(istream& entree, string& retourLigne)
{
	retourLigne.clear();
	char lu;
	bool continueLecture(true);
	while (continueLecture && !( (entree.get(lu)).eof() ) ) 
	{
		//cout << "&" << (int)lu << "&" << endl;
		if (lu == '\r')
		{
			if (entree.peek() == '\n')
			{
				retourLigne="\r\n";
			}
			else
			{
				retourLigne="\r";
			}
			continueLecture=false;

		}
		else if (lu=='\n')
		{
			retourLigne='\n';
			continueLecture=false;
		}
	}
	entree.clear();
	entree.seekg(0); // Rembobinage
	return entree;
}

// Lit les caractères du flots et forme un mot
// La lecture s'arrête si le caractère séparateur ou un caractère invisible est trouvé 
// Si un '"' est lu, la lecture continue jusqu'au '"' suivant, 
// les caractères invisibles sont ignorés, mais les espaces sont conservés
// La fonction retourne le dernier caractère lu
char toolbox::lectureMot(istream& entree, string& mot, char delimMots, bool gardeSignesInvisibles)
{
	mot.clear();
	char  lu=0x00;
	bool inner(false), continueLecture(true);
	while (continueLecture && !((entree.get(lu)).eof())) 
	{
		if (lu=='"')
		{
			inner = !inner;
		}
		// On ôte les "
		
		else if (inner==0 && (lu==delimMots || lu=='\r' || lu=='\n' ||  (!gardeSignesInvisibles && ( (lu<0x20) || (lu>=0x7F)) ) ) )	// Les espaces sont conservés
		{
			continueLecture=false;
		}
		else if( gardeSignesInvisibles  ||( lu>0x19 && lu<0x7F) )	// On est à l'intérieur, les espaces sont conservés
		{
			mot.push_back(lu);
		}
	}
	return lu;
}

bool toolbox::lectureLigne(istream& entree, vector<string>& ligne, char delimMots, bool gardeSignesInvisibles)
{
	ligne.clear();
	string mot("");
	char lu=0x00;
	bool continueLecture(true);
	while (continueLecture && !entree.eof())
	{
		lu=lectureMot(entree, mot, delimMots, gardeSignesInvisibles);
		if (mot.size()>0)
		{
			ligne.push_back(mot);
		}
		if (lu=='\n' || lu=='\r')
		{
			continueLecture=false;
			while (iscntrl(entree.peek()))
			{
				entree.get(lu);
			}
		}
	}
	return entree.eof();
}
/*
void toolbox::	 affiche(const vector< vector < pair<int, reel> > > v, int n, int m)
{
	int taille(v.size());
	if (n==-1)
	{
		n=taille;
	}
	int limite(0);
	for (int i(0); i<n; ++i)
	{
		if (m==-1)
		{
			limite=v[i].size();
		}
		else
		{
			limite=m;
		}
		for (int j(0); j<limite; ++j)
		{
			cout << v[i][j].first << " " << v[i][j].second << "; " ;
		}
		cout << endl;
	}
	cout << endl;
}
*/

void toolbox::messageBienvenue(bool versionLongue)
{
	cout << "| Welcome to Sambada!\n"
	<< "| Copyright (C) 2011-2014 EPFL (Ecole Polytechnique federale de Lausanne)\n"
	<< "| Laboratory of Geographic information systems (LaSIG)\n";
	if (!versionLongue)
	{
		
		cout << "| Contact: sylvie.stucki@a3.epfl.ch, stephane.joost@epfl.ch\n"
		<< "| Sambada is free software and comes with ABSOLUTELY NO WARRANTY.\n"
		<< "| Please refer to the file AUTHORS for details.\n";
	}
	else
	{
		cout << "| Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.\n"
			<< "| Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n"
			<< "| You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.\n|\n"
			
			<< "| Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), Stephane Joost (stephane.joost@epfl.ch)\n" 
			<< "| Laboratory of Geographic information systems\n"
			<< "| EPFL ENAC IIE LASIG\n"
			<< "| Station 18\n"
			<< "| CH-1015 Lausanne\n"
			<< "| Web site : http://lasig.epfl.ch/sambada\n|\n"
			
			<< "| Sambada includes two libraries: Scythe Statistical Library (under GPL 3) and Shapefile C Library (under LGPL 2.1, courtesy of Frank Warmerdam).\n|\n"
			
			<< "| Scythe Statistical Library\n"
			<< "| Copyright (C) 2000-2002 Andrew D. Martin and Kevin M. Quinn;\n"
			<< "| 2002-2012 Andrew D. Martin, Kevin M. Quinn, and Daniel Pemstein.  All Rights Reserved.\n|\n"
	
			<< "| Shapefile C Library\n"
			<< "| Copyright (c) 1999, Frank Warmerdam\n";
	}
	
}


int ComparaisonVecteurs::caseComparaisonVecteurs=0;

ComparaisonVecteurs::ComparaisonVecteurs()
{}

ComparaisonVecteurs::~ComparaisonVecteurs()
{}

int ComparaisonVecteurs::getCase()
{
	return caseComparaisonVecteurs;
}

void ComparaisonVecteurs::setCase(int i)
{
	caseComparaisonVecteurs=i;
}

/*bool ComparaisonVecteurs::compare(const vector<reel>& v1, const vector<reel>& v2)
{
	return (v1[caseComparaisonVecteurs] < v2[caseComparaisonVecteurs]);
}*/

ComparaisonVecteurs::ComparaisonVecteurs(ComparaisonVecteurs& c)
{}


