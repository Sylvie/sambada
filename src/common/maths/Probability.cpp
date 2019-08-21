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

#include "Probability.hpp"

#include "distributions.h"

namespace sambada::probability {


	reel searchForUpperBoundScore(reel pValeur, int deglib)
	{
		int nombreIterations(0);
		int limiteIterations(1000);

		sambada::reel scoreMajorant(1.);
		while (scythe::pchisq(scoreMajorant, deglib) < pValeur && nombreIterations < limiteIterations)
		{
			scoreMajorant *= 2.;
			++nombreIterations;
		}

		if (nombreIterations == limiteIterations)
		{
			scoreMajorant == std::numeric_limits<reel>::quiet_NaN();
		}

		return scoreMajorant;
	}

	reel dichotomicChiSquareQuantileFunction(reel pValeur, int deglib, reel scoreMajorant, reel seuilConv)
	{
		int nombreIterations(0);
		int limiteIterations(1000);

		sambada::reel scoreMinorant(0.);
		sambada::reel scoreCentral(0.);
		sambada::reel pValeurEstimee(0.);
		sambada::reel residu(0.);
		do
		{
			scoreCentral = (scoreMinorant + scoreMajorant) / 2;
			pValeurEstimee = scythe::pchisq(scoreCentral, deglib);
			residu = pValeurEstimee - pValeur;

			if (0 <= residu)
			{
				scoreMajorant = scoreCentral;
			}
			else
			{
				scoreMinorant = scoreCentral;
			}

			++nombreIterations;
		} while (seuilConv < std::abs(residu) && nombreIterations < limiteIterations);

		return scoreCentral;
	}


	reel chiSquareQuantileFunction(reel pValeur, int deglib, reel seuilConv)
	{
		reel score(1.0), limiteDomaine(0.45);

		if (deglib <= 0)
		{
			score = std::numeric_limits<reel>::quiet_NaN();
		}
		else if (seuilConv <= 0.)
		{
			score = std::numeric_limits<reel>::quiet_NaN();
		}
		else if (pValeur < 0. || 1.0 < pValeur)
		{
			score = std::numeric_limits<reel>::quiet_NaN();
		}
			// Source: https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
		else if (std::abs(pValeur) < std::numeric_limits<reel>::min())
		{
			score = 0.;
		}
		else if (std::abs(pValeur - 1.) < std::numeric_limits<reel>::min())
		{
			score = std::numeric_limits<reel>::infinity();
		}
		else
		{
			reel scoreMajorant(searchForUpperBoundScore(pValeur, deglib));
			if (!std::isnan(scoreMajorant))
			{
				score = dichotomicChiSquareQuantileFunction(pValeur, deglib, scoreMajorant, seuilConv);
			}
			else
			{
				score = std::numeric_limits<reel>::quiet_NaN();
			}
		}
		return score;
	}

}