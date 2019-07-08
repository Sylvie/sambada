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

#include "histograms/StoreyHistograms.hpp"
#include "common/TypesCommuns.hpp"

#include "catch.hpp"
#include <numeric>

void compareHistogramCounts(const std::vector<int>& counts, const std::vector<int>& expectedCounts);

TEST_CASE("Test that StoreyHistograms can create p-values histograms", "[storey-histograms-unit]")
{
	std::vector<sambada::StoreyHistograms::ScoreType> listeScoresTypes(
			{
					sambada::StoreyHistograms::ScoreType::G,
					sambada::StoreyHistograms::ScoreType::GOrphelins,
					sambada::StoreyHistograms::ScoreType::GPop,
					sambada::StoreyHistograms::ScoreType::Wald,
					sambada::StoreyHistograms::ScoreType::WaldOrphelins,
					sambada::StoreyHistograms::ScoreType::WaldPop
			}
	);

	SECTION("Test that scoreMin is correct")
	{
		sambada::StoreyHistograms storeyHistograms(3, 6.);

		CHECK(storeyHistograms.scoreMin == Approx(6.));
	}

	SECTION("Test that p-values are correct")
	{
		sambada::StoreyHistograms storeyHistograms(1, 6.);

		std::vector<sambada::reel> expectedPValues({0.95, 0.94, 0.93, 0.92, 0.91, 0.9, 0.89, 0.88, 0.87, 0.86, 0.85, 0.84, 0.83, 0.82, 0.81, 0.8, 0.79, 0.78, 0.77, 0.76, 0.75, 0.74, 0.73, 0.72, 0.71, 0.7, 0.69, 0.68, 0.67, 0.66, 0.65, 0.64, 0.63, 0.62, 0.61, 0.6, 0.59, 0.58, 0.57, 0.56, 0.55, 0.54, 0.53, 0.52, 0.51, 0.5, 0.49, 0.48, 0.47, 0.46, 0.45, 0.44, 0.43, 0.42, 0.41, 0.4, 0.39, 0.38, 0.37, 0.36, 0.35, 0.34, 0.33, 0.32, 0.31, 0.3, 0.29, 0.28, 0.27, 0.26, 0.25, 0.24, 0.23, 0.22, 0.21, 0.2, 0.19, 0.18, 0.17, 0.16, 0.15, 0.14, 0.13, 0.12, 0.11, 0.1, 0.09, 0.08, 0.07, 0.0599999999999999, 0.0499999999999999, 0.0399999999999999, 0.0299999999999999, 0.0199999999999999, 0.0099999999999999});
		std::vector<sambada::reel> pValues(storeyHistograms.getPValues());

		CHECKED_IF(pValues.size() == expectedPValues.size())
		{
			for (size_t i(0); i < expectedPValues.size(); ++i)
			{
				CHECK(pValues[i] == Approx(expectedPValues[i]));
			}
		}
	}

	SECTION("Test that score thresholds are correct")
	{
		sambada::StoreyHistograms storeyHistograms(1, 6.);

		std::vector<sambada::reel> expectedScoreThresholds(
				{0.00393214000001953, 0.00566555214052741, 0.007716715544952, 0.010086932215776, 0.012777711671053, 0.0157907740934313, 0.0191280539398315, 0.0227917040277124, 0.0267841001161233, 0.0311078460021464, 0.0357657791558977, 0.040760976920001, 0.0460967633024078, 0.0517767163946213, 0.0578046764508421, 0.0641847546673016, 0.0709213427051411, 0.0780191230046582, 0.0854830799436345, 0.0933185118978289, 0.101531044267622, 0.110126643541314, 0.119111632472776, 0.128492706459102, 0.138276951212761, 0.148471861832546, 0.159085363388553, 0.170125833148597, 0.18160212458709, 0.193523593332646, 0.205900125227766, 0.218742166693153, 0.23206075761083, 0.24586756696459, 0.260174931503893, 0.274995897728456, 0.290344267526239, 0.306234647837711, 0.322682504765163, 0.339704222598183, 0.35731716828632, 0.375539761958745, 0.394391554169761, 0.413893310640177, 0.434067105369943, 0.454936423119573, 0.476526272399809, 0.498863310274476, 0.521975980474872, 0.545894666550918, 0.570651862051189,
				 0.596282360039052, 0.622823464625417, 0.650315227642454, 0.678800714112448, 0.708326300800794, 0.738942012906212, 0.770701904864674, 0.803664492364912, 0.837893244041483, 0.87345714298923, 0.910431330311541, 0.94889784547952, 0.988946481478024, 1.03067577672932, 1.07419417085759, 1.11962135781185, 1.16708987813886, 1.21674700288959, 1.2687569755773, 1.32330369693147, 1.38059396152976, 1.44086138805315, 1.50437122924054, 1.57142630852761, 1.64237441514982, 1.7176176092514, 1.79762406036565, 1.88294329343061, 1.97422608959095, 2.07225085582224, 2.17795915885414, 2.29250452609228, 2.41732093045561, 2.55422131249636, 2.70554345409542, 2.874373395996, 3.06490172007636, 3.28302028675954, 3.5373845964626, 3.84145882069412, 4.2178845879214, 4.70929224688511, 5.41189443105435, 6.63489660102123});
		std::vector<sambada::reel> scoreThresholds(storeyHistograms.getScoreThresholds());

		CHECKED_IF(scoreThresholds.size() == expectedScoreThresholds.size())
		{
			for (size_t i(0); i < expectedScoreThresholds.size(); ++i)
			{
				CHECK(scoreThresholds[i] == Approx(expectedScoreThresholds[i]));
			}
		}
	}

	SECTION("Test that score thresholds are correct for each GroupHistogram")
	{
		size_t dimMax(2);
		sambada::StoreyHistograms storeyHistograms(dimMax, 6.);

		std::vector<sambada::reel> expectedScoreThresholds(
				{0.00393214000001953, 0.00566555214052741, 0.007716715544952, 0.010086932215776, 0.012777711671053, 0.0157907740934313, 0.0191280539398315, 0.0227917040277124, 0.0267841001161233, 0.0311078460021464, 0.0357657791558977, 0.040760976920001, 0.0460967633024078, 0.0517767163946213, 0.0578046764508421, 0.0641847546673016, 0.0709213427051411, 0.0780191230046582, 0.0854830799436345, 0.0933185118978289, 0.101531044267622, 0.110126643541314, 0.119111632472776, 0.128492706459102, 0.138276951212761, 0.148471861832546, 0.159085363388553, 0.170125833148597, 0.18160212458709, 0.193523593332646, 0.205900125227766, 0.218742166693153, 0.23206075761083, 0.24586756696459, 0.260174931503893, 0.274995897728456, 0.290344267526239, 0.306234647837711, 0.322682504765163, 0.339704222598183, 0.35731716828632, 0.375539761958745, 0.394391554169761, 0.413893310640177, 0.434067105369943, 0.454936423119573, 0.476526272399809, 0.498863310274476, 0.521975980474872, 0.545894666550918, 0.570651862051189,
				 0.596282360039052, 0.622823464625417, 0.650315227642454, 0.678800714112448, 0.708326300800794, 0.738942012906212, 0.770701904864674, 0.803664492364912, 0.837893244041483, 0.87345714298923, 0.910431330311541, 0.94889784547952, 0.988946481478024, 1.03067577672932, 1.07419417085759, 1.11962135781185, 1.16708987813886, 1.21674700288959, 1.2687569755773, 1.32330369693147, 1.38059396152976, 1.44086138805315, 1.50437122924054, 1.57142630852761, 1.64237441514982, 1.7176176092514, 1.79762406036565, 1.88294329343061, 1.97422608959095, 2.07225085582224, 2.17795915885414, 2.29250452609228, 2.41732093045561, 2.55422131249636, 2.70554345409542, 2.874373395996, 3.06490172007636, 3.28302028675954, 3.5373845964626, 3.84145882069412, 4.2178845879214, 4.70929224688511, 5.41189443105435, 6.63489660102123});

		for (auto scoreType(listeScoresTypes.begin()); scoreType != listeScoresTypes.end(); ++scoreType)
		{
			sambada::GroupHistograms histograms(storeyHistograms.getHistograms(*scoreType));
			CHECKED_IF(histograms.getHistograms().size() == (dimMax + 1))
			{
				for (size_t dimension(1); dimension <= dimMax; ++dimension)
				{
					sambada::Histogram histogram(histograms.getHistograms()[dimension]);
					std::vector<sambada::reel> scoreThresholds(histogram.getBinLimits());

					CHECKED_IF(scoreThresholds.size() == expectedScoreThresholds.size())
					{
						for (size_t i(0); i < expectedScoreThresholds.size(); ++i)
						{
							CHECK(scoreThresholds[i] == Approx(expectedScoreThresholds[i]));
						}
					}
				}
			}
		}

	}

	SECTION("Test that score names are correct for each GroupHistogram")
	{
		size_t dimMax(2);
		sambada::StoreyHistograms storeyHistograms(dimMax, 6.);

		std::vector<std::vector<std::string> > expectedNames({{"G0",             "G1",             "G2"},
		                                                      {"GOrphelins0",    "GOrphelins1",    "GOrphelins2"},
		                                                      {"GPop0",          "GPop1",          "GPop2"},
		                                                      {"Wald0",          "Wald1",          "Wald2"},
		                                                      {"WaldOrphelins0", "WaldOrphelins1", "WaldOrphelins2"},
		                                                      {"WaldPop0",       "WaldPop1",       "WaldPop2"}});

		for (size_t i(0); i < listeScoresTypes.size(); ++i)
		{
			sambada::GroupHistograms histograms(storeyHistograms.getHistograms(listeScoresTypes[i]));
			CHECKED_IF(histograms.getHistograms().size() == (dimMax + 1))
			{
				for (size_t dimension(0); dimension <= dimMax; ++dimension)
				{
					sambada::Histogram histogram(histograms.getHistograms()[dimension]);

					CHECK(histogram.getName() == expectedNames[i][dimension]);
				}
			}
		}
	}

	SECTION("Test that calling \"getHistograms()\" with a negative index returns first GroupHistogram")
	{
		size_t dimMax(2);
		sambada::StoreyHistograms storeyHistograms(dimMax, 6.);

		std::vector<std::string> expectedNames({"G0", "G1", "G2"});

		sambada::GroupHistograms histograms(storeyHistograms.getHistograms((sambada::StoreyHistograms::ScoreType) (-1)));
		CHECKED_IF(histograms.getHistograms().size() == (dimMax + 1))
		{
			for (size_t dimension(0); dimension <= dimMax; ++dimension)
			{
				sambada::Histogram histogram(histograms.getHistograms()[dimension]);

				CHECK(histogram.getName() == expectedNames[dimension]);
			}
		}
	}

	SECTION("Test that calling \"getHistograms()\" with an index greater than the last element returns last GroupHistogram")
	{
		size_t dimMax(2);
		sambada::StoreyHistograms storeyHistograms(dimMax, 6.);

		std::vector<std::string> expectedNames({"WaldPop0", "WaldPop1", "WaldPop2"});

		sambada::GroupHistograms histograms(storeyHistograms.getHistograms((sambada::StoreyHistograms::ScoreType) 6));
		CHECKED_IF(histograms.getHistograms().size() == (dimMax + 1))
		{
			for (size_t dimension(0); dimension <= dimMax; ++dimension)
			{
				sambada::Histogram histogram(histograms.getHistograms()[dimension]);

				CHECK(histogram.getName() == expectedNames[dimension]);
			}
		}
	}

	SECTION("Test that scores are counted in the correct bin")
	{
		size_t dimMax(2);
		sambada::StoreyHistograms storeyHistograms(dimMax, 0.);

		std::vector<std::vector<sambada::reel> > scores({{0.5, 1},
		                                                 {1.5, 2},
		                                                 {2.5, 3},
		                                                 {3.5, 4},
		                                                 {4.5, 5},
		                                                 {6,   7}});
		for (auto scoreType(listeScoresTypes.begin()); scoreType != listeScoresTypes.end(); ++scoreType)
		{
			for (size_t dimension(1); dimension <= dimMax; ++dimension)
			{
				storeyHistograms.addValue(*scoreType, dimension, scores[(size_t)(*scoreType)][dimension-1]);
			}

		}

		std::vector<std::vector<std::vector<int>>> expectedHistograms(6, std::vector<std::vector<int>>(2, std::vector<int>(96, 0)));
		expectedHistograms[(size_t) listeScoresTypes[0]][0][48] = 1;
		expectedHistograms[(size_t) listeScoresTypes[0]][1][64] = 1;
		expectedHistograms[(size_t) listeScoresTypes[1]][0][73] = 1;
		expectedHistograms[(size_t) listeScoresTypes[1]][1][80] = 1;
		expectedHistograms[(size_t) listeScoresTypes[2]][0][84] = 1;
		expectedHistograms[(size_t) listeScoresTypes[2]][1][87] = 1;
		expectedHistograms[(size_t) listeScoresTypes[3]][0][89] = 1;
		expectedHistograms[(size_t) listeScoresTypes[3]][1][91] = 1;
		expectedHistograms[(size_t) listeScoresTypes[4]][0][92] = 1;
		expectedHistograms[(size_t) listeScoresTypes[4]][1][93] = 1;
		expectedHistograms[(size_t) listeScoresTypes[5]][0][94] = 1;
		expectedHistograms[(size_t) listeScoresTypes[5]][1][95] = 1;

		for (auto scoreType(listeScoresTypes.begin()); scoreType != listeScoresTypes.end(); ++scoreType)
		{
			INFO("ScoreType: " + std::to_string((size_t) *scoreType));
			sambada::GroupHistograms histograms(storeyHistograms.getHistograms(*scoreType));
			CHECKED_IF(histograms.getHistograms().size() == (dimMax + 1))
			{
				for (size_t dimension(1); dimension <= dimMax; ++dimension)
				{
					INFO("Dimension: " + std::to_string(dimension));
					sambada::Histogram histogram(histograms.getHistograms()[dimension]);
					std::vector<int> counts(histogram.getCounts());

					compareHistogramCounts(counts, expectedHistograms[(size_t) *scoreType][dimension - 1]);
				}
			}
		}
	}

	SECTION("Test that scores less than the minimum are not counted")
	{
		size_t dimMax(2);
		sambada::StoreyHistograms storeyHistograms(dimMax, 3.);

		std::vector<std::vector<sambada::reel> > scores({{0.5, 1},
		                                                 {1.5, 2},
		                                                 {2.5, 3},
		                                                 {3.5, 4},
		                                                 {4.5, 5},
		                                                 {6,   7}});
		for (auto scoreType(listeScoresTypes.begin()); scoreType != listeScoresTypes.end(); ++scoreType)
		{
			for (size_t dimension(1); dimension <= dimMax; ++dimension)
			{
				storeyHistograms.addValue(*scoreType, dimension, scores[(size_t)(*scoreType)][dimension-1]);
			}

		}

		std::vector<std::vector<std::vector<int>>> expectedHistograms(6, std::vector<std::vector<int>>(2, std::vector<int>(96, 0)));
		expectedHistograms[(size_t) listeScoresTypes[2]][1][87] = 1;
		expectedHistograms[(size_t) listeScoresTypes[3]][0][89] = 1;
		expectedHistograms[(size_t) listeScoresTypes[3]][1][91] = 1;
		expectedHistograms[(size_t) listeScoresTypes[4]][0][92] = 1;
		expectedHistograms[(size_t) listeScoresTypes[4]][1][93] = 1;
		expectedHistograms[(size_t) listeScoresTypes[5]][0][94] = 1;
		expectedHistograms[(size_t) listeScoresTypes[5]][1][95] = 1;

		for (auto scoreType(listeScoresTypes.begin()); scoreType != listeScoresTypes.end(); ++scoreType)
		{
			INFO("ScoreType: " + std::to_string((size_t) *scoreType));
			sambada::GroupHistograms histograms(storeyHistograms.getHistograms(*scoreType));
			CHECKED_IF(histograms.getHistograms().size() == (dimMax + 1))
			{
				for (size_t dimension(1); dimension <= dimMax; ++dimension)
				{
					INFO("Dimension: " + std::to_string(dimension));
					sambada::Histogram histogram(histograms.getHistograms()[dimension]);
					std::vector<int> counts(histogram.getCounts());

					compareHistogramCounts(counts, expectedHistograms[(size_t) *scoreType][dimension - 1]);
				}
			}
		}

	}


	SECTION("Test that scores with non-existing ScoreType are not counted")
	{
		size_t dimMax(2);
		sambada::StoreyHistograms storeyHistograms(dimMax, 0.);

		std::vector<std::vector<sambada::reel> > scores({{0.5, 1},
		                                                 {1.5, 2},
		                                                 {2.5, 3},
		                                                 {3.5, 4},
		                                                 {4.5, 5},
		                                                 {6,   7}});
		for (auto scoreType(listeScoresTypes.begin()); scoreType != listeScoresTypes.end(); ++scoreType)
		{
			for (size_t dimension(1); dimension <= dimMax; ++dimension)
			{
				storeyHistograms.addValue(*scoreType, dimension, scores[(size_t)(*scoreType)][dimension-1]);
			}

		}

		storeyHistograms.addValue((sambada::StoreyHistograms::ScoreType)-1, 1, 3.7);
		storeyHistograms.addValue((sambada::StoreyHistograms::ScoreType)6, 2, 3.7);

		std::vector<std::vector<std::vector<int>>> expectedHistograms(6, std::vector<std::vector<int>>(2, std::vector<int>(96, 0)));
		expectedHistograms[(size_t) listeScoresTypes[0]][0][48] = 1;
		expectedHistograms[(size_t) listeScoresTypes[0]][1][64] = 1;
		expectedHistograms[(size_t) listeScoresTypes[1]][0][73] = 1;
		expectedHistograms[(size_t) listeScoresTypes[1]][1][80] = 1;
		expectedHistograms[(size_t) listeScoresTypes[2]][0][84] = 1;
		expectedHistograms[(size_t) listeScoresTypes[2]][1][87] = 1;
		expectedHistograms[(size_t) listeScoresTypes[3]][0][89] = 1;
		expectedHistograms[(size_t) listeScoresTypes[3]][1][91] = 1;
		expectedHistograms[(size_t) listeScoresTypes[4]][0][92] = 1;
		expectedHistograms[(size_t) listeScoresTypes[4]][1][93] = 1;
		expectedHistograms[(size_t) listeScoresTypes[5]][0][94] = 1;
		expectedHistograms[(size_t) listeScoresTypes[5]][1][95] = 1;

		for (auto scoreType(listeScoresTypes.begin()); scoreType != listeScoresTypes.end(); ++scoreType)
		{
			INFO("ScoreType: " + std::to_string((size_t) *scoreType));
			sambada::GroupHistograms histograms(storeyHistograms.getHistograms(*scoreType));
			CHECKED_IF(histograms.getHistograms().size() == (dimMax + 1))
			{
				for (size_t dimension(1); dimension <= dimMax; ++dimension)
				{
					INFO("Dimension: " + std::to_string(dimension));
					sambada::Histogram histogram(histograms.getHistograms()[dimension]);
					std::vector<int> counts(histogram.getCounts());

					compareHistogramCounts(counts, expectedHistograms[(size_t) *scoreType][dimension - 1]);
				}
			}
		}
	}
}
