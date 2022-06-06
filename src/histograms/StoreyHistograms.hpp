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

#ifndef SAMBADA_STOREYHISTOGRAMS_HPP
#define SAMBADA_STOREYHISTOGRAMS_HPP

#include "common/TypesCommuns.hpp"
#include "histograms/GroupHistograms.hpp"

#include <vector>

namespace sambada {
	class StoreyHistograms
	{
	public:
		enum class ScoreType
		{
			G,
			GOrphelins,
			GPop,
			Wald,
			WaldOrphelins,
			WaldPop
		};

		explicit StoreyHistograms(int dimensionMax);

		void addValue(ScoreType scoreType, int dimension, reel value);

		const GroupHistograms& getHistograms(ScoreType scoreType) const;

		const int nbPvalStorey;

		const int getDimensionMax() const;
		const std::vector<sambada::reel>& getPValues() const;
		const std::vector<sambada::reel>& getScoreThresholds() const;

		void addValidModel(int dimension);
		const std::vector<int>& getNumValidModels() const;

		/*
		 * 	// Définition des caractéristiques nécessaires au calcul de la FDR selon Storey
		typedef struct
		{
			int nbPvalStorey;
			vector<int> nbModelesValides;

			vector<reel> pval;
			vector<reel> seuilScore;

			vector<vector<int>> compteurG, compteurGOrphelins, compteurGPop;
			vector<vector<int>> compteurWald, compteurWaldOrphelins, compteurWaldPop;

			reel scoreMin;
		} donneesFDR;

		 */

	private:
		const int dimensionMax;
		const int numScoreTypes;

		std::vector<GroupHistograms> histograms;
		std::vector<reel> pValues;
		std::vector<reel> scoreThresholds;

		void initPValuesAndScoreThresholds();
		void initHistograms();

		std::vector<int> numValidModels;
	};

}

#endif //SAMBADA_STOREYHISTOGRAMS_HPP
