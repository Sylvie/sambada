/*************************************************************************
* Copyright (©) 2011-2020 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

#include "ScribeModelesLineairesGeneralises.hpp"

namespace sambada {

	ScribeModelesLineairesGeneralises::ScribeModelesLineairesGeneralises(const Scribe& scribe)
			: dimensionMax(0), scribe(scribe)
	{}

	void ScribeModelesLineairesGeneralises::initialise(const SpecificationsVariables& specVariablesEnvironnementales, const SpecificationsVariables& specMarqueurs, const std::pair<std::string, std::string>& nomFichierBase, int dimensionMaximale, const std::string& chaineRetourLigne, char charDelimMots, int precisionFlots)
	{
		specVarEnv = specVariablesEnvironnementales;
		specMarq = specMarqueurs;

		dimensionMax = dimensionMaximale;

		std::vector<std::string> nomsFichiers;
		for (int i(0); i <= dimensionMax; ++i)
		{
			nomsFichiers.push_back(nomFichierBase.first + "-Out-" + std::to_string(i) + nomFichierBase.second);
		}

		scribe.initialise(nomsFichiers, chaineRetourLigne, charDelimMots, precisionFlots);

	}

	void ScribeModelesLineairesGeneralises::ecrisEnTetes(bool avecStructurePop) const
	{
		// Ecriture des noms de colonnes pour s'y repérer
		std::vector<std::vector<std::string> > names(4);
		names[0].push_back("Marker");
		names[0].push_back("Env_");
		names[0].push_back("Beta_");
		names[1].push_back("Loglikelihood");
		names[1].push_back("AverageProb");
		names[1].push_back("Beta_0");
		names[1].push_back("NumError");
		names[2].push_back("Loglikelihood");
		names[2].push_back("Gscore");
		names[2].push_back("WaldScore");
		names[2].push_back("NumError");
		names[2].push_back("Efron");
		names[2].push_back("McFadden");
		names[2].push_back("McFaddenAdj");
		names[2].push_back("CoxSnell");
		names[2].push_back("Nagelkerke");
		names[2].push_back("AIC");
		names[2].push_back("BIC");

		names[3].push_back("GscorePop");
		names[3].push_back("WaldScorePop");

		scribe.ecriture(0, names[0][0]);
		scribe.ecriture(0, names[1], true);

		for (int i(1); i <= dimensionMax; ++i)
		{
			scribe.ecriture(i, names[0][0]);
			for (int j(1); j <= i; ++j)
			{
				scribe.ecriture(i, names[0][1] + std::to_string(j));
			}
			if (!avecStructurePop || i != dimensionMax)
			{
				scribe.ecriture(i, names[2]);
			}
			else
			{
				scribe.ecriture(i, names[2], false);
				scribe.ecriture(i, names[3]);
			}
			for (int j(0); j < i; ++j)
			{
				scribe.ecriture(i, names[0][2] + std::to_string(j));
			}
			scribe.ecriture(i, names[0][2] + std::to_string(i), true);
		}
	}

	void ScribeModelesLineairesGeneralises::ecrisModele(const Modele& modele) const
	{
		int numFichier(modele.first.environnement.size());

		// No de marqueur
		scribe.ecriture(numFichier, specMarq.detailsVariables[specMarq.variablesActives.at(modele.first.marqueur)].name, false);

		// Liste des variables
		for (std::set<int>::iterator i(modele.first.environnement.begin()); i != modele.first.environnement.end(); ++i)
		{
			scribe.ecriture(numFichier, specVarEnv.detailsVariables[specVarEnv.variablesActives.at(*i)].name, false);
		}
		// Résultats
		scribe.ecriture(numFichier, modele.second, true);
	}


}