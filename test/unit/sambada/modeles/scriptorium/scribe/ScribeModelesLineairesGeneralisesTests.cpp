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

#include <variables/SpecificationsVariables.hpp>
#include "catch.hpp"
#include "modeles/scriptorium/scribe/ScribeModelesLineairesGeneralises.hpp"
#include "modeles/scriptorium/scribe/FlotSortieChaineFactory.hpp"
#include "common/TypesCommuns.hpp"
#include "modeles/scriptorium/lecteur/Lecteur.hpp"
#include "modeles/scriptorium/lecteur/FlotEntreeChaineFactory.hpp"
#include <string>

TEST_CASE("Test that ScribeModelesLineairesGeneralises can write models in several output streams", "[scribe-mlg-unit]")
{
	//ARS-BFGL-NGS-106520_AA -497.564738070906 0.309701492537313 -0.801515217280957 0
	//Hapmap28985-BTA-73836_GG bio3 -381.270794045966 160.936242443686 111.211799305358 0 0.212761323380211 0.174271906831096 0.16994045471851 0.181407973822525 0.265635379062047 766.541588091933 775.920786630291 19.9820961881851 -0.263008876258493
	//ARS-BFGL-NGS-106520_AA longitude pop1 -384.857852389517 59.0316812458271 53.0013412636071 0 0.255065924842735 0.0712300059599299 0.0639901641200908 0.070791833249324 0.110049693905941 775.715704779034 789.784502586571 21.1050478081843 -0.690299103228441 54.6518602528727
	//Hapmap28985-BTA-73836_GG latitude tmax10 pop1 -353.343344889023 20.404103668053 20.6665668031513 0 0.240848498029564 0.0280626626715301 0.0170599102338552 0.0250589178683572 0.0421022600403521 714.686689778045 733.445086854761 7.53921009347497 0.632349795811089 -0.0351020724390216 34.5966166921478

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

	char delimMots(',');
	std::string retourLigne("\n");
	int precision(15);

	sambada::SpecificationsVariables specMarq;
	specMarq.variablesActives = {{0, 0},
	                             {1, 1}};
	specMarq.detailsVariables = {{0, "ARS-BFGL-NGS-106520_AA",   true, true, 0},
	                             {1, "Hapmap28985-BTA-73836_GG", true, true, 1}};

	sambada::SpecificationsVariables specEnv;
	specEnv.detailsVariables = {{0, "longitude", true, true,  0},
	                            {1, "latitude",  true, true,  1},
	                            {2, "bio3",      true, true,  2},
	                            {3, "tmin5",     true, false, 0},
	                            {4, "tmax10",    true, true,  3},
	                            {5, "pop1",      true, true,  4}};
	specEnv.variablesActives = {{0, 0},
	                            {1, 1},
	                            {2, 2},
	                            {3, 4},
	                            {4, 5}};
	specMarq.variablesPassives = {{0, 3}};


	SECTION("Test that Scribe creates the correct number of streams")
	{
		scribeGLM.initialise(specEnv, specMarq, nomBase, dimensionMax, retourLigne, delimMots, precision);

		CHECK(factory.getFlotsSortie().size() == dimensionMax + 1);
	}

	SECTION("Test that Scribe creates streams with the correct names")
	{
		scribeGLM.initialise(specEnv, specMarq, nomBase, dimensionMax, retourLigne, delimMots, precision);

		CHECK(factory.getNomsFlots() == nomFichiersAttendus);
	}

	SECTION("Test that stream precisions are correct")
	{
		scribeGLM.initialise(specEnv, specMarq, nomBase, dimensionMax, retourLigne, delimMots, precision);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());
		for (auto flot(flots.begin()); flot != flots.end(); ++flot)
		{
			std::ostream& flotInterieur(*flot->get());
			CHECK(flotInterieur.precision() == precision);
		}
	}

	SECTION("Test that streams are empty before the first ecriture")
	{
		scribeGLM.initialise(specEnv, specMarq, nomBase, dimensionMax, retourLigne, delimMots, precision);

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
		scribeGLM.initialise(specEnv, specMarq, nomBase, dimensionMax, retourLigne, ',', precision);

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
		scribeGLM.initialise(specEnv, specMarq, nomBase, dimensionMax, retourLigne, ',', precision);

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

	SECTION("Test that a model of dimension 0 is written correctly")
	{
//	//ARS-BFGL-NGS-106520_AA -497.564738070906 0.309701492537313 -0.801515217280957 0
		scribeGLM.initialise(specEnv, specMarq, nomBase, dimensionMax, retourLigne, delimMots, precision);

		std::string expectedMarker("ARS-BFGL-NGS-106520_AA");
		std::vector<sambada::reel> expectedValues({-497.564738070906, 0.309701492537313, -0.801515217280957, 0});

		sambada::EtiquetteModele etiquetteModele;
		etiquetteModele.marqueur = 0;
		etiquetteModele.environnement = {};
		sambada::Modele modele({etiquetteModele, expectedValues});

		scribeGLM.ecrisModele(modele);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());
		std::vector<std::string> contenusFlots;
		for (auto i(flots.begin()); i != flots.end(); ++i)
		{
			contenusFlots.push_back(static_cast<std::ostringstream *>(i->get())->str());
		}

		sambada::FlotEntreeChaineFactory entreeChaineFactory;
		entreeChaineFactory.setContenusFlotsEntree(contenusFlots);
		sambada::Lecteur lecteur(entreeChaineFactory);
		lecteur.initialise(nomFichiersAttendus, retourLigne, delimMots, precision);

		std::vector<std::vector<std::string>> messagesLus(dimensionMax + 1);

		for (int i(0); i <= dimensionMax; ++i)
		{
			lecteur.lecture(i, messagesLus[i]);
		}

		CHECK(messagesLus[1].size() == 0);
		CHECK(messagesLus[2].size() == 0);
		CHECK(messagesLus[3].size() == 0);

		CHECKED_IF(messagesLus[0].size() == 5)
		{
			CHECK(messagesLus[0][0] == expectedMarker);
			for (int i(0); i < 4; ++i)
			{
				CHECK(std::stold(messagesLus[0][i + 1]) == Approx(expectedValues[i]));
			}
		}

	}

}
