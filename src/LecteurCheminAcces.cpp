/*************************************************************************
* Copyright (©) 2011-2019 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

#include "LecteurCheminAcces.h"

#include <iostream>

CheminAcces::CheminAcces(const std::string& chemin, const std::string& radical, const std::string& extension)
		: chemin(chemin), radical(radical), extension(extension)
{}

CheminAcces::CheminAcces(const CheminAcces& cheminAcces)
		: CheminAcces(cheminAcces.chemin, cheminAcces.radical, cheminAcces.extension)
{}

CheminAcces::CheminAcces()
{}

CheminAcces::~CheminAcces()
{

}

bool CheminAcces::operator==(const CheminAcces& autre) const
{
	return chemin == autre.chemin && radical == autre.radical && extension == autre.extension;
}


LecteurCheminAcces::LecteurCheminAcces()
{}

LecteurCheminAcces::LecteurCheminAcces(const LecteurCheminAcces& cheminAcces)
{}

LecteurCheminAcces::~LecteurCheminAcces()
{

}

CheminAcces LecteurCheminAcces::decompose(const std::string& cheminComplet)
{
	CheminAcces cheminAcces;

	std::string separateurStandard(getSeparateurCheminStandard());
	std::string separateurSysteme(getSeparateurCheminSysteme());
	size_t positionSeparateurStandard(cheminComplet.rfind(separateurStandard));
	size_t positionSeparateurSysteme(cheminComplet.rfind(separateurSysteme));

	std::string nomFichier("");
	if (positionSeparateurStandard == std::string::npos)
	{
		if (positionSeparateurSysteme == std::string::npos)
		{
			nomFichier = cheminComplet;
		}
		else
		{
			cheminAcces.chemin = cheminComplet.substr(0, positionSeparateurSysteme + 1);
			nomFichier = cheminComplet.substr(positionSeparateurSysteme + 1);
		}
	}
	else
	{
		if (positionSeparateurSysteme == std::string::npos)
		{
			cheminAcces.chemin = cheminComplet.substr(0, positionSeparateurStandard + 1);
			nomFichier = cheminComplet.substr(positionSeparateurStandard + 1);
		}
		else
		{
			size_t positionSeparateur(std::max(positionSeparateurStandard, positionSeparateurSysteme));
			cheminAcces.chemin = cheminComplet.substr(0, positionSeparateur + 1);
			nomFichier = cheminComplet.substr(positionSeparateur + 1);
		}
	}

	size_t positionPoint(nomFichier.rfind("."));

	if (positionPoint == std::string::npos)
	{
		cheminAcces.radical = nomFichier;
	}
	else
	{
		cheminAcces.radical = nomFichier.substr(0, positionPoint);
		cheminAcces.extension = nomFichier.substr(positionPoint);
	}

	return cheminAcces;
}

std::string LecteurCheminAcces::getSeparateurCheminSysteme() const
{
	std::string separateur(getSeparateurCheminStandard());
	std::string separateurSysteme("");
#ifdef sambada_folder_separator
	separateurSysteme = sambada_folder_separator;
#endif
	if (separateurSysteme.size() > 0)
	{
		separateur = separateurSysteme;
	}
	return separateur;
}

std::string LecteurCheminAcces::getSeparateurCheminStandard() const
{
	std::string separateur("/");
	return separateur;
}


