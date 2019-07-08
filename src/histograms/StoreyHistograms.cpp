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

#include "StoreyHistograms.hpp"
#include "common/Probability.hpp"

#include <cmath>
#include <limits>

sambada::StoreyHistograms::StoreyHistograms(int dimensionMax, sambada::reel scoreMin)
		: dimensionMax(dimensionMax), scoreMin(scoreMin), numScoreTypes(6), nbPvalStorey(96)
{
	initPValuesAndScoreThresholds();
	initHistograms();
}

void sambada::StoreyHistograms::addValue(ScoreType scoreType, int dimension, reel value)
{
	histograms[(size_t) scoreType].addValue(dimension, value);
}

void sambada::StoreyHistograms::initPValuesAndScoreThresholds()
{
	sambada::reel precision(std::sqrt(std::numeric_limits<sambada::reel>::epsilon()));

	for (int i(nbPvalStorey - 1); i > 0; --i)
	{
		pValues.push_back(0.01 * i);
		scoreThresholds.push_back(sambada::probability::chiSquareQuantileFunction(1. - 0.01 * i, 1, precision));
	}
}

void sambada::StoreyHistograms::initHistograms()
{
	std::vector<std::string> names({"G", "GOrphelins", "GPop", "Wald", "WaldOrphelins", "WaldPop"});

	for (size_t i(0); i < (size_t) numScoreTypes; ++i)
	{
		histograms.push_back(sambada::GroupHistograms(dimensionMax + 1, names[i], scoreThresholds));
	}
}


const sambada::GroupHistograms& sambada::StoreyHistograms::getHistograms(ScoreType scoreType) const
{
	if (scoreType < ScoreType::G)
	{
		scoreType = ScoreType::G;
	}
	else if (scoreType > ScoreType::WaldPop)
	{
		scoreType = ScoreType::WaldPop;
	}
	return histograms[(size_t) scoreType];
}


const std::vector<sambada::reel>& sambada::StoreyHistograms::getPValues() const
{
	return pValues;
}

const std::vector<sambada::reel>& sambada::StoreyHistograms::getScoreThresholds() const
{
	return scoreThresholds;
}
