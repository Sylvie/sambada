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

#ifndef SAMBADA_SCRIBEMODELESLINEAIRESGENERALISES_HPP
#define SAMBADA_SCRIBEMODELESLINEAIRESGENERALISES_HPP

#include "modeles/scriptorium/scribe/Scribe.hpp"
#include "modeles/Modele.hpp"
#include "variables/SpecificationsVariables.hpp"

namespace sambada {

	class ScribeModelesLineairesGeneralises
	{
	public:

		explicit ScribeModelesLineairesGeneralises(const Scribe& scribe);

		ScribeModelesLineairesGeneralises(ScribeModelesLineairesGeneralises& scribeMLG) = delete;

		virtual ~ScribeModelesLineairesGeneralises() = default;

		virtual void initialise(const SpecificationsVariables& specVarEnv, const SpecificationsVariables& specMarq, const std::pair<std::string, std::string>& nomFichierBase, int dimensionMax, const std::string& chaineRetourLigne, char charDelimMots, int precisionFlots);

		virtual void ecrisEnTetes(bool avecStructurePop) const;

		virtual void ecrisModele(const Modele& modele) const;

	private:

		int dimensionMax;
		Scribe scribe;

		SpecificationsVariables specVarEnv;
		SpecificationsVariables specMarq;
	};
}

#endif //SAMBADA_SCRIBEMODELESLINEAIRESGENERALISES_HPP
