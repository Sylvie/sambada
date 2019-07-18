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

#include "StoreyHistogramsStreamWriter.hpp"
#include <limits>

namespace sambada {
	std::ostream& StoreyHistogramsStreamWriter::write(const StoreyHistograms& storeyHistograms, std::ostream& output, bool writePopHistograms, char divider)
	{
		std::string separator(&divider, 1);

		std::vector<reel> pValues(storeyHistograms.getPValues());
		std::vector<reel> scoreThresholds(storeyHistograms.getScoreThresholds());

		output << "P-valeurs" << separator;
		for (auto pValue(pValues.begin()); pValue != pValues.end(); ++pValue)
		{
			if (pValue != pValues.begin())
			{
				output << separator;
			}
			output << *pValue;
		}
		output << separator << 0;
		output << std::endl;

		output << "Scores" << separator;
		for (auto score(scoreThresholds.begin()); score != scoreThresholds.end(); ++score)
		{
			if (score != scoreThresholds.begin())
			{
				output << separator;
			}
			output << *score;
		}
		output << separator << std::numeric_limits<reel>::infinity();
		output << std::endl;


		int dimensionMax(storeyHistograms.getDimensionMax());

		std::vector<StoreyHistograms::ScoreType> scoreTypes({StoreyHistograms::ScoreType::G,
		                                                     StoreyHistograms::ScoreType::GOrphelins,
		                                                     StoreyHistograms::ScoreType::Wald,
		                                                     StoreyHistograms::ScoreType::WaldOrphelins});

		for (size_t dimension(1); dimension <= (size_t) dimensionMax; ++dimension)
		{
			for (auto scoreType(scoreTypes.begin()); scoreType != scoreTypes.end(); ++scoreType)
			{
				Histogram histogram(storeyHistograms.getHistograms(*scoreType).getHistograms()[dimension]);

				output << histogram.getName() << separator;

				std::vector<int> counts(histogram.getCounts());
				for (auto count(counts.begin()); count != counts.end(); ++count)
				{
					if (count != counts.begin())
					{
						output << separator;
					}
					output << *count;
				}

				output << std::endl;
			}
		}

		if (writePopHistograms)
		{
			std::vector<StoreyHistograms::ScoreType> popScoreTypes({StoreyHistograms::ScoreType::GPop, StoreyHistograms::ScoreType::WaldPop});

			for (auto scoreType(popScoreTypes.begin()); scoreType != popScoreTypes.end(); ++scoreType)
			{
				Histogram histogram(storeyHistograms.getHistograms(*scoreType).getHistograms()[dimensionMax]);

				output << storeyHistograms.getHistograms(*scoreType).getGroupName() << separator;

				std::vector<int> counts(histogram.getCounts());
				for (auto count(counts.begin()); count != counts.end(); ++count)
				{
					if (count != counts.begin())
					{
						output << separator;
					}
					output << *count;
				}

				output << std::endl;
			}
		}

		return output;
	}
}