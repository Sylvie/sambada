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

#ifndef SAMBADA_GROUPHISTOGRAMS_HPP
#define SAMBADA_GROUPHISTOGRAMS_HPP

#include "histograms/Histogram.hpp"
#include <string>
#include <vector>

namespace sambada {
	class GroupHistograms
	{
	public:
		GroupHistograms(int groupSize, const std::string& groupName, const std::vector<reel>& binLimits);

		void addValue(int index, reel value);

		const std::string& getGroupName() const;

		const std::vector<sambada::Histogram>& getHistograms() const;

	private:
		std::string name;
		std::vector<sambada::Histogram> histograms;
	};
}

#endif //SAMBADA_GROUPHISTOGRAMS_HPP
