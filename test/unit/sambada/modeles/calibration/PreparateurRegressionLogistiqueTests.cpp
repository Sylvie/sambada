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

#include "catch.hpp"
#include "modeles/calibration/PreparateurRegressionLogistique.hpp"

TEST_CASE("Test that PreparateurRegressionLogistique sets up the TableauNoir correctly", "[preparateur-regression-logistique-unit]")
{
	sambada::TableauNoir tableauNoir;
	sambada::PreparateurRegressionLogistique preparateur;

	SECTION("Test that the initial TableauNoir has no points nor parameters")
	{
		CHECK(tableauNoir.nbPoints == 0);
		CHECK(tableauNoir.nbParametres == 0);

		/* Matrices nbPoints x _ */
		CHECK(tableauNoir.X.rows() == 0);
		CHECK(tableauNoir.Y.rows() == 0);
		CHECK(tableauNoir.Xb.rows() == 0);
		CHECK(tableauNoir.nouv_Xb.rows() == 0);
		CHECK(tableauNoir.exp_Xb.rows() == 0);
		CHECK(tableauNoir.pi_hat.rows() == 0);
		CHECK(tableauNoir.interm.rows() == 0);
		CHECK(tableauNoir.intermScores.rows() == 0);

		CHECK(tableauNoir.X.cols() == 0);
		CHECK(tableauNoir.Y.cols() == 0);
		CHECK(tableauNoir.Xb.cols() == 0);
		CHECK(tableauNoir.nouv_Xb.cols() == 0);
		CHECK(tableauNoir.exp_Xb.cols() == 0);
		CHECK(tableauNoir.pi_hat.cols() == 0);
		CHECK(tableauNoir.interm.cols() == 0);
		CHECK(tableauNoir.intermScores.cols() == 0);

		/* Matrices nbParams x _ */
		CHECK(tableauNoir.beta_hat.rows() == 0);
		CHECK(tableauNoir.nouv_beta_hat.rows() == 0);
		CHECK(tableauNoir.diff_beta_hat.rows() == 0);
		CHECK(tableauNoir.scores.rows() == 0);
		CHECK(tableauNoir.J_info.rows() == 0);
		CHECK(tableauNoir.inv_J_info.rows() == 0);

		CHECK(tableauNoir.beta_hat.cols() == 0);
		CHECK(tableauNoir.nouv_beta_hat.cols() == 0);
		CHECK(tableauNoir.diff_beta_hat.cols() == 0);
		CHECK(tableauNoir.scores.cols() == 0);
		CHECK(tableauNoir.J_info.cols() == 0);
		CHECK(tableauNoir.inv_J_info.cols() == 0);
	}

	SECTION("Test that PraparateurRegressionLogistique sets the number of points correctly")
	{
		const int nombrePoints(5);

		preparateur.setNombrePoints(tableauNoir, nombrePoints);

		CHECK(tableauNoir.nbPoints == nombrePoints);
		CHECK(tableauNoir.nbParametres == 0);

		CHECK(tableauNoir.X.rows() == nombrePoints);
		CHECK(tableauNoir.Y.rows() == nombrePoints);
		CHECK(tableauNoir.Xb.rows() == nombrePoints);
		CHECK(tableauNoir.nouv_Xb.rows() == nombrePoints);
		CHECK(tableauNoir.exp_Xb.rows() == nombrePoints);
		CHECK(tableauNoir.pi_hat.rows() == nombrePoints);
		CHECK(tableauNoir.interm.rows() == nombrePoints);
		CHECK(tableauNoir.intermScores.rows() == nombrePoints);

		CHECK(tableauNoir.X.cols() == 0);
		CHECK(tableauNoir.Y.cols() == 1);
		CHECK(tableauNoir.Xb.cols() == 1);
		CHECK(tableauNoir.nouv_Xb.cols() == 1);
		CHECK(tableauNoir.exp_Xb.cols() == 1);
		CHECK(tableauNoir.pi_hat.cols() == 1);
		CHECK(tableauNoir.interm.cols() == 1);
		CHECK(tableauNoir.intermScores.cols() == 1);

		CHECK(tableauNoir.beta_hat.rows() == 0);
		CHECK(tableauNoir.nouv_beta_hat.rows() == 0);
		CHECK(tableauNoir.diff_beta_hat.rows() == 0);
		CHECK(tableauNoir.scores.rows() == 0);
		CHECK(tableauNoir.J_info.rows() == 0);
		CHECK(tableauNoir.inv_J_info.rows() == 0);

		CHECK(tableauNoir.beta_hat.cols() == 0);
		CHECK(tableauNoir.nouv_beta_hat.cols() == 0);
		CHECK(tableauNoir.diff_beta_hat.cols() == 0);
		CHECK(tableauNoir.scores.cols() == 0);
		CHECK(tableauNoir.J_info.cols() == 0);
		CHECK(tableauNoir.inv_J_info.cols() == 0);
	}

	SECTION("Test that PraparateurRegressionLogistique sets the number of parameters correctly")
	{
		const int nombreParametres(3);

		preparateur.setNombreParametres(tableauNoir, nombreParametres);

		CHECK(tableauNoir.nbPoints == 0);
		CHECK(tableauNoir.nbParametres == nombreParametres);

		CHECK(tableauNoir.X.rows() == 0);
		CHECK(tableauNoir.Y.rows() == 0);
		CHECK(tableauNoir.Xb.rows() == 0);
		CHECK(tableauNoir.nouv_Xb.rows() == 0);
		CHECK(tableauNoir.exp_Xb.rows() == 0);
		CHECK(tableauNoir.pi_hat.rows() == 0);
		CHECK(tableauNoir.interm.rows() == 0);
		CHECK(tableauNoir.intermScores.rows() == 0);

		CHECK(tableauNoir.X.cols() == nombreParametres);
		CHECK(tableauNoir.Y.cols() == 0);
		CHECK(tableauNoir.Xb.cols() == 0);
		CHECK(tableauNoir.nouv_Xb.cols() == 0);
		CHECK(tableauNoir.exp_Xb.cols() == 0);
		CHECK(tableauNoir.pi_hat.cols() == 0);
		CHECK(tableauNoir.interm.cols() == 0);
		CHECK(tableauNoir.intermScores.cols() == 0);

		CHECK(tableauNoir.beta_hat.rows() == nombreParametres);
		CHECK(tableauNoir.nouv_beta_hat.rows() == nombreParametres);
		CHECK(tableauNoir.diff_beta_hat.rows() == nombreParametres);
		CHECK(tableauNoir.scores.rows() == nombreParametres);
		CHECK(tableauNoir.J_info.rows() == nombreParametres);
		CHECK(tableauNoir.inv_J_info.rows() == nombreParametres);

		CHECK(tableauNoir.beta_hat.cols() == 1);
		CHECK(tableauNoir.nouv_beta_hat.cols() == 1);
		CHECK(tableauNoir.diff_beta_hat.cols() == 1);
		CHECK(tableauNoir.scores.cols() == 1);
		CHECK(tableauNoir.J_info.cols() == nombreParametres);
		CHECK(tableauNoir.inv_J_info.cols() == nombreParametres);
	}

	SECTION("Test that PraparateurRegressionLogistique sets the number of points and then the number of parameters correctly")
	{
		const int nombrePoints(12);
		const int nombreParametres(5);

		preparateur.setNombrePoints(tableauNoir, nombrePoints);
		preparateur.setNombreParametres(tableauNoir, nombreParametres);

		CHECK(tableauNoir.nbPoints == nombrePoints);
		CHECK(tableauNoir.nbParametres == nombreParametres);

		CHECK(tableauNoir.X.rows() == nombrePoints);
		CHECK(tableauNoir.Y.rows() == nombrePoints);
		CHECK(tableauNoir.Xb.rows() == nombrePoints);
		CHECK(tableauNoir.nouv_Xb.rows() == nombrePoints);
		CHECK(tableauNoir.exp_Xb.rows() == nombrePoints);
		CHECK(tableauNoir.pi_hat.rows() == nombrePoints);
		CHECK(tableauNoir.interm.rows() == nombrePoints);
		CHECK(tableauNoir.intermScores.rows() == nombrePoints);

		CHECK(tableauNoir.X.cols() == nombreParametres);
		CHECK(tableauNoir.Y.cols() == 1);
		CHECK(tableauNoir.Xb.cols() == 1);
		CHECK(tableauNoir.nouv_Xb.cols() == 1);
		CHECK(tableauNoir.exp_Xb.cols() == 1);
		CHECK(tableauNoir.pi_hat.cols() == 1);
		CHECK(tableauNoir.interm.cols() == 1);
		CHECK(tableauNoir.intermScores.cols() == 1);

		CHECK(tableauNoir.beta_hat.rows() == nombreParametres);
		CHECK(tableauNoir.nouv_beta_hat.rows() == nombreParametres);
		CHECK(tableauNoir.diff_beta_hat.rows() == nombreParametres);
		CHECK(tableauNoir.scores.rows() == nombreParametres);
		CHECK(tableauNoir.J_info.rows() == nombreParametres);
		CHECK(tableauNoir.inv_J_info.rows() == nombreParametres);

		CHECK(tableauNoir.beta_hat.cols() == 1);
		CHECK(tableauNoir.nouv_beta_hat.cols() == 1);
		CHECK(tableauNoir.diff_beta_hat.cols() == 1);
		CHECK(tableauNoir.scores.cols() == 1);
		CHECK(tableauNoir.J_info.cols() == nombreParametres);
		CHECK(tableauNoir.inv_J_info.cols() == nombreParametres);
	}

	SECTION("Test that PraparateurRegressionLogistique sets the number of parameters and then the number of points correctly")
	{
		const int nombrePoints(7);
		const int nombreParametres(4);

		preparateur.setNombreParametres(tableauNoir, nombreParametres);
		preparateur.setNombrePoints(tableauNoir, nombrePoints);

		CHECK(tableauNoir.nbPoints == nombrePoints);
		CHECK(tableauNoir.nbParametres == nombreParametres);

		CHECK(tableauNoir.X.rows() == nombrePoints);
		CHECK(tableauNoir.Y.rows() == nombrePoints);
		CHECK(tableauNoir.Xb.rows() == nombrePoints);
		CHECK(tableauNoir.nouv_Xb.rows() == nombrePoints);
		CHECK(tableauNoir.exp_Xb.rows() == nombrePoints);
		CHECK(tableauNoir.pi_hat.rows() == nombrePoints);
		CHECK(tableauNoir.interm.rows() == nombrePoints);
		CHECK(tableauNoir.intermScores.rows() == nombrePoints);

		CHECK(tableauNoir.X.cols() == nombreParametres);
		CHECK(tableauNoir.Y.cols() == 1);
		CHECK(tableauNoir.Xb.cols() == 1);
		CHECK(tableauNoir.nouv_Xb.cols() == 1);
		CHECK(tableauNoir.exp_Xb.cols() == 1);
		CHECK(tableauNoir.pi_hat.cols() == 1);
		CHECK(tableauNoir.interm.cols() == 1);
		CHECK(tableauNoir.intermScores.cols() == 1);

		CHECK(tableauNoir.beta_hat.rows() == nombreParametres);
		CHECK(tableauNoir.nouv_beta_hat.rows() == nombreParametres);
		CHECK(tableauNoir.diff_beta_hat.rows() == nombreParametres);
		CHECK(tableauNoir.scores.rows() == nombreParametres);
		CHECK(tableauNoir.J_info.rows() == nombreParametres);
		CHECK(tableauNoir.inv_J_info.rows() == nombreParametres);

		CHECK(tableauNoir.beta_hat.cols() == 1);
		CHECK(tableauNoir.nouv_beta_hat.cols() == 1);
		CHECK(tableauNoir.diff_beta_hat.cols() == 1);
		CHECK(tableauNoir.scores.cols() == 1);
		CHECK(tableauNoir.J_info.cols() == nombreParametres);
		CHECK(tableauNoir.inv_J_info.cols() == nombreParametres);
	}
}