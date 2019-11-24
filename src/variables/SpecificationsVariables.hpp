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

#ifndef SAMBADA_SPECIFICATIONSVARIABLES_HPP
#define SAMBADA_SPECIFICATIONSVARIABLES_HPP

#include <string>
#include <vector>
#include <map>

namespace sambada {

	/**
	 *  Définition des spécificités d'une variable
	 */
	typedef struct
	{
		int number;
		std::string name;
		bool isNumeric;
		bool isActive;
		int localIndex;
	}
			DetailsVariable;

	typedef std::vector<DetailsVariable> DetailsVariables;

	// Lien entre l'indice local et le numéro global de la variable.
	typedef std::map<int, int> IndexDetailsVariables;

	class SpecificationsVariables
	{
	public:
		DetailsVariables detailsVariables;

		IndexDetailsVariables variablesActives;
		IndexDetailsVariables variablesPassives;
	};

}

#endif //SAMBADA_SPECIFICATIONSVARIABLES_HPP
