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

	/*
	SECTION("Test that single-item ecriture writes in the correct file")
	{
		scribe.initialise(nomsFlots, retourLigne, delimMots, precision);

		std::vector<std::string> messages({
				                                  "First message in second file",
				                                  "Second message in first file",
				                                  "Third message in third file"
		                                  });

		std::vector<std::string> expectedMessages({
				                                          "Second message in first file ",
				                                          "First message in second file ",
				                                          "Third message in third file "
		                                          });

		scribe.ecriture(1, messages[0]);
		scribe.ecriture(0, messages[1]);
		scribe.ecriture(2, messages[2]);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

		CHECKED_IF(flots.size() == dimensionMax)
		{
			for (int i(0); i < dimensionMax; ++i)
			{
				std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

				CHECK(stream->str() == expectedMessages[i]);
			}
		}
	}

	SECTION("Test that single-item ecriture use the correct word delimitor")
	{
		scribe.initialise(nomsFlots, retourLigne, '%', precision);


		std::vector<std::string> expectedMessages({
				                                          "First%message\n",
				                                          "Second%message\n",
				                                          "Third%message\n"
		                                          });

		scribe.ecriture(0, "First", false);
		scribe.ecriture(0, "message", true);

		scribe.ecriture(1, "Second", false);
		scribe.ecriture(1, "message", true);

		scribe.ecriture(2, "Third", false);
		scribe.ecriture(2, "message", true);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

		CHECKED_IF(flots.size() == dimensionMax)
		{
			for (int i(0); i < dimensionMax; ++i)
			{
				std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

				CHECK(stream->str() == expectedMessages[i]);
			}
		}
	}

	SECTION("Test that single-item ecriture use the correct line delimitor")
	{
		scribe.initialise(nomsFlots, "&&", delimMots, precision);


		std::vector<std::string> expectedMessages({
				                                          "First message&&Fourth message ",
				                                          "Second message&&Fifth message ",
				                                          "Third message&&Sixth message "
		                                          });

		scribe.ecriture(0, "First message", true);
		scribe.ecriture(1, "Second message", true);
		scribe.ecriture(2, "Third message", true);

		scribe.ecriture(0, "Fourth message", false);
		scribe.ecriture(1, "Fifth message", false);
		scribe.ecriture(2, "Sixth message", false);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

		CHECKED_IF(flots.size() == dimensionMax)
		{
			for (int i(0); i < dimensionMax; ++i)
			{
				std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

				CHECK(stream->str() == expectedMessages[i]);
			}
		}
	}


	SECTION("Test that vector-item ecriture writes in the correct file")
	{
		scribe.initialise(nomsFlots, retourLigne, delimMots, precision);

		std::vector<std::vector<std::string>> messages({
				                                               {"First",  "message", "in", "second", "file"},
				                                               {"Second", "message", "in", "first",  "file"},
				                                               {"Third",  "message", "in", "third",  "file"}
		                                               });

		std::vector<std::string> expectedMessages({
				                                          "Second message in first file ",
				                                          "First message in second file ",
				                                          "Third message in third file "
		                                          });

		scribe.ecriture(1, messages[0]);
		scribe.ecriture(0, messages[1]);
		scribe.ecriture(2, messages[2]);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

		CHECKED_IF(flots.size() == dimensionMax)
		{
			for (int i(0); i < dimensionMax; ++i)
			{
				std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

				CHECK(stream->str() == expectedMessages[i]);
			}
		}
	}

	SECTION("Test that vector-item ecriture use the correct word delimitor")
	{
		scribe.initialise(nomsFlots, retourLigne, '%', precision);


		std::vector<std::vector<std::string>> messages({
				                                               {"First",  "message", "in", "second", "file"},
				                                               {"Second", "message", "in", "first",  "file"},
				                                               {"Third",  "message", "in", "third",  "file"}
		                                               });

		std::vector<std::string> expectedMessages({
				                                          "Second%message%in%first%file%",
				                                          "First%message%in%second%file%",
				                                          "Third%message%in%third%file%"
		                                          });


		scribe.ecriture(1, messages[0]);
		scribe.ecriture(0, messages[1]);
		scribe.ecriture(2, messages[2]);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

		CHECKED_IF(flots.size() == dimensionMax)
		{
			for (int i(0); i < dimensionMax; ++i)
			{
				std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

				CHECK(stream->str() == expectedMessages[i]);
			}
		}
	}

	SECTION("Test that vector-item ecriture use the correct line delimitor")
	{
		scribe.initialise(nomsFlots, "&&", delimMots, precision);


		std::vector<std::string> expectedMessages({
				                                          "First message&&Fourth message ",
				                                          "Second message&&Fifth message ",
				                                          "Third message&&Sixth message "
		                                          });

		std::vector<std::vector<std::string>> messages({
				                                               {"First",  "message"},
				                                               {"Second", "message"},
				                                               {"Third",  "message"},
				                                               {"Fourth", "message"},
				                                               {"Fifth",  "message"},
				                                               {"Sixth",  "message"}
		                                               });

		scribe.ecriture(0, messages[0], true);
		scribe.ecriture(1, messages[1], true);
		scribe.ecriture(2, messages[2], true);

		scribe.ecriture(0, messages[3], false);
		scribe.ecriture(1, messages[4], false);
		scribe.ecriture(2, messages[5], false);

		std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

		CHECKED_IF(flots.size() == dimensionMax)
		{
			for (int i(0); i < dimensionMax; ++i)
			{
				std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

				CHECK(stream->str() == expectedMessages[i]);
			}
		}
	}

	SECTION("Test that Scribe can reset its streams")
	{
		int secondNombreFlots(2);
		std::vector<std::string> secondsNomsFlots({"quatrièmeFlot", "cinquièmeFlot"});

		char secondDelimMots('-');
		std::string secondRetourLigne("&&");
		int secondePrecision(22);

		SECTION("Test that Scribe creates the correct number of streams after reset")
		{
			scribe.initialise(nomsFlots, retourLigne, delimMots, precision);
			factory.reset();
			scribe.initialise(secondsNomsFlots, secondRetourLigne, secondDelimMots, secondePrecision);

			CHECK(factory.getFlotsSortie().size() == secondNombreFlots);
		}

		SECTION("Test that Scribe creates streams with the correct names after reset")
		{
			scribe.initialise(nomsFlots, retourLigne, delimMots, precision);
			factory.reset();
			scribe.initialise(secondsNomsFlots, secondRetourLigne, secondDelimMots, secondePrecision);

			CHECK(factory.getNomsFlots() == secondsNomsFlots);
		}

		SECTION("Test that stream precisions are correct after reset")
		{
			scribe.initialise(nomsFlots, retourLigne, delimMots, precision);
			factory.reset();
			scribe.initialise(secondsNomsFlots, secondRetourLigne, secondDelimMots, secondePrecision);

			std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());
			for (auto flot(flots.begin()); flot != flots.end(); ++flot)
			{
				std::ostream& flotInterieur(*flot->get());
				CHECK(flotInterieur.precision() == secondePrecision);
			}
		}

		SECTION("Test that streams are empty before the first ecriture after reset")
		{
			scribe.initialise(nomsFlots, retourLigne, delimMots, precision);
			factory.reset();
			scribe.initialise(secondsNomsFlots, secondRetourLigne, secondDelimMots, secondePrecision);

			std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());
			CHECKED_IF(flots.size() == secondNombreFlots)
			{
				for (int i(0); i < secondNombreFlots; ++i)
				{
					std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

					CHECK(stream->str() == "");
				}
			}
		}

		SECTION("Test that single-item ecriture writes in the correct file after reset")
		{
			scribe.initialise(nomsFlots, retourLigne, delimMots, precision);

			std::vector<std::string> messages({
					                                  "First message in first file",
					                                  "Second message in second file",
					                                  "Third message in third file",
					                                  "Fourth message in new first file",
					                                  "Fifth message in new second file"
			                                  });

			std::vector<std::string> expectedMessages({
					                                          "Fourth message in new first file-",
					                                          "Fifth message in new second file-"
			                                          });

			scribe.ecriture(0, messages[0]);
			scribe.ecriture(1, messages[1]);
			scribe.ecriture(2, messages[2]);

			factory.reset();
			scribe.initialise(secondsNomsFlots, secondRetourLigne, secondDelimMots, secondePrecision);
			scribe.ecriture(0, messages[3]);
			scribe.ecriture(1, messages[4]);

			std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

			CHECKED_IF(flots.size() == secondNombreFlots)
			{
				for (int i(0); i < secondNombreFlots; ++i)
				{
					std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

					CHECK(stream->str() == expectedMessages[i]);
				}
			}
		}

		SECTION("Test that single-item ecriture use the correct word delimitor after reset")
		{
			scribe.initialise(nomsFlots, retourLigne, delimMots, precision);


			std::vector<std::string> expectedMessages({
					                                          "First-message-",
					                                          "Second-message-"
			                                          });

			scribe.ecriture(0, "Some", false);
			scribe.ecriture(0, "thing", true);

			scribe.ecriture(1, "Other", false);
			scribe.ecriture(1, "stuff", true);

			scribe.ecriture(2, "Another", false);
			scribe.ecriture(2, "story", true);

			factory.reset();
			scribe.initialise(secondsNomsFlots, secondRetourLigne, secondDelimMots, secondePrecision);

			scribe.ecriture(0, "First", false);
			scribe.ecriture(0, "message", false);

			scribe.ecriture(1, "Second", false);
			scribe.ecriture(1, "message", false);

			std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

			CHECKED_IF(flots.size() == secondNombreFlots)
			{
				for (int i(0); i < secondNombreFlots; ++i)
				{
					std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

					CHECK(stream->str() == expectedMessages[i]);
				}
			}
		}

		SECTION("Test that single-item ecriture use the correct line delimitor after reset")
		{
			scribe.initialise(nomsFlots, retourLigne, delimMots, precision);

			scribe.ecriture(0, "Something", true);
			scribe.ecriture(1, "Other stuff", true);
			scribe.ecriture(2, "Blah", true);

			std::vector<std::string> expectedMessages({
					                                          "First message&&Third message&&Fifth message-",
					                                          "Second message&&Fourth message-Sixth message&&"
			                                          });
			factory.reset();
			scribe.initialise(secondsNomsFlots, secondRetourLigne, secondDelimMots, secondePrecision);

			scribe.ecriture(0, "First message", true);
			scribe.ecriture(1, "Second message", true);

			scribe.ecriture(0, "Third message", true);
			scribe.ecriture(1, "Fourth message", false);

			scribe.ecriture(0, "Fifth message", false);
			scribe.ecriture(1, "Sixth message", true);

			std::vector<sambada::FlotSortie> flots(factory.getFlotsSortie());

			CHECKED_IF(flots.size() == secondNombreFlots)
			{
				for (int i(0); i < secondNombreFlots; ++i)
				{
					std::ostringstream *stream = static_cast<std::ostringstream *>(flots[i].get());

					CHECK(stream->str() == expectedMessages[i]);
				}
			}
		}
	}*/
}
