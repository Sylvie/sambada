/*************************************************************************
* Copyright (©) 2011-2022 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

#include "Tokenizer.hpp"

namespace sambada
{
	char Tokenizer::lectureMot(std::istream& entree, std::string& mot, char delimMots, bool gardeSignesInvisibles) const
	{
		mot.clear();
		char lu = 0x00;
		bool inner(false), continueLecture(true);
		while (continueLecture && !((entree.get(lu)).eof()))
		{
			if (lu == '"')
			{
				inner = !inner;
			}
				// On ôte les "

			else if (inner == 0 && (lu == delimMots || lu == '\r' || lu == '\n' || (!gardeSignesInvisibles && ((lu < 0x20) || (lu >= 0x7F)))))    // Les espaces sont conservés
			{
				continueLecture = false;
			}
			else if (gardeSignesInvisibles || (lu > 0x19 && lu < 0x7F))    // On est à l'intérieur, les espaces sont conservés
			{
				mot.push_back(lu);
			}
		}
		return lu;
	}

	bool Tokenizer::lectureLigne(std::istream& entree, std::vector<std::string>& ligne, char delimMots, bool gardeSignesInvisibles) const
	{
		ligne.clear();
		std::string mot("");
		char lu = 0x00;
		bool continueLecture(true);
		while (continueLecture && !entree.eof())
		{
			lu = lectureMot(entree, mot, delimMots, gardeSignesInvisibles);
			if (mot.size() > 0)
			{
				ligne.push_back(mot);
			}
			if (lu == '\n' || lu == '\r')
			{
				continueLecture = false;
				while (iscntrl(entree.peek()))
				{
					entree.get(lu);
				}
			}
		}
		return entree.eof();
	}
}
