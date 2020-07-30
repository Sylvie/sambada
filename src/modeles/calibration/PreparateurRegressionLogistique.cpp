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

#include "PreparateurRegressionLogistique.hpp"

void sambada::PreparateurRegressionLogistique::setNombrePoints(sambada::TableauNoir& tableauNoir, int nombrePoints) const
{
	tableauNoir.nbPoints = nombrePoints;

	tableauNoir.X.resize(nombrePoints, tableauNoir.nbParametres);
	tableauNoir.Y.resize(nombrePoints, 1);
	tableauNoir.Xb.resize(nombrePoints, 1);
	tableauNoir.nouv_Xb.resize(nombrePoints, 1);
	tableauNoir.exp_Xb.resize(nombrePoints, 1);
	tableauNoir.pi_hat.resize(nombrePoints, 1);
	tableauNoir.interm.resize(nombrePoints, 1);
	tableauNoir.intermScores.resize(nombrePoints, 1);

}

void sambada::PreparateurRegressionLogistique::setNombreParametres(sambada::TableauNoir& tableauNoir, int nombreParametres) const
{
	tableauNoir.nbParametres = nombreParametres;

	tableauNoir.X.resize(tableauNoir.nbPoints, nombreParametres);
	tableauNoir.beta_hat.resize(nombreParametres, 1);
	tableauNoir.nouv_beta_hat.resize(nombreParametres, 1);
	tableauNoir.diff_beta_hat.resize(nombreParametres, 1);
	tableauNoir.scores.resize(nombreParametres, 1);
	tableauNoir.J_info.resize(nombreParametres, nombreParametres);
	tableauNoir.inv_J_info.resize(nombreParametres, nombreParametres);
}
