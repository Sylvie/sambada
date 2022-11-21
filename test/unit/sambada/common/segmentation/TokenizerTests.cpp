/*************************************************************************
* Copyright (©) 2011-2022 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

#include "common/segmentation/Tokenizer.hpp"
#include "catch.hpp"

#include <limits>
#include <sstream>

TEST_CASE("Test that the Tokenizer reads files correctly", "[tokenizer-unit]")
{
	sambada::Tokenizer tokenizer;

	char delimMots(' ');
	bool gardeSignesInvisibles(false);

	SECTION("Test that lectureMot process input stream correctly")
	{
		std::string mot;
		std::istringstream iss;

		SECTION("when there is a single word without invisible chars")
		{
			iss.str("Motus!");

			char lu = tokenizer.lectureMot(iss, mot, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(lu == '!');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "Motus!");
			}
		}

		SECTION("when there are several words without invisible chars")
		{
			iss.str("un deux trois");

			char lu = tokenizer.lectureMot(iss, mot, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(lu == ' ');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "un");
			}
		}

		SECTION("when the text is quoted")
		{
			iss.str("\"texte entre guillemets\"");

			char lu = tokenizer.lectureMot(iss, mot, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(lu == '"');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "texte entre guillemets");
			}
		}

		SECTION("when there is a line feed")
		{
			iss.str("un\n deux");

			char lu = tokenizer.lectureMot(iss, mot, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(lu == '\n');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "un");
			}

		}

		SECTION("when there is a carriage return")
		{
			iss.str("un\r deux");

			char lu = tokenizer.lectureMot(iss, mot, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(lu == '\r');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "un");
			}

		}

		SECTION("when there is a carriage return + line feed")
		{
			iss.str("un\r\n deux");

			char lu = tokenizer.lectureMot(iss, mot, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(lu == '\r');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "un");
			}

		}

		SECTION("when there is a quote and a line feed")
		{
			iss.str("\"un\n deux");

			char lu = tokenizer.lectureMot(iss, mot, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(lu == 'x');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "un deux");
			}

		}

		SECTION("when there is a quote, a line feed and gardeSignesInvisibles is true")
		{
			iss.str("\"un\n deux");

			char lu = tokenizer.lectureMot(iss, mot, delimMots, true);

			SECTION("... check last read character")
			{
				CHECK(lu == 'x');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "un\n deux");
			}

		}

		SECTION("when there are several words with a custom delimitor")
		{
			iss.str("un%deux%trois");

			char lu = tokenizer.lectureMot(iss, mot, '%', gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(lu == '%');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "un");
			}
		}

		SECTION("when there are several words with a custom delimitor and gardeSignesInvisibles is true")
		{
			iss.str("u\tn%deux%trois");

			char lu = tokenizer.lectureMot(iss, mot, '%', true);

			SECTION("... check last read character")
			{
				CHECK(lu == '%');
			}
			SECTION("... check read word")
			{
				CHECK(mot == "u\tn");
			}
		}
	}

	SECTION("Test that lectureLigne process input stream correctly")
	{
		std::vector<std::string> ligne;
		std::istringstream iss;
		std::vector<std::string> ligneAttendue;

		SECTION("when there is a single word without invisible chars")
		{
			iss.str("Motus!");
			ligneAttendue = {"Motus!"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}
		}

		SECTION("when there are several words without invisible chars")
		{
			iss.str("un deux trois");
			ligneAttendue = {"un", "deux", "trois"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}
		}

		SECTION("when the text is quoted")
		{
			iss.str("\"texte entre guillemets\"");
			ligneAttendue = {"texte entre guillemets"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}
		}

		SECTION("when there is a line feed")
		{
			iss.str("un deux \n trois");
			ligneAttendue = {"un", "deux"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK_FALSE(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}

		}

		SECTION("when there is a carriage return")
		{
			iss.str("un deux\r trois");
			ligneAttendue = {"un", "deux"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK_FALSE(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}

		}

		SECTION("when there is a carriage return + line feed")
		{
			iss.str("un deux\r\ntrois");
			ligneAttendue = {"un", "deux"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK_FALSE(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}

		}

		SECTION("when there is a quote and a line feed")
		{
			iss.str("\"un\n deux");
			ligneAttendue = {"un deux"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, delimMots, gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}

		}

		SECTION("when there is a quote, a line feed and gardeSignesInvisibles is true")
		{
			iss.str("\"un\n deux");
			ligneAttendue = {"un\n deux"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, delimMots, true);

			SECTION("... check last read character")
			{
				CHECK(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}

		}

		SECTION("when there are several words with a custom delimitor")
		{
			iss.str("un%deux%trois");
			ligneAttendue = {"un", "deux", "trois"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, '%', gardeSignesInvisibles);

			SECTION("... check last read character")
			{
				CHECK(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}
		}

		SECTION("when there are several words with a custom delimitor and gardeSignesInvisibles is true")
		{
			iss.str("u\tn%deux%trois");
			ligneAttendue = {"u\tn", "deux", "trois"};

			bool endOfStream = tokenizer.lectureLigne(iss, ligne, '%', true);

			SECTION("... check last read character")
			{
				CHECK(endOfStream);
			}
			SECTION("... check read word")
			{
				CHECK(ligne == ligneAttendue);
			}
		}

	}

}
