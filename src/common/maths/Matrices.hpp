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

#ifndef SAMBADA_MATRICES_HPP
#define SAMBADA_MATRICES_HPP

#include "common/TypesCommuns.hpp"
#include "matrix.h"

namespace sambada {

	typedef scythe::Matrix<reel, scythe::Col, scythe::Concrete> MatriceReels;
	typedef scythe::Matrix<bool, scythe::Col, scythe::Concrete> MatriceBools;
	typedef scythe::Matrix<std::string, scythe::Col, scythe::Concrete> MatriceStrings;

}

#endif //SAMBADA_MATRICES_HPP
