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

#include "FamilleVariablesFactory.hpp"

namespace sambada {
	FamilleVariables FamilleVariablesFactory::genereFamille(int nombreVariables, int nombreGenerations, const MatriceBools& masque)
	{
		FamilleVariables famille(0);

		if (areDonneesValides(nombreVariables, nombreGenerations, masque))
		{
			int nombreGenerationsEffectif = nombreGenerations <= nombreVariables ? nombreGenerations : nombreVariables;

			famille.push_back(creeGenerationNulle());
			famille.push_back(creePremiereGeneration(nombreVariables, masque, famille[0]));

			for (size_t i(2); i <= nombreGenerationsEffectif; ++i)
			{
				famille.push_back(creeGeneration(nombreVariables, masque, famille[i-1]));
			}
		}

		return famille;
	}

	bool FamilleVariablesFactory::areDonneesValides(int nombreVariables, int nombreGenerations, const MatriceBools& masque)
	{
		return (nombreVariables > 0 && nombreGenerations > 0 && nombreVariables == masque.cols());
	}

	GenerationVariables FamilleVariablesFactory::creeGenerationNulle()
	{
		EtiquetteCombinaisonVariables etiquetteNulle;
		CombinaisonVariables combinaisonNulle;

		GenerationVariables generationNulle;
		generationNulle[etiquetteNulle] = combinaisonNulle;
		return generationNulle;
	}

	GenerationVariables FamilleVariablesFactory::creePremiereGeneration(int nombreVariables, const sambada::MatriceBools& masque, const sambada::GenerationVariables& generationNulle)
	{
		GenerationVariables generation;

		const CombinaisonVariables& combinaisonNulle(generationNulle.cbegin()->second);

		for(int i(0); i < nombreVariables; ++i)
		{
			EtiquetteCombinaisonVariables etiquette;
			etiquette.insert(i);
			generation[etiquette] = creeCombinaisonVariables(combinaisonNulle, i, masque);
		}

		return generation;
	}

	GenerationVariables FamilleVariablesFactory::creeGeneration(int nombreVariables, const sambada::MatriceBools& masque, const sambada::GenerationVariables& generationPrecedente)
	{
		GenerationVariables generation;

		for(GenerationVariables::const_iterator iter(generationPrecedente.cbegin()); iter != generationPrecedente.cend(); ++iter)
		{
			int derniereVariableGenerationPrecedente(*(iter->second.variables.crbegin()));

			for(int variableCourante(derniereVariableGenerationPrecedente+1); variableCourante < nombreVariables; ++variableCourante)
			{
				CombinaisonVariables combinaison = creeCombinaisonVariables(iter->second, variableCourante, masque);
				generation[combinaison.variables] = combinaison;
			}
		}

		return generation;
	}

	CombinaisonVariables FamilleVariablesFactory::creeCombinaisonVariables(const CombinaisonVariables& combinaisonExistante, int nouvelleVariable, const MatriceBools& masque)
	{
		CombinaisonVariables combinaison;

		combinaison.variables = combinaisonExistante.variables;
		combinaison.variables.insert(nouvelleVariable);

		if (combinaisonExistante.variables.size() == 0)
		{
			combinaison.parents.insert(combinaisonExistante.variables);
			combinaison.masque = masque(scythe::_, nouvelleVariable);
		}
		else
		{
			for (int variable : combinaison.variables)
			{
				EtiquetteCombinaisonVariables etiquette = combinaison.variables;
				etiquette.erase(variable);
				combinaison.parents.insert(etiquette);
			}

			combinaison.masque = combinaisonExistante.masque % masque(scythe::_, nouvelleVariable);
		}

		return combinaison;
	}
}