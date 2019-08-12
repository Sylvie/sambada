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

#ifndef SAMBADA_SCRIBE_HPP
#define SAMBADA_SCRIBE_HPP

#include "Archiviste.hpp"
#include "FlotSortieFactory.hpp"

namespace sambada {
	class Scribe : public Archiviste
	{
	public:
		Scribe(FlotSortieFactory& factory);

		Scribe(const Scribe&) = delete;

		virtual ~Scribe() = default;

		virtual void initialise(const std::vector<std::string>& noms, const std::string& signeRetourLigne, char delimMots, int precision);

		template<typename T>
		bool ecriture(int numFichier, T element, bool retourLigne = false) const;

	protected:
		FlotSortieFactory& factory;

		std::vector<FlotSortie> flots;
	};

	template<typename T>
	bool Scribe::ecriture(int numFichier, T element, bool retourLigne) const
	{
		if (numFichier < 0 || flots.size() <= numFichier)
		{
			return false;
		}
		else
		{

			*(flots[numFichier]) << element << delimMots;

			if (retourLigne)
			{
				*(flots[numFichier]) << signeRetourLigne;
			}
			return true;
		}
	}

}

#endif //SAMBADA_SCRIBE_HPP
