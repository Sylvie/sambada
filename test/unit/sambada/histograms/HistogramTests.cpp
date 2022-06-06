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

#include "histograms/Histogram.hpp"
#include "common/TypesCommuns.hpp"

#include "catch.hpp"
#include <numeric>

void compareHistogramCounts(const std::vector<int>& counts, const std::vector<int>& expectedCounts)
{
	CHECKED_IF(counts.size() == expectedCounts.size())
	{
		for (size_t i(0); i < expectedCounts.size(); ++i)
		{
			INFO("Bin number: " + std::to_string(i));
			CHECK(counts[i] == expectedCounts[i]);
		}
	}
}

TEST_CASE("Test that Histogram can create an histogram", "[histogram-unit]")
{
	std::string name("monHistogramme");
	std::vector<sambada::reel> binLimits({2, 5, 7, 20});

	SECTION("Test that Histogram has correct name")
	{
		sambada::Histogram histogram(name, {});

		CHECK(histogram.getName() == "monHistogramme");
	}

	SECTION("Test that Histogram without separations has a single bin")
	{
		sambada::Histogram histogram(name, {});

		auto limits(histogram.getBinLimits());
		CHECK(limits.size() == 0);

		auto counts(histogram.getCounts());
		CHECK(counts.size() == 1);
	}

	SECTION("Test that Histogram without separations counts values")
	{
		sambada::Histogram histogram(name, {});

		std::vector<double> values({0.5, -130, 1119372.576});
		for (auto value: values)
		{
			histogram.addValue(value);
		}

		auto counts(histogram.getCounts());
		CHECKED_IF(counts.size() == 1)
		{
			CHECK(counts[0] == 3);
		}
	}

	SECTION("Test that Histogram puts value in first bin correctly")
	{
		sambada::Histogram histogram(name, binLimits);

		histogram.addValue(1.993);

		std::vector<int> expectedCounts({1, 0, 0, 0, 0});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts value equal to lower limit in second bin correctly")
	{
		sambada::Histogram histogram(name, binLimits);

		histogram.addValue(binLimits[0]);

		std::vector<int> expectedCounts({0, 1, 0, 0, 0});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts value in second bin correctly")
	{
		sambada::Histogram histogram(name, binLimits);

		histogram.addValue(3.31);

		std::vector<int> expectedCounts({0, 1, 0, 0, 0});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts value equal to second lower limit in third bin correctly")
	{
		sambada::Histogram histogram(name, binLimits);

		histogram.addValue(binLimits[1]);

		std::vector<int> expectedCounts({0, 0, 1, 0, 0});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts value in middle bin correctly")
	{
		sambada::Histogram histogram(name, binLimits);

		histogram.addValue(6);

		std::vector<int> expectedCounts({0, 0, 1, 0, 0});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts value equal to second upper limit in second last bin correctly")
	{
		sambada::Histogram histogram(name, binLimits);

		histogram.addValue(binLimits[2]);

		std::vector<int> expectedCounts({0, 0, 0, 1, 0});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts value in second last bin correctly")
	{
		sambada::Histogram histogram(name, binLimits);

		histogram.addValue(8.1);

		std::vector<int> expectedCounts({0, 0, 0, 1, 0});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts value equal to upper limit in last bin correctly")
	{
		sambada::Histogram histogram(name, binLimits);

		histogram.addValue(binLimits[3]);

		std::vector<int> expectedCounts({0, 0, 0, 0, 1});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts value in last bin correctly")
	{
		sambada::Histogram histogram(name, binLimits);

		histogram.addValue(21);

		std::vector<int> expectedCounts({0, 0, 0, 0, 1});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts values in correct bins")
	{
		sambada::Histogram histogram(name, binLimits);

		std::vector<double> values(41);
		std::iota(values.begin(), values.end(), -10);

		for (double value : values)
		{
			histogram.addValue(value);
		}

		std::vector<int> expectedCounts({12, 3, 2, 13, 11});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}

	SECTION("Test that Histogram puts values in correct bins when the limits are not sorted")
	{
		std::vector<sambada::reel> shuffledBinLimits({7, 5, 20, 2});

		sambada::Histogram histogram(name, shuffledBinLimits);

		std::vector<sambada::reel> values(41);
		std::iota(values.begin(), values.end(), -10);

		for (double value : values)
		{
			histogram.addValue(value);
		}

		std::vector<int> expectedCounts({12, 3, 2, 13, 11});

		auto counts(histogram.getCounts());

		compareHistogramCounts(counts, expectedCounts);
	}
}
