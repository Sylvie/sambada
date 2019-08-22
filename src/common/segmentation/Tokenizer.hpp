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
*************************************************************************/

#ifndef SAMBADA_TOKENIZER_HPP
#define SAMBADA_TOKENIZER_HPP

#include <istream>

namespace sambada {

	class Tokenizer
	{
	public:

		virtual ~Tokenizer() = default;

		/**
		 * Lit les caractères du flots et forme un mot
 		 * La lecture s'arrête si le caractère séparateur ou un caractère invisible est trouvé
		 * Si un '"' est lu, la lecture continue jusqu'au '"' suivant,
		 * les caractères invisibles sont ignorés, mais les espaces sont conservés
		 * @return le dernier caractère lu
		 */
		char lectureMot(std::istream& entree, std::string& mot, char delimMots = ' ', bool gardeSignesInvisibles = false);


	};

}


#endif //SAMBADA_TOKENIZER_HPP
