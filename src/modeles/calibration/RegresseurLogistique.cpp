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

#include "RegresseurLogistique.hpp"
#include "stat.h"
#include "optimize.h"

sambada::RegresseurLogistique::RegresseurLogistique(const ConfigurationRegresseurLogistique& configuration)
:configuration(configuration)
{}

// Liste des erreurs:
// 1: divergence exponentielle
// 2: J_info singulière
// 3: divergence beta
// 4: max iterations atteint
// 5: marqueur constant
// 6: modèle avec parents non-significatifs
int sambada::RegresseurLogistique::calculeRegression(sambada::TableauNoir& tableauNoir) const
{

	// Test de convergence
	bool continueCalcul(true), singularMatrix(false), divergentCalculation(false);
	int nbIterations(0), typeErreur(0);
	int nbParam(tableauNoir.X.cols());

	tableauNoir.beta_hat = 0;

	// Iteration
	while (continueCalcul && !singularMatrix && !divergentCalculation && (nbIterations < configuration.limiteIterations))
	{
		// Calcul pi
		tableauNoir.nouv_Xb = tableauNoir.X * tableauNoir.beta_hat; // Test avant l'exp

		if (scythe::max(tableauNoir.nouv_Xb) > configuration.limiteExp)
		{
			continueCalcul = false;
			tableauNoir.composantEfron = scythe::sum((tableauNoir.intermScores % tableauNoir.intermScores)(0, 0, tableauNoir.taille - 1, 0));
			tableauNoir.logLikelihood = scythe::sum((tableauNoir.Y % (tableauNoir.Xb) - scythe::log(1. + tableauNoir.exp_Xb))(0, 0, tableauNoir.taille - 1, 0));
			typeErreur = 1;

		}
		else
		{
			++nbIterations;

			// Calcul pi
			tableauNoir.Xb = tableauNoir.nouv_Xb;
			tableauNoir.exp_Xb = scythe::exp(tableauNoir.Xb);

			tableauNoir.pi_hat = tableauNoir.exp_Xb / (1 + tableauNoir.exp_Xb);

			// Calcul ni * pi * (1 - pi)
			tableauNoir.interm = tableauNoir.pi_hat % (1 - tableauNoir.pi_hat);

			// Calcul des scores U
			tableauNoir.intermScores = (tableauNoir.Y - tableauNoir.pi_hat);
			for (int k(0); k < nbParam; ++k)
			{
				tableauNoir.scores(k, 0) = sum(tableauNoir.intermScores % tableauNoir.X(scythe::_, k));
			}

			// Calcul de J, qui est symétrique
			for (int k(0); k < nbParam; ++k)
			{
				tableauNoir.J_info(k, k) = sum(tableauNoir.interm % tableauNoir.X(scythe::_, k) % tableauNoir.X(scythe::_, k));
				for (int l(k + 1); l < nbParam; ++l)
				{
					tableauNoir.J_info(k, l) = sum(tableauNoir.interm % tableauNoir.X(scythe::_, k) % tableauNoir.X(scythe::_, l));
					tableauNoir.J_info(l, k) = tableauNoir.J_info(k, l);
				}
			}

			/*if ( det(J_info)<=0)
			 {
			 singularMatrix = true;
			 }
			 else */
			{
				// Calcul de l'inverse de J
				try
				{
					tableauNoir.inv_J_info = scythe::invpd(tableauNoir.J_info);
				}
				catch (scythe::scythe_exception& error)
				{
					singularMatrix = true;
					continueCalcul = false;
					typeErreur = 2;
				}

				if (!singularMatrix)
				{
					// Mise à jour de beta_hat
					tableauNoir.nouv_beta_hat = tableauNoir.beta_hat + tableauNoir.inv_J_info * tableauNoir.scores;

					for (int l(0); l < nbParam; ++l)
					{
						if (abs(tableauNoir.nouv_beta_hat(l, 0)) > configuration.limiteNaN)
						{
							continueCalcul = false;
							divergentCalculation = true;
							typeErreur = 3;
							break;
						}
					}

					if (continueCalcul)
					{
						// Test de convergence
						tableauNoir.diff_beta_hat = tableauNoir.nouv_beta_hat - tableauNoir.beta_hat;
						continueCalcul = false;
						for (int l(0); l < nbParam; ++l)
						{
							if (std::abs(tableauNoir.diff_beta_hat(l, 0)) > configuration.critereConvergence * std::max(configuration.epsilon, (reel)abs(tableauNoir.beta_hat(l, 0))))
							{
								continueCalcul = true;
								break;
							}
						}

						tableauNoir.beta_hat = tableauNoir.nouv_beta_hat;
					}
				}
			}
			// Si le calcul est terminé, on calcule l'indice d'Effron
			if (!continueCalcul)
			{
				tableauNoir.composantEfron = scythe::sum((tableauNoir.intermScores % tableauNoir.intermScores)(0, 0, tableauNoir.taille - 1, 0));
				tableauNoir.logLikelihood = sum((tableauNoir.Y % (tableauNoir.Xb) - scythe::log(1. + tableauNoir.exp_Xb))(0, 0, tableauNoir.taille - 1, 0));
			}
		}
	}
	if (nbIterations == configuration.limiteIterations)
	{
		tableauNoir.composantEfron = scythe::sum((tableauNoir.intermScores % tableauNoir.intermScores)(0, 0, tableauNoir.taille - 1, 0));
		tableauNoir.logLikelihood = scythe::sum((tableauNoir.Y % (tableauNoir.Xb) - scythe::log(1. + tableauNoir.exp_Xb))(0, 0, tableauNoir.taille - 1, 0));
		typeErreur = 4;
	}
	return typeErreur;
}
