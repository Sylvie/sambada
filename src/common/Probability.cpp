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

		else if (pValeur > limiteDomaine)
		{
			reel residu(scythe::pchisq(score, deglib) - pValeur);
			int compteur(0), limiteIter(1000);
			do
			{
				score = score - (scythe::pchisq(score, deglib) - pValeur) / scythe::dchisq(score, deglib);
				residu = scythe::pchisq(score, deglib);
				++compteur;
				//cout << x << " " << chisq.prob(x)+chisq.valeur <<" " << valeur << endl;
			} while ((std::abs(residu) > seuilConv) && (compteur < limiteIter));
		}
		else
		{
			reel p1(0.), p2(limiteDomaine), p3(0.); // p1 < p, p2 >p;
			reel q1(0.), q2(0.37), q3((q1 + q2) / 2);
			reel residu(0);
			int compteur(0), limiteIter(1000);
			//cout << pValeur << ":" << endl;
			do
			{
				p3 = scythe::pchisq(q3, deglib);
				//cout << "q->p " << q3 << " " << p3 << endl;
				residu = p3 - pValeur;
				if (residu >= 0)    // p3 >= p
				{
					q2 = q3;
					p2 = p3;
				}
				else // p >p3
				{
					q1 = q3;
					p1 = p3;
				}
				q3 = (q1 + q2) / 2;

				++compteur;
			} while ((std::abs(residu) > seuilConv) && (compteur < limiteIter));
			score = q3;
		}
		return score;
	}

}