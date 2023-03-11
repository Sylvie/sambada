/*************************************************************************
* Copyright (©) 2011-2023 EPFL (Ecole Polytechnique fédérale de Lausanne)
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
*************************************************************************/

#include "Lecteur.hpp"

namespace sambada {
	Lecteur::Lecteur(FlotEntreeFactory& factory)
			: factory(factory)
	{

	}

	void Lecteur::initialise(const std::vector<std::string>& nomsFichiers, const std::string& chaineRetourLigne, char charDelimMots, int precisionFlots)
	{
		Archiviste::initialise(nomsFichiers, chaineRetourLigne, charDelimMots, precisionFlots);

		flots.clear();
		for (size_t i(0); i < nomsFichiers.size(); ++i)
		{
			flots.push_back(factory.creeFlotEntree(nomsFichiers[i]));
			flots[i]->precision(precisionFlots);
		}
	}

	bool Lecteur::lecture(int numFichier, std::vector<std::string>& ligne) const
	{
		if (numFichier < 0 || flots.size() <= (size_t) numFichier)
		{
			return false;
		}
		else
		{
			ligne.clear();
			tokenizer.lectureLigne(*flots[numFichier], ligne, getDelimMots());
			return true;
		}
	}

	bool Lecteur::lectureGroupe(int numFichier, std::vector<std::string>& groupe, int nombre) const
	{
		if (numFichier < 0 || flots.size() <= (size_t) numFichier)
		{
			return false;
		}
		else
		{
			groupe.clear();
			std::string mot("");
			for (int i(0); (!flots[numFichier]->eof()) && i < nombre; ++i)
			{
				tokenizer.lectureMot(*flots[numFichier], mot, getDelimMots());
				groupe.push_back(mot);
			}
			return groupe.size() == nombre;
		}
	}

	bool Lecteur::finFichier(int numFichier) const
	{
		if (numFichier < 0 || flots.size() <= (size_t) numFichier)
		{
			return true;
		}
		else
		{
			flots[numFichier]->peek();
			return flots[numFichier]->eof();
		}
	}
}
