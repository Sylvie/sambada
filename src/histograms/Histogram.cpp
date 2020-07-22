/*************************************************************************
* Copyright (©) 2011-2020 EPFL (Ecole Polytechnique fédérale de Lausanne)
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
#include <algorithm>

sambada::Histogram::Histogram(const std::string& name, const std::vector<reel>& binLimits)
:name(name), binLimits(binLimits), counts(binLimits.size() + 1, 0)
{
	if(!std::is_sorted(this->binLimits.cbegin(), this->binLimits.cend()))
	{
		std::sort(this->binLimits.begin(), this->binLimits.end());
	}
}

void sambada::Histogram::addValue(reel value)
{
	std::vector<reel>::const_iterator upperBound(std::upper_bound(binLimits.cbegin(), binLimits.cend(), value));
	int distance(std::distance(binLimits.cbegin(), upperBound));
	++counts[distance];
}

const std::string& sambada::Histogram::getName() const
{
	return name;
}

const std::vector<sambada::reel>& sambada::Histogram::getBinLimits() const
{
	return binLimits;
}

const std::vector<int>& sambada::Histogram::getCounts() const
{
	return counts;
}
