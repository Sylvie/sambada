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

#include "common/Probability.hpp"
#include "catch.hpp"

#include <cmath>
#include <limits>

TEST_CASE("Test that Probability results are correct", "[probability-unit]")
{

	sambada::reel precision(std::sqrt(std::numeric_limits<sambada::reel>::epsilon()));

	std::vector<sambada::reel> values(100, 0);
	for (size_t i(0); i < values.size(); ++i)
	{
		values[i] = 1. - i * 0.01;
	}

	SECTION("Test that chiSquareQuantileFunction returns NaN for null or negative degree of liberty")
	{
		sambada::reel pValeur(0.01);
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(pValeur, 0, precision)));
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(pValeur, -1, precision)));
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(pValeur, -100, precision)));
	}

	SECTION("Test that chiSquareQuantileFunction returns NaN for null or negative convergence threshold")
	{
		sambada::reel pValeur(0.01);
		int degreeFreedom(1);
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(pValeur, degreeFreedom, 0.)));
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(pValeur, degreeFreedom, -1.)));
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(pValeur, degreeFreedom, -1000.)));
	}

	SECTION("Test that chiSquareQuantileFunction returns NaN for p-values lower than 0")
	{
		int degreeFreedom(1);
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(-0.01, degreeFreedom, precision)));
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(-5, degreeFreedom, precision)));
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(-1000.54, degreeFreedom, precision)));
	}

	SECTION("Test that chiSquareQuantileFunction returns NaN for p-values greater than 1")
	{
		int degreeFreedom(1);
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(1.000001, degreeFreedom, precision)));
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(2., degreeFreedom, precision)));
		CHECK(std::isnan(sambada::probability::chiSquareQuantileFunction(540003.5, degreeFreedom, precision)));
	}

	SECTION("Test that chiSquareQuantileFunction returns 0 for p-values equal to 0")
	{
		int pValue(0.);
		CHECK(abs(sambada::probability::chiSquareQuantileFunction(pValue, 1, precision)) < std::numeric_limits<sambada::reel>::min());
		CHECK(abs(sambada::probability::chiSquareQuantileFunction(pValue, 2, precision)) < std::numeric_limits<sambada::reel>::min());
		CHECK(abs(sambada::probability::chiSquareQuantileFunction(pValue, 10, precision)) < std::numeric_limits<sambada::reel>::min());
	}

	SECTION("Test that chiSquareQuantileFunction computes correct p-values for 1 degree of liberty")
	{
		std::vector<sambada::reel> expectedResults(
				{0, 0.000157087857909702, 0.000628450161283672, 0.00141438330081178, 0.00251538193435401, 0.00393214000001953, 0.00566555214052741, 0.007716715544952, 0.010086932215776, 0.012777711671053, 0.0157907740934312, 0.0191280539398314, 0.0227917040277124, 0.0267841001161233, 0.0311078460021464, 0.0357657791558977, 0.040760976920001, 0.0460967633024078, 0.0517767163946213, 0.057804676450842, 0.0641847546673016, 0.0709213427051411, 0.0780191230046581, 0.0854830799436345, 0.0933185118978289, 0.101531044267622, 0.110126643541314, 0.119111632472776, 0.128492706459102, 0.138276951212761, 0.148471861832546, 0.159085363388553, 0.170125833148597, 0.18160212458709, 0.193523593332646, 0.205900125227766, 0.218742166693153, 0.232060757610829, 0.24586756696459, 0.260174931503893, 0.274995897728456, 0.290344267526239, 0.306234647837711, 0.322682504765163, 0.339704222598183, 0.35731716828632, 0.375539761958745, 0.394391554169761, 0.413893310640177, 0.434067105369943, 0.454936423119573,
				 0.476526272399809, 0.498863310274476, 0.521975980474872, 0.545894666550918, 0.570651862051189, 0.596282360039052, 0.622823464625417, 0.650315227642454, 0.678800714112447, 0.708326300800794, 0.738942012906212, 0.770701904864674, 0.803664492364912, 0.837893244041483, 0.87345714298923, 0.910431330311541, 0.94889784547952, 0.988946481478024, 1.03067577672932, 1.07419417085759, 1.11962135781185, 1.16708987813886, 1.21674700288959, 1.2687569755773, 1.32330369693147, 1.38059396152976, 1.44086138805315, 1.50437122924054, 1.57142630852761, 1.64237441514982, 1.7176176092514, 1.79762406036565, 1.88294329343061, 1.97422608959095, 2.07225085582224, 2.17795915885414, 2.29250452609228, 2.41732093045562, 2.55422131249636, 2.70554345409542, 2.874373395996, 3.06490172007636, 3.28302028675954, 3.5373845964626, 3.84145882069412, 4.2178845879214, 4.7092922468851, 5.41189443105434, 6.63489660102121});

		CHECKED_IF(expectedResults.size() == 100)
		{
			// Cas pVal = 1.0, valeur attendue = 0.0, une marge est nécessaire
			{
				INFO("value: " + std::to_string(values[0]))
				CHECK(sambada::probability::chiSquareQuantileFunction(1. - values[0], 1, precision) == Approx(expectedResults[0]).margin(precision));
			}
			for (size_t i(1); i < values.size(); ++i)
			{
				INFO("value: " + std::to_string(values[i]));
				CHECK(sambada::probability::chiSquareQuantileFunction(1. - values[i], 1, precision) == Approx(expectedResults[i]));
			}
		}
	}
}
