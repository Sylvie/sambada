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
#include "modeles/scriptorium/lecteur/Lecteur.hpp"
#include "modeles/scriptorium/lecteur/FlotEntreeChaineFactory.hpp"

TEST_CASE("Test that Lecteur can read from several output streams", "[lecteur-unit]")
{

	int nombreFlots(3);
	std::vector<std::string> nomsFlots({"premierFlot", "deuxièmeFlot", "troisièmeFlot"});

	sambada::FlotEntreeChaineFactory factory;

	sambada::Lecteur lecteur(factory);

	char delimMots(' ');
	std::string retourLigne("\n");
	int precision(15);

	SECTION("Test that Lecteur creates the correct number of streams")
	{
		lecteur.initialise(nomsFlots, retourLigne, delimMots, precision);

		CHECK(factory.getFlotsEntree().size() == (size_t) nombreFlots);
	}

	SECTION("Test that Lecteur creates streams with the correct names")
	{
		lecteur.initialise(nomsFlots, retourLigne, delimMots, precision);

		CHECK(factory.getNomsFlots() == nomsFlots);
	}

	SECTION("Test that stream precisions are correct")
	{
		lecteur.initialise(nomsFlots, retourLigne, delimMots, precision);

		std::vector<sambada::FlotEntree> flots(factory.getFlotsEntree());
		CHECKED_IF(flots.size() == nombreFlots)
		{
			for (auto flot(flots.begin()); flot != flots.end(); ++flot)
			{
				std::istream& flotInterieur(*flot->get());
				CHECK(flotInterieur.precision() == precision);
			}
		}
	}

	SECTION("Test that streams indicates end-of-file when they are empty")
	{
		factory.setContenusFlotsEntree({"", "", ""});

		lecteur.initialise(nomsFlots, retourLigne, delimMots, precision);

		std::vector<sambada::FlotEntree> flots(factory.getFlotsEntree());
		CHECKED_IF(flots.size() == nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				flots[i]->peek();
				CHECK(flots[i].get()->eof());
			}
		}
	}

	SECTION("Test that lecture of strings reads from the correct file")
	{
		factory.setContenusFlotsEntree({
				                               "Second message in first file",
				                               "First message in second file",
				                               "Third message in third file"
		                               });

		lecteur.initialise(nomsFlots, retourLigne, delimMots, precision);

		std::vector<std::vector<std::string>> expectedMessages({
				                                                       {"First",  "message", "in", "second", "file"},
				                                                       {"Second", "message", "in", "first",  "file"},
				                                                       {"Third",  "message", "in", "third",  "file"}
		                                                       });

		std::vector<std::vector<std::string>> messagesLus(nombreFlots);

		lecteur.lecture(1, messagesLus[0]);
		lecteur.lecture(0, messagesLus[1]);
		lecteur.lecture(2, messagesLus[2]);

		CHECKED_IF(messagesLus.size() == nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				CHECK(messagesLus[i] == expectedMessages[i]);
			}
		}
	}

	SECTION("Test that lecture of strings uses the correct word delimitor")
	{
		factory.setContenusFlotsEntree({
				                               "Second%message%in%first%file%\n",
				                               "First%message%in%second%file%\n",
				                               "Third%message%in%third%file%\n"
		                               });

		lecteur.initialise(nomsFlots, retourLigne, '%', precision);

		std::vector<std::vector<std::string>> expectedMessages({
				                                                       {"First",  "message", "in", "second", "file"},
				                                                       {"Second", "message", "in", "first",  "file"},
				                                                       {"Third",  "message", "in", "third",  "file"}
		                                                       });


		std::vector<std::vector<std::string>> messagesLus(nombreFlots);

		lecteur.lecture(1, messagesLus[0]);
		lecteur.lecture(0, messagesLus[1]);
		lecteur.lecture(2, messagesLus[2]);

		CHECKED_IF(messagesLus.size() == nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				CHECK(messagesLus[i] == expectedMessages[i]);
			}
		}
	}

	SECTION("Test that lecture of strings can read several lines")
	{
		factory.setContenusFlotsEntree({
				                               "First message \nFourth message ",
				                               "Second message \nFifth message ",
				                               "Third message \nSixth message "
		                               });


		std::vector<std::vector<std::string>> expectedMessages({
				                                                       {"First",  "message"},
				                                                       {"Second", "message"},
				                                                       {"Third",  "message"},
				                                                       {"Fourth", "message"},
				                                                       {"Fifth",  "message"},
				                                                       {"Sixth",  "message"}
		                                                       });


		lecteur.initialise(nomsFlots, retourLigne, delimMots, precision);

		std::vector<std::vector<std::string>> messagesLus(2 * nombreFlots);

		lecteur.lecture(0, messagesLus[0]);
		lecteur.lecture(1, messagesLus[1]);
		lecteur.lecture(2, messagesLus[2]);

		lecteur.lecture(0, messagesLus[3]);
		lecteur.lecture(1, messagesLus[4]);
		lecteur.lecture(2, messagesLus[5]);

		CHECKED_IF(messagesLus.size() == 2 * nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				CHECK(messagesLus[i] == expectedMessages[i]);
			}
		}
	}


	SECTION("Test that vector-item lecture reads from the correct file")
	{
		factory.setContenusFlotsEntree({
				                               "Deuxieme message dans le premier fichier",
				                               "First message in second file",
				                               "Dritte Nachricht in der dritten Datei"
		                               });

		lecteur.initialise(nomsFlots, retourLigne, delimMots, precision);

		std::vector<std::vector<std::string>> expectedMessages({
				                                                       {"First",    "message",   "in",   "second", "file"},
				                                                       {"Deuxieme", "message",   "dans", "le"},
				                                                       {"Dritte",   "Nachricht", "in",   "der",    "dritten", "Datei"}
		                                                       });

		std::vector<std::vector<std::string>> messagesLus(nombreFlots);

		lecteur.lectureGroupe(1, messagesLus[0], 5);
		lecteur.lectureGroupe(0, messagesLus[1], 4);
		lecteur.lectureGroupe(2, messagesLus[2], 6);

		CHECKED_IF(messagesLus.size() == nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				CHECK(messagesLus[i] == expectedMessages[i]);
			}
		}
	}

	SECTION("Test that vector-item lecture use the correct word delimitor")
	{
		factory.setContenusFlotsEntree({
				                               "First%message%in%second%file%\n",
				                               "Deuxieme%message%dans%le%premier%fichier%\n",
				                               "Dritte%Nachricht%in%der%dritten%Datei%\n"
		                               });

		lecteur.initialise(nomsFlots, retourLigne, '%', precision);

		std::vector<std::vector<std::string>> expectedMessages({
				                                                       {"First",    "message",   "in",   "second", "file"},
				                                                       {"Deuxieme", "message",   "dans", "le"},
				                                                       {"Dritte",   "Nachricht", "in",   "der",    "dritten", "Datei"}
		                                                       });


		std::vector<std::vector<std::string>> messagesLus(nombreFlots);

		lecteur.lectureGroupe(0, messagesLus[0], 5);
		lecteur.lectureGroupe(1, messagesLus[1], 4);
		lecteur.lectureGroupe(2, messagesLus[2], 6);

		CHECKED_IF(messagesLus.size() == nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				CHECK(messagesLus[i] == expectedMessages[i]);
			}
		}
	}

	SECTION("Test that vector-item lecture can read from several lines")
	{
		factory.setContenusFlotsEntree({
				                               "First message \nFourth message ",
				                               "Deuxieme Message \nCinquieme Message ",
				                               "Dritte Nachricht \nSechste Nachricht "
		                               });


		std::vector<std::vector<std::string>> expectedMessages({
				                                                       {"First"},
				                                                       {"Deuxieme"},
				                                                       {"Dritte"},
				                                                       {"message", "Fourth"},
				                                                       {"Message", "Cinquieme"},
				                                                       {"Naricht", "Sechste"}
		                                                       });


		lecteur.initialise(nomsFlots, retourLigne, delimMots, precision);

		std::vector<std::vector<std::string>> messagesLus(2 * nombreFlots);

		lecteur.lectureGroupe(0, messagesLus[0], 1);
		lecteur.lectureGroupe(1, messagesLus[1], 1);
		lecteur.lectureGroupe(2, messagesLus[2], 1);

		lecteur.lectureGroupe(0, messagesLus[3], 2);
		lecteur.lectureGroupe(1, messagesLus[4], 2);
		lecteur.lectureGroupe(2, messagesLus[5], 2);

		CHECKED_IF(messagesLus.size() == 2 * nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				CHECK(messagesLus[i] == expectedMessages[i]);
			}
		}
	}
}
