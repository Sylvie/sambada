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

#include "common/maths/Probability.hpp"
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
		sambada::reel pValue(0.);
		CHECK(abs(sambada::probability::chiSquareQuantileFunction(pValue, 1, precision)) < std::numeric_limits<sambada::reel>::min());
		CHECK(abs(sambada::probability::chiSquareQuantileFunction(pValue, 2, precision)) < std::numeric_limits<sambada::reel>::min());
		CHECK(abs(sambada::probability::chiSquareQuantileFunction(pValue, 10, precision)) < std::numeric_limits<sambada::reel>::min());
	}

	SECTION("Test that chiSquareQuantileFunction returns infinity for p-values equal to 1")
	{
		sambada::reel pValue(1.);
		CHECK(sambada::probability::chiSquareQuantileFunction(pValue, 1, precision) == std::numeric_limits<sambada::reel>::infinity());
		CHECK(sambada::probability::chiSquareQuantileFunction(pValue, 2, precision) == std::numeric_limits<sambada::reel>::infinity());
		CHECK(sambada::probability::chiSquareQuantileFunction(pValue, 10, precision) == std::numeric_limits<sambada::reel>::infinity());
	}

	SECTION("Test that chiSquareQuantileFunction computes correct p-values for 1 degree of liberty")
	{
		std::vector<sambada::reel> expectedResults(
				{0, 0.000157087857909702, 0.000628450161283672, 0.00141438330081178, 0.00251538193435401, 0.00393214000001953, 0.00566555214052741, 0.007716715544952, 0.010086932215776, 0.012777711671053, 0.0157907740934312, 0.0191280539398314, 0.0227917040277124, 0.0267841001161233, 0.0311078460021464, 0.0357657791558977, 0.040760976920001, 0.0460967633024078, 0.0517767163946213, 0.057804676450842, 0.0641847546673016, 0.0709213427051411, 0.0780191230046581, 0.0854830799436345, 0.0933185118978289, 0.101531044267622, 0.110126643541314, 0.119111632472776, 0.128492706459102, 0.138276951212761, 0.148471861832546, 0.159085363388553, 0.170125833148597, 0.18160212458709, 0.193523593332646, 0.205900125227766, 0.218742166693153, 0.232060757610829, 0.24586756696459, 0.260174931503893, 0.274995897728456, 0.290344267526239, 0.306234647837711, 0.322682504765163, 0.339704222598183, 0.35731716828632, 0.375539761958745, 0.394391554169761, 0.413893310640177, 0.434067105369943, 0.454936423119573,
				 0.476526272399809, 0.498863310274476, 0.521975980474872, 0.545894666550918, 0.570651862051189, 0.596282360039052, 0.622823464625417, 0.650315227642454, 0.678800714112447, 0.708326300800794, 0.738942012906212, 0.770701904864674, 0.803664492364912, 0.837893244041483, 0.87345714298923, 0.910431330311541, 0.94889784547952, 0.988946481478024, 1.03067577672932, 1.07419417085759, 1.11962135781185, 1.16708987813886, 1.21674700288959, 1.2687569755773, 1.32330369693147, 1.38059396152976, 1.44086138805315, 1.50437122924054, 1.57142630852761, 1.64237441514982, 1.7176176092514, 1.79762406036565, 1.88294329343061, 1.97422608959095, 2.07225085582224, 2.17795915885414, 2.29250452609228, 2.41732093045562, 2.55422131249636, 2.70554345409542, 2.874373395996, 3.06490172007636, 3.28302028675954, 3.5373845964626, 3.84145882069412, 4.2178845879214, 4.7092922468851, 5.41189443105434, 6.63489660102121});

		CHECKED_IF(expectedResults.size() == 100)
		{
			for (size_t i(0); i < values.size(); ++i)
			{
				INFO("value: " + std::to_string(values[i]));
				sambada::reel score(sambada::probability::chiSquareQuantileFunction(1. - values[i], 1, precision));
				INFO("score: " + std::to_string(score));
				sambada::reel marge(0.0);
				// Cas pVal = 1.0, valeur attendue = 0.0, une marge est nécessaire
				if (i == 0)
				{
					marge=precision;
				}
				CHECK(score == Approx(expectedResults[i]).margin(marge));
			}
		}
	}

	SECTION("Test that chiSquareQuantileFunction computes correct p-values for 2 degrees of liberty")
	{
		std::vector<sambada::reel> expectedResults(
				{0, 0.0201006717070029, 0.0404054146350389, 0.0609184149694171, 0.0816439890405103, 0.102586588775101, 0.123750807436175, 0.145141385669671, 0.166763217878102, 0.188621358942483, 0.210721031315653, 0.233067632511903, 0.25566674301977, 0.278524134667015, 0.301645779469167, 0.32503785899555, 0.348706774289556, 0.372659156382987, 0.396901877447677, 0.421442062631305, 0.446287102628419, 0.47144466704214, 0.496922718596999, 0.522729528268815, 0.54887369140352, 0.575364144903562, 0.602210185567843, 0.629421489679401, 0.657008133944072, 0.684980617893552, 0.713349887877465, 0.742127362781664, 0.77132496162397, 0.800955133194251, 0.831030887923332, 0.861565832184909, 0.892574205256839, 0.924070919193117, 0.956071601886, 0.98859264362956, 1.02165124753198, 1.05526548416474, 1.08945435088334, 1.12423783630708, 1.15963699050588, 1.19567400151124, 1.23237227884763, 1.26975654487194, 1.30785293481333, 1.34668910652753, 1.38629436111989, 1.42669977575493, 1.4679383501604, 1.51004516855607, 1.55305757899799, 1.59701539243554, 1.64196110413966, 1.68794014058906, 1.73500113540945, 1.78319623856757, 1.83258146374831, 1.88321707971689, 1.93516805252341, 1.98850454668773, 2.04330249506396, 2.09964424899736, 2.15761932274386, 2.21732524904322, 2.27886856637673, 2.34236596300589, 2.40794560865187, 2.47574871200324, 2.54593135162577, 2.61866663996752, 2.69414729593322, 2.77258872223978, 2.85423271128029, 2.93935194011788, 3.02825546525955, 3.12129549652934, 3.2188758248682, 3.3214624136433, 3.42959685618385, 3.54391368386375, 3.66516292749662, 3.79423996977176, 3.93222571274567, 4.08044165705311, 4.24052707240018, 4.41454982637944, 4.60517018598809, 4.81589121730375, 5.05145728861651, 5.31852007386556, 5.62682143352008, 5.99146454710798, 6.4377516497364, 7.01311579463996, 7.82404601085629, 9.21034037197618});

		CHECKED_IF(expectedResults.size() == 100)
		{
			for (size_t i(0); i < values.size(); ++i)
			{
				INFO("value: " + std::to_string(values[i]));
				sambada::reel score(sambada::probability::chiSquareQuantileFunction(1. - values[i], 2, precision));
				INFO("score: " + std::to_string(score));
				sambada::reel marge(0.0);
				// Cas pVal = 1.0, valeur attendue = 0.0, une marge est nécessaire
				if (i == 0)
				{
					marge=precision;
				}
				CHECK(score == Approx(expectedResults[i]).margin(marge));
			}
		}
	}

	SECTION("Test that chiSquareQuantileFunction computes correct p-values for 3 degrees of liberty")
	{
		std::vector<sambada::reel> expectedResults({0, 0.114831801899117, 0.184831819028658, 0.245098708710298, 0.300151418797226, 0.351846317749272, 0.401173358000625, 0.448738701326967, 0.494947561697215, 0.540087992925564, 0.584374374155183, 0.627972075125604, 0.671012415662294, 0.713602227787573, 0.755830232541817, 0.797771444244722, 0.839490303533742, 0.881042963208507, 0.922478993077795, 0.963842676406123, 1.00517401305235, 1.04650950796041, 1.0878827999489, 1.12932516993795, 1.17086595698709, 1.21253290304567, 1.25435244204196, 1.29634994515353, 1.33854993135059, 1.3809762502783, 1.42365224303528, 1.46660088527122, 1.50984491616565, 1.55340695619226, 1.59730961606829, 1.64157559889882, 1.68622779722592, 1.73128938646164, 1.77678391600776, 1.8227353992338, 1.86916840338872, 1.91610814045644, 1.9635805599259, 2.01161244442959, 2.06023150920944, 2.10946650639279, 2.15934733510585, 2.20990515851526, 2.26117252897367, 2.31318352255218, 2.36597388437534, 2.41958118633389, 2.47404499894407, 2.52940707935127, 2.58571157775017, 2.64300526481826, 2.7013377831473, 2.76076192611722, 2.82133394820745, 2.88311391139796, 2.94616607310195, 3.01055932202216, 3.0763676694726, 3.14367080510243, 3.21255472766055, 3.2831124635255, 3.35544488829475, 3.42966166991078, 3.50588235576818, 3.58423763121895, 3.66487078317032, 3.74793941044676, 3.83361743280234, 3.92209746365341, 4.01359362876118, 4.10834493563232, 4.20661932829835, 4.30871860219529, 4.41498440816162, 4.52580564906147, 4.64162767608745, 4.76296383781555, 4.89041014424292, 5.02466411282651, 5.16654931616078, 5.3170478373171, 5.47734390432258, 5.64888367296753, 5.83345891340976, 6.03332708539163, 6.25138863117032, 6.49145771584493, 6.75869261969684, 7.06031416781787, 7.40688004310323, 7.81472790325118, 8.31117091082631, 8.94728749887945, 9.83740931119259, 11.3448667301444});

		CHECKED_IF(expectedResults.size() == 100)
		{
			for (size_t i(0); i < values.size(); ++i)
			{
				INFO("value: " + std::to_string(values[i]));
				sambada::reel score(sambada::probability::chiSquareQuantileFunction(1. - values[i], 3, precision));
				INFO("score: " + std::to_string(score));
				sambada::reel marge(0.0);
				// Cas pVal = 1.0, valeur attendue = 0.0, une marge est nécessaire
				if (i == 0)
				{
					marge=precision;
				}
				CHECK(score == Approx(expectedResults[i]).margin(marge));
			}
		}
	}


	SECTION("Test that chiSquareQuantileFunction computes correct p-values for 10 degrees of liberty")
	{
		std::vector<sambada::reel> expectedResults({0, 2.55821216018721, 3.05905141086872, 3.41206854712002, 3.69654144495669, 3.94029913611906, 4.15672429181494, 4.35339173166206, 4.53504966643008, 4.70489208499592, 4.86518205192533, 5.01758842555041, 5.16338163551605, 5.303554244264, 5.43889870272397, 5.57005944421596, 5.69756891786533, 5.8218731997855, 5.94335062844352, 6.06232564448647, 6.17907925603939, 6.29385708012509, 6.40687561103036, 6.51832717043078, 6.62838386301954, 6.73720077195464, 6.84491856629256, 6.95166564866947, 7.05755993999874, 7.16271037505222, 7.26721816592761, 7.371177877839, 7.47467835220645, 7.57780350482595, 7.68063302138202, 7.78324296829606, 7.88570633357778, 7.98809350973929, 8.09047272877589, 8.19291045758977, 8.29547176094109, 8.39822063798395, 8.50122033762981, 8.60453365733765, 8.70822322942696, 8.81235179862397, 8.91698249426426, 9.02217910037435, 9.12800632673044, 9.23453008393846, 9.34181776559197, 9.44993854064245, 9.55896365926067, 9.66896677568202, 9.78002429181837, 9.89221572579308, 10.0056241100251, 10.1203364240685, 10.2364440681241, 10.354043384006, 10.4732362313955, 10.5941306284946, 10.7168414677462, 10.8414913191777, 10.9682113362501, 11.0971422819318, 11.2284356962292, 11.3622552307526, 11.4987781813113, 11.6381972563243, 11.780722627394, 11.9265843192707, 12.0760350103478, 12.2293533327721, 12.3868477845811, 12.5488613968894, 12.7157773396884, 12.8880257041011, 13.0660917723901, 13.2505261876681, 13.4419575749731, 13.6411083621069, 13.848814830017, 14.0660528318963, 14.2939712271443, 14.533935995231, 14.7875894183369, 15.05693098618, 15.3444303857069, 15.6531892212831, 15.9871791721053, 16.3516046605637, 16.7534776469389, 17.2025739686041, 17.713123565424, 18.3070380532751, 19.0207433482011, 19.9219100082354, 21.1607675413047, 23.2092511589544});

		CHECKED_IF(expectedResults.size() == 100)
		{
			for (size_t i(0); i < values.size(); ++i)
			{
				INFO("value: " + std::to_string(values[i]));
				sambada::reel score(sambada::probability::chiSquareQuantileFunction(1. - values[i], 10, precision));
				INFO("score: " + std::to_string(score));
				sambada::reel marge(0.0);
				// Cas pVal = 1.0, valeur attendue = 0.0, une marge est nécessaire
				if (i == 0)
				{
					marge=precision;
				}
				CHECK(score == Approx(expectedResults[i]).margin(marge));
			}
		}
	}
}
