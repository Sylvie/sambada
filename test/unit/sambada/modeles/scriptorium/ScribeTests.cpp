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
#include "modeles/scriptorium/Scribe.hpp"
#include "modeles/scriptorium/FlotSortieChaineFactory.hpp"

TEST_CASE("Test that Scribe can write in several output streams", "[scribe-unit]")
{

	int nombreFlots(3);
	std::vector<std::string> nomsFlots({"premierFlot", "deuxièmeFlot", "troisièmeFlot"});

	sambada::FlotSortieChaineFactory factory;

	sambada::Scribe scribe(factory);

	char delimMots(' ');
	std::string retourLigne("\n");
	int precision(15);

	SECTION("Test that Scribe creates the correct number of streams")
	{
		scribe.initialise(nomsFlots, retourLigne, delimMots, precision);

		CHECK(factory.getFlotsSortie().size() == nombreFlots);
	}

	SECTION("Test that Scribe creates streams with the correct names")
	{
		scribe.initialise(nomsFlots, retourLigne, delimMots, precision);

		CHECK(factory.getNomsFlots() == nomsFlots);
	}

	SECTION("Test that stream precisions are correct")
	{
		scribe.initialise(nomsFlots, retourLigne, delimMots, precision);

		std::vector<FlotSortie> flots(factory.getFlotsSortie());
		for (auto flot(flots.begin()); flot != flots.end(); ++flot)
		{
			std::ostream& flotInterieur(*flot->get());
			CHECK(flotInterieur.precision() == precision);
		}
	}

}
