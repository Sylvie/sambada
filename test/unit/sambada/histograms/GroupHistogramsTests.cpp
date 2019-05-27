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

#include "histograms/GroupHistograms.hpp"
#include "common/TypesCommuns.hpp"

#include "catch.hpp"
#include <numeric>

void compareHistogramCounts(const std::vector<int>& counts, const std::vector<int>& expectedCounts);

TEST_CASE("Test that GroupHistograms can create a group of histograms", "[group-histograms-unit]")
{
	std::string name("monHistogramme");
	std::vector<sambada::reel> binLimits({2, 5, 7, 20});
	size_t length(3);

	SECTION("Test that GroupHistograms can be empty")
	{
		sambada::GroupHistograms groupHistograms(0, name, binLimits);

		auto histograms(groupHistograms.getHistograms());

		CHECK(histograms.size() == 0);
	}

	SECTION("Test that GroupHistograms cannot have negative size")
	{
		sambada::GroupHistograms groupHistograms(-3, name, binLimits);

		auto histograms(groupHistograms.getHistograms());

		CHECK(histograms.size() == 0);
	}

	SECTION("Test that GroupHistograms has the correct number of Histograms")
	{
		sambada::GroupHistograms groupHistograms(length, name, binLimits);

		auto histograms(groupHistograms.getHistograms());

		CHECK(histograms.size() == length);
	}

	SECTION("Test that Histograms have the correct names")
	{
		sambada::GroupHistograms groupHistograms(length, name, binLimits);

		auto histograms(groupHistograms.getHistograms());

		CHECKED_IF(histograms.size() == length)
		{
			CHECK(histograms[0].getName() == "monHistogramme0");
			CHECK(histograms[1].getName() == "monHistogramme1");
			CHECK(histograms[2].getName() == "monHistogramme2");
		}
	}

	SECTION("Test that Histograms have the correct counts")
	{

		std::vector<sambada::reel> values(43);
		std::iota(values.begin(), values.end(), -10);
		std::vector<std::vector<int>> expectedCounts({{4, 1, 1, 4, 5},
		                                              {4, 1, 1, 4, 4},
		                                              {4, 1, 0, 5, 4}});

		sambada::GroupHistograms groupHistograms(length, name, binLimits);

		for (size_t i(0); i < values.size(); ++i)
		{
			groupHistograms.addValue(i % length, values[i]);
		}

		auto histograms(groupHistograms.getHistograms());

		CHECKED_IF(histograms.size() == length)
		{
			compareHistogramCounts(histograms[0].getCounts(), expectedCounts[0]);
			compareHistogramCounts(histograms[1].getCounts(), expectedCounts[1]);
			compareHistogramCounts(histograms[2].getCounts(), expectedCounts[2]);
		}
	}
}
