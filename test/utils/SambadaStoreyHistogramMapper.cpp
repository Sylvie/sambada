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

#include "SambadaStoreyHistogramMapper.hpp"

#include <sstream>

SambadaStoreyHistogram SambadaStoreyHistogramMapper::readStoreyHistogram(std::istream& lecteur) const
{
	SambadaStoreyHistogram histogram;
	histogram.infWasFound = false;

	while (!lecteur.eof() && histogram.header.size() < SambadaStoreyHistogram::nombreLignesHeader)
	{
		std::string etiquette("");
		lecteur >> etiquette >> std::ws;
		histogram.etiquettes.push_back(etiquette);

		std::string header("");
		getline(lecteur, header);

		size_t	position(header.find("inf"));
		if(position != std::string::npos)
		{
			histogram.infWasFound = true;
			header = header.substr(0, position);
		}

		std::istringstream iss(header);
		long double lu(0.);
		std::vector<long double> valeurs;
		while (iss >> lu)
		{
			valeurs.push_back(lu);
		}
		histogram.header.push_back(valeurs);

		lecteur >> std::ws;
	}
	lecteur >> std::ws;

	while (!lecteur.eof())
	{
		std::string etiquette("");
		lecteur >> etiquette >> std::ws;
		histogram.etiquettes.push_back(etiquette);

		if (lecteur.eof())
		{
			break;
		}

		std::string concatenatedValues("");
		getline(lecteur, concatenatedValues);

		std::vector<int> values(0);
		std::istringstream iss(concatenatedValues);
		int value;
		while (iss >> value)
		{
			values.push_back(value);
		}
		histogram.valeurs.push_back(values);

		lecteur >> std::ws;
	}

	return histogram;
}
