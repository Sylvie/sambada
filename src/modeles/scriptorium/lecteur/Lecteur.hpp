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

#ifndef SAMBADA_LECTEUR_HPP
#define SAMBADA_LECTEUR_HPP

#include "modeles/scriptorium/Archiviste.hpp"
#include "modeles/scriptorium/lecteur/FlotEntreeFactory.hpp"

namespace sambada {

	class Lecteur : public Archiviste
	{
	public:
		Lecteur(FlotEntreeFactory& factory);

		virtual ~Lecteur() = default;

		virtual void initialise(const std::vector<std::string>& nomsFichiers, const std::string& chaineRetourLigne, char charDelimMots, int precisionFlots);

	private:
		FlotEntreeFactory& factory;

		std::vector<FlotEntree> flots;

	};

}


#endif //SAMBADA_LECTEUR_HPP
