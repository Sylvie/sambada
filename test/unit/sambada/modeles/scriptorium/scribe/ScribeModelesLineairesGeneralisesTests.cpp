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

#include "catch.hpp"
#include "modeles/scriptorium/scribe/ScribeModelesLineairesGeneralises.hpp"
#include "modeles/scriptorium/scribe/FlotSortieChaineFactory.hpp"

TEST_CASE("Test that ScribeModelesLineairesGeneralises can write models in several output streams", "[scribe-mlg-unit]")
{

	int dimensionMax(3);
	std::pair<std::string, std::string> nomBase({"nomFichier", ".txt"});

	std::vector<std::string> nomFichiersAttendus({
			                                             "nomFichier-Out-0.txt",
			                                             "nomFichier-Out-1.txt",
			                                             "nomFichier-Out-2.txt",
			                                             "nomFichier-Out-3.txt"
	                                             });

	sambada::FlotSortieChaineFactory factory;
	sambada::Scribe scribe(factory);

	sambada::ScribeModelesLineairesGeneralises scribeGLM(scribe);

	char delimMots(' ');
	std::string retourLigne("\n");
	int precision(15);

	SECTION("Test that Scribe creates the correct number of streams")
	{
		scribeGLM.initialise(nomBase, dimensionMax, retourLigne, delimMots, precision);

		CHECK(factory.getFlotsSortie().size() == dimensionMax + 1);
	}

	SECTION("Test that Scribe creates streams with the correct names")
	{
		scribeGLM.initialise(nomBase, dimensionMax, retourLigne, delimMots, precision);

		CHECK(factory.getNomsFlots() == nomFichiersAttendus);
	}

	SECTION("Test that stream precisions are correct")
	{
		scribeGLM.initialise(nomBase, dimensionMax, retourLigne, delimMots, precision);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());
		for (auto flot(flots.begin()); flot != flots.end(); ++flot)
		{
			std::ostream& flotInterieur(*flot->get());
			CHECK(flotInterieur.precision() == precision);
		}
	}

	SECTION("Test that streams are empty before the first ecriture")
	{
		scribeGLM.initialise(nomBase, dimensionMax, retourLigne, delimMots, precision);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());
		CHECKED_IF(flots.size() == dimensionMax + 1)
		{
			for (int i(0); i <= dimensionMax; ++i)
			{
				std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

				CHECK(stream->str() == "");
			}
		}
	}


	SECTION("Test that the file headers are correct without computation of population structure")
	{
		scribeGLM.initialise(nomBase, dimensionMax, retourLigne, ',', precision);

		std::vector<std::string> expectedMessages({
				                                          "Marker,Loglikelihood,AverageProb,Beta_0,NumError\n",
				                                          "Marker,Env_1,Loglikelihood,Gscore,WaldScore,NumError,Efron,McFadden,McFaddenAdj,CoxSnell,Nagelkerke,AIC,BIC,Beta_0,Beta_1\n",
				                                          "Marker,Env_1,Env_2,Loglikelihood,Gscore,WaldScore,NumError,Efron,McFadden,McFaddenAdj,CoxSnell,Nagelkerke,AIC,BIC,Beta_0,Beta_1,Beta_2\n",
				                                          "Marker,Env_1,Env_2,Env_3,Loglikelihood,Gscore,WaldScore,NumError,Efron,McFadden,McFaddenAdj,CoxSnell,Nagelkerke,AIC,BIC,Beta_0,Beta_1,Beta_2,Beta_3\n"
		                                          });

		scribeGLM.ecrisEnTetes(false);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

		CHECKED_IF(flots.size() == dimensionMax + 1)
		{
			for (int i(0); i <= dimensionMax; ++i)
			{
				std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

				INFO("Dimension: " + std::to_string(i))
				CHECK(stream->str() == expectedMessages[i]);
			}
		}
	}


	SECTION("Test that the file headers are correct with computation of population structure")
	{
		scribeGLM.initialise(nomBase, dimensionMax, retourLigne, ',', precision);

		std::vector<std::string> expectedMessages({
				                                          "Marker,Loglikelihood,AverageProb,Beta_0,NumError\n",
				                                          "Marker,Env_1,Loglikelihood,Gscore,WaldScore,NumError,Efron,McFadden,McFaddenAdj,CoxSnell,Nagelkerke,AIC,BIC,Beta_0,Beta_1\n",
				                                          "Marker,Env_1,Env_2,Loglikelihood,Gscore,WaldScore,NumError,Efron,McFadden,McFaddenAdj,CoxSnell,Nagelkerke,AIC,BIC,Beta_0,Beta_1,Beta_2\n",
				                                          "Marker,Env_1,Env_2,Env_3,Loglikelihood,Gscore,WaldScore,NumError,Efron,McFadden,McFaddenAdj,CoxSnell,Nagelkerke,AIC,BIC,GscorePop,WaldScorePop,Beta_0,Beta_1,Beta_2,Beta_3\n"
		                                          });

		scribeGLM.ecrisEnTetes(true);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

		CHECKED_IF(flots.size() == dimensionMax + 1)
		{
			for (int i(0); i <= dimensionMax; ++i)
			{
				std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

				INFO("Dimension: " + std::to_string(i))
				CHECK(stream->str() == expectedMessages[i]);
			}
		}
	}
	
}
