#include "catch.hpp"
#include "modeles/calibration/TableauNoir.hpp"
#include "modeles/calibration/RegresseurLogistique.hpp"
#include <limits>
#include "optimize.h"

void redimensionneTableau(sambada::TableauNoir& tableauNoir, int nbParam) {
	tableauNoir.beta_hat.resize(nbParam, 1);
	tableauNoir.nouv_beta_hat.resize(nbParam, 1);
	tableauNoir.diff_beta_hat.resize(nbParam, 1);
	tableauNoir.scores.resize(nbParam, 1);
	tableauNoir.J_info.resize(nbParam, nbParam);
	tableauNoir.inv_J_info.resize(nbParam, nbParam);
	tableauNoir.X.resize(tableauNoir.nbPoints, nbParam);
}

TEST_CASE("Test that RegresseurLogistique computes models correctly", "[regresseur-logistique-unit]")
{
	sambada::TableauNoir tableauNoir;
	tableauNoir.nbPoints = 804;
	tableauNoir.taille = 804;

	tableauNoir.Y.resize(tableauNoir.nbPoints, 1);
	tableauNoir.Xb.resize(tableauNoir.nbPoints, 1);
	tableauNoir.nouv_Xb.resize(tableauNoir.nbPoints, 1);
	tableauNoir.exp_Xb.resize(tableauNoir.nbPoints, 1);
	tableauNoir.pi_hat.resize(tableauNoir.nbPoints, 1);
	tableauNoir.interm.resize(tableauNoir.nbPoints, 1);
	tableauNoir.intermScores.resize(tableauNoir.nbPoints, 1);

	std::vector<std::string> nomsMarqueurs({"Hapmap41762-BTA-117570_AA", "Hapmap41762-BTA-117570_AG", "Hapmap41762-BTA-117570_GG", "Hapmap28985-BTA-73836_CC", "Hapmap28985-BTA-73836_CG", "Hapmap28985-BTA-73836_GG"});
	std::vector<int> taillesMarqueurs({804, 804, 804, 200, 400, 600});
	std::vector<std::vector<int> > marq({
			                                    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
			                                    {1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			                                    {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
			                                    {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
			                                    {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
			                                    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0}
	                                    });

	std::vector<std::string> nomsEnvs({"bio3", "prec7", "tmax10"});
	std::vector<std::vector<sambada::reel>> dataEnv({
			                                                {89,  90,  89,  85,  85,  85,  86,  86,  81,  81,  72,  72,  73,  81,  81,  81,  81,  81,  81,  81,  83,  82,  82,  82,  82,  81,  77,  77,  77,  76,  79,  79,  79,  79,  81,  76,  77,  77,  76,  77,  77,  76,  76,  82,  84,  84,  77,  76,  76,  77,  77,  79,  79,  79,  79,  80,  80,  79,  79,  79,  79,  79,  79,  79,  79,  79,  82,  82,  82,  82,  82,  82,  84,  84,  84,  85,  85,  85,  83,  83,  80,  80,  80,  80,  80,  84,  83,  83,  83,  83,  83,  83,  83,  83,  87,  87,  87,  89,  89,  89,  89,  89,  89,  88,  88,  88,  88,  88,  85,  85,  85,  88,  88,  88,  88,  88,  88,  88,  88,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  86,  85,  85,  90,  90,  90,  90,  90,  90,  90,  89,  89,  89,  90,  90,  90,  89,  89,  89,  88,  88,  88,  87,  87,  87,  87,  89,  89,  89,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  85,  86,  86,  86,  86,  83,  83,  83,  85,  85,  85,  85,  84,  84,  84,  84,  85,  85,  85,  85,  85,  85,  85,  87,  87,  87,  87,  87, 87, 87, 87, 86, 86, 86, 85, 85, 85, 86, 86, 86, 85, 85, 85, 85, 85, 85, 85, 75, 75, 75, 76, 76, 76, 76, 74, 74, 74, 74, 75, 75, 75, 76, 76, 76, 76, 76, 76, 76, 73, 73, 73, 72, 72, 72, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 82, 82, 82, 82, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 73, 73, 73, 74, 74, 74, 74, 72, 72, 72, 72, 72, 72, 72, 73, 73, 73, 64, 64, 64, 64, 64, 64, 64, 63, 63, 63, 65, 65, 65, 65, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 82, 82, 82, 82, 81, 81, 81, 81, 82, 82, 82, 82, 82, 81, 81, 81, 81, 81, 81, 81, 81, 81, 83, 83, 83, 83, 82, 82, 82, 82, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 74, 74, 74, 74, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 74, 74, 74, 77, 77, 76, 76, 76, 76, 77, 77, 77, 77, 77, 77, 76, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 84, 84, 84, 84, 82, 82, 82, 82, 83, 83, 83, 83, 83, 76, 76, 76, 76, 76, 76, 77, 77, 76, 76, 77, 77, 77, 77, 77, 77, 77, 77, 77, 76, 76, 76, 76, 76, 78, 78, 78, 78, 78, 78, 78, 78, 78, 79, 79, 79, 79, 79, 78, 78, 78, 78, 79, 79, 79, 79, 78, 78, 78, 78, 78, 79, 79, 79, 79, 79, 79, 79, 79, 79, 81, 81, 81, 81, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 84, 84, 84, 84, 84, 84, 84, 84, 77, 77, 76, 77, 77, 77, 77, 79, 79, 79, 80, 79, 79, 79, 80, 80, 80, 79, 79, 79, 79, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 81, 81, 81, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 83, 83, 84, 84, 84, 84, 84, 84, 84, 85, 83, 83, 80, 80, 80, 80, 80, 80, 80, 79, 79, 79, 79, 79, 79, 79, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 81, 81, 81, 81, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 81, 81, 81, 80, 80, 80, 81, 81, 81, 81, 80, 80, 80, 80, 80, 80, 80, 80, 84, 84, 84, 84, 83, 83, 82, 82, 82, 82, 80, 80, 80, 80, 82, 82, 82, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 77, 80, 80, 78, 80, 81, 80, 84, 81, 82, 81, 81, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 82, 82, 82, 83, 83, 83, 84, 84, 84, 84, 84, 84, 84, 84, 84, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 84, 84, 84, 84, 85, 85, 85, 85, 85, 85, 85, 85, 86, 86, 86, 85, 85, 85, 85, 74, 74, 74, 75, 75, 75, 75},
			                                                {11,  27,  14,  95,  95,  95,  35,  28,  74,  74,  123, 123, 126, 91,  91,  91,  100, 100, 100, 68,  47,  47,  47,  47,  47,  24,  165, 165, 158, 158, 94,  94,  94,  94,  77,  152, 150, 150, 157, 173, 173, 172, 172, 71,  74,  74,  138, 126, 126, 140, 140, 154, 149, 149, 149, 150, 150, 142, 142, 137, 137, 137, 152, 152, 152, 152, 92,  92,  92,  92,  92,  87,  77,  67,  67,  70,  70,  70,  75,  75,  126, 121, 120, 90,  101, 84,  89,  89,  121, 127, 127, 127, 133, 133, 14,  14,  14,  12,  12,  12,  11,  11,  11,  17,  17,  17,  17,  17,  47,  47,  47,  38,  38,  38,  38,  39,  39,  39,  39,  58,  58,  58,  58,  58,  58,  58,  41,  41,  41,  41,  39,  39,  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,  30,  30,  30,  14,  14,  14,  12,  12,  12,  14,  14,  14,  14,  14,  14,  14,  95,  95,  95,  95,  95,  52,  52,  52,  52,  54,  54,  54,  50,  50,  50,  50,  50,  50,  50,  50,  63,  63,  63,  64,  64,  64,  64,  64,  64,  64,  64,  26,  26,  26,  26,  25,  25,  25,  38,  38,  38,  38,  36, 36, 36, 36, 36, 36, 36, 29, 29, 29, 28, 28, 28, 29, 29, 29, 30, 30, 30, 30, 70, 70, 70, 75, 75, 75, 75, 72, 72, 72, 72, 71, 71, 71, 143, 143, 143, 143, 143, 143, 143, 114, 114, 114, 107, 107, 107, 106, 106, 106, 106, 105, 105, 105, 110, 110, 110, 85, 85, 85, 85, 90, 90, 90, 90, 67, 67, 67, 67, 74, 74, 74, 74, 74, 135, 135, 135, 139, 139, 139, 139, 130, 130, 130, 119, 119, 119, 123, 126, 126, 126, 87, 87, 87, 89, 89, 89, 89, 88, 88, 88, 88, 88, 88, 88, 87, 87, 87, 87, 94, 94, 94, 94, 94, 91, 91, 100, 70, 70, 70, 70, 68, 68, 68, 68, 74, 74, 74, 74, 74, 73, 73, 73, 73, 56, 56, 56, 56, 56, 47, 47, 47, 47, 50, 50, 50, 47, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 24, 24, 24, 25, 25, 25, 138, 138, 138, 138, 145, 145, 145, 145, 150, 150, 150, 150, 150, 150, 150, 138, 138, 138, 165, 165, 162, 162, 162, 162, 165, 165, 165, 158, 158, 158, 158, 94, 94, 94, 94, 93, 93, 93, 93, 94, 94, 94, 94, 94, 94, 81, 81, 81, 81, 81, 81, 81, 81, 81, 78, 78, 78, 78, 78, 77, 77, 77, 77, 77, 77, 77, 77, 62, 62, 62, 62, 60, 60, 60, 60, 61, 61, 61, 61, 61, 63, 63, 63, 63, 63, 63, 63, 63, 63, 61, 61, 61, 61, 62, 62, 62, 62, 62, 152, 152, 152, 149, 149, 149, 150, 150, 157, 157, 170, 170, 170, 173, 173, 173, 173, 173, 173, 172, 172, 172, 172, 172, 100, 100, 100, 100, 100, 100, 100, 100, 100, 102, 102, 102, 102, 102, 104, 104, 104, 104, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 95, 95, 95, 95, 72, 72, 72, 72, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 73, 73, 73, 73, 75, 75, 75, 74, 138, 138, 126, 140, 139, 139, 139, 154, 154, 149, 150, 148, 148, 148, 134, 134, 134, 142, 137, 137, 152, 108, 108, 108, 108, 108, 105, 105, 105, 105, 107, 107, 107, 107, 107, 111, 111, 111, 90, 90, 90, 92, 92, 92, 92, 92, 87, 87, 87, 87, 81, 81, 81, 77, 77, 67, 67, 60, 60, 60, 70, 75, 75, 116, 116, 116, 116, 116, 116, 116, 130, 130, 130, 130, 130, 130, 130, 126, 126, 126, 121, 121, 121, 121, 120, 120, 120, 109, 109, 109, 109, 113, 113, 113, 113, 121, 121, 121, 121, 121, 123, 123, 123, 123, 123, 125, 125, 125, 125, 129, 129, 129, 90, 90, 90, 99, 99, 99, 99, 101, 101, 101, 100, 100, 100, 100, 100, 84, 84, 92, 92, 89, 89, 89, 89, 89, 89, 115, 115, 115, 115, 113, 113, 113, 113, 115, 115, 115, 115, 115, 124, 124, 124, 124, 121, 121, 121, 121, 127, 133, 133, 133, 126, 126, 126, 144, 144, 144, 144, 139, 150, 134, 111, 113, 129, 101, 92, 74, 50, 74, 74, 76, 76, 76, 76, 55, 55, 55, 55, 55, 55, 55, 55, 55, 50, 50, 50, 61, 61, 61, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 140, 140, 140, 141, 141, 141, 141},
			                                                {236, 262, 254, 286, 286, 286, 262, 262, 272, 275, 286, 286, 290, 287, 287, 287, 284, 284, 284, 278, 273, 273, 273, 273, 273, 271, 291, 291, 289, 290, 291, 291, 291, 291, 287, 302, 303, 303, 309, 299, 299, 301, 301, 291, 282, 282, 297, 296, 296, 299, 299, 298, 298, 298, 298, 298, 298, 302, 302, 304, 304, 304, 303, 303, 303, 303, 290, 290, 290, 290, 290, 291, 288, 280, 280, 285, 285, 285, 285, 285, 294, 301, 304, 293, 289, 291, 288, 288, 288, 296, 296, 296, 294, 294, 240, 240, 240, 232, 232, 232, 236, 236, 236, 210, 210, 210, 210, 210, 291, 291, 291, 282, 282, 282, 282, 280, 280, 280, 280, 216, 216, 216, 216, 216, 216, 216, 288, 288, 288, 288, 292, 292, 262, 262, 262, 262, 262, 262, 262, 244, 244, 244, 256, 256, 256, 254, 254, 254, 262, 262, 262, 259, 259, 259, 259, 255, 255, 255, 286, 286, 286, 286, 286, 286, 286, 286, 286, 284, 284, 284, 281, 281, 281, 281, 277, 277, 277, 277, 259, 259, 259, 262, 262, 262, 262, 262, 262, 262, 262, 261, 261, 261, 261, 261, 261, 261, 261, 261, 261, 261, 263, 263, 263, 263, 262, 262, 262, 258, 258, 258, 262, 262, 262, 261, 261, 261, 260, 260, 260, 260, 288, 288, 288, 290, 290, 290, 290, 294, 294, 294, 294, 291, 291, 291, 279, 279, 279, 279, 279, 279, 279, 280, 280, 280, 279, 279, 279, 264, 264, 264, 264, 266, 266, 266, 260, 260, 260, 288, 288, 288, 288, 281, 281, 281, 281, 269, 269, 269, 269, 272, 272, 272, 272, 275, 305, 305, 305, 309, 309, 309, 309, 294, 294, 294, 286, 286, 286, 286, 290, 290, 290, 265, 265, 265, 260, 260, 260, 260, 258, 258, 258, 264, 264, 264, 264, 286, 286, 286, 286, 287, 287, 287, 287, 287, 287, 287, 284, 281, 281, 281, 281, 278, 278, 278, 278, 277, 277, 277, 277, 277, 282, 282, 282, 282, 257, 257, 257, 257, 257, 273, 273, 273, 273, 268, 268, 268, 273, 269, 269, 269, 269, 269, 269, 269, 270, 270, 270, 270, 271, 271, 271, 268, 268, 268, 321, 321, 321, 321, 311, 311, 311, 311, 306, 306, 306, 315, 315, 315, 315, 321, 321, 321, 291, 291, 291, 291, 291, 291, 291, 291, 291, 289, 289, 289, 290, 292, 292, 292, 292, 292, 292, 292, 292, 291, 291, 291, 291, 291, 291, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 288, 288, 288, 288, 285, 285, 285, 285, 286, 286, 286, 286, 286, 286, 286, 286, 286, 273, 273, 273, 273, 273, 276, 276, 276, 276, 274, 274, 274, 274, 276, 276, 276, 276, 276, 302, 302, 302, 303, 303, 303, 303, 303, 309, 309, 301, 301, 301, 299, 299, 299, 299, 299, 299, 301, 301, 301, 301, 301, 296, 296, 296, 296, 296, 296, 296, 296, 296, 297, 297, 297, 297, 297, 297, 297, 297, 297, 301, 301, 301, 301, 302, 302, 302, 302, 302, 303, 303, 303, 303, 303, 298, 298, 298, 298, 290, 290, 290, 290, 290, 290, 290, 290, 290, 287, 287, 287, 287, 291, 291, 291, 291, 274, 274, 274, 274, 270, 270, 270, 270, 270, 270, 270, 284, 284, 284, 284, 284, 284, 284, 282, 297, 297, 296, 299, 298, 298, 298, 298, 298, 298, 298, 295, 295, 295, 304, 304, 304, 302, 304, 304, 303, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 304, 304, 304, 293, 293, 293, 288, 288, 288, 288, 288, 291, 291, 291, 291, 288, 288, 288, 288, 288, 280, 280, 282, 282, 282, 285, 285, 285, 296, 296, 296, 293, 293, 293, 293, 294, 294, 294, 294, 294, 294, 294, 294, 294, 294, 301, 301, 301, 301, 304, 304, 304, 303, 303, 303, 303, 302, 302, 302, 302, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 297, 297, 297, 293, 293, 293, 291, 291, 291, 291, 289, 289, 289, 290, 290, 290, 290, 290, 291, 291, 288, 288, 288, 288, 304, 304, 304, 304, 290, 290, 290, 290, 291, 291, 291, 291, 290, 290, 290, 290, 290, 281, 281, 281, 281, 288, 288, 288, 288, 296, 294, 294, 294, 296, 296, 296, 292, 292, 292, 292, 298, 298, 304, 304, 302, 297, 289, 288, 275, 260, 275, 275, 276, 276, 276, 276, 252, 252, 252, 252, 248, 248, 248, 248, 248, 260, 260, 260, 268, 268, 268, 266, 266, 266, 263, 263, 263, 264, 264, 264, 265, 265, 265, 265, 265, 265, 265, 265, 261, 261, 261, 264, 264, 264, 264, 265, 265, 265, 265, 262, 262, 262, 262, 265, 265, 265, 259, 259, 259, 259, 314, 314, 314, 313, 313, 313, 313}
	                                                });

	sambada::ConfigurationRegresseurLogistique configuration = {100, std::sqrt(std::numeric_limits<sambada::reel>::epsilon()), 1.e-6, 1000000, std::min((sambada::reel) 700, std::log(std::numeric_limits<sambada::reel>::max() / 2))};
	sambada::RegresseurLogistique regresseurLogistique(configuration);

	SECTION("Test that univariate models computed with all points are correct")
	{
		/* Model:
		 * Hapmap41762-BTA-117570_AA bio3 -269.053878322823 41.9521916771594 35.7445154865424 0 0.0392687721459054 0.0723238896228927 0.0654280506469977 0.0508413732659607 0.0989205722664067 542.107756645645 551.486955184003 -17.3693654948711 0.187236665153414 79.7502514265949
		 * */

		int numeroMarqueur = 0;
		std::vector<int> numerosEnv = {0};
		int nbParam = 2;

		tableauNoir.taille = taillesMarqueurs[numeroMarqueur];
		redimensionneTableau(tableauNoir, nbParam);

		tableauNoir.Y = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, 1, marq[numeroMarqueur].begin());

		std::vector<sambada::reel> vecteurX(tableauNoir.nbPoints, 1.);
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[0]].begin(), dataEnv[numerosEnv[0]].end());
		tableauNoir.X = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, nbParam, vecteurX.begin());

		int codeErreur = regresseurLogistique.calculeRegression(tableauNoir);

		CHECK(codeErreur == 0);

		CHECK(tableauNoir.logLikelihood == Approx(-269.053878322823));
		CHECK(tableauNoir.composantEfron == Approx(79.7502514265949));

		CHECK(tableauNoir.beta_hat(0,0) == Approx(-17.3693654948711));
		CHECK(tableauNoir.beta_hat(1,0) == Approx(0.187236665153414));
	}

	SECTION("Test that bivariate models computed with all points are correct")
	{
		/* Model:
		 * Hapmap41762-BTA-117570_AG prec7 tmax10 -435.120101963281 0.592258461412371 0.58529645333064 0 0.0870423021793632 0.0758606099663791 0.0694889940433464 0.0850180337122862 0.12321142767156 876.240203926561 890.309001734098 -0.946516830318298 -0.0183359381677985 0.0050682175537951 145.477083900859
		 * */

		int numeroMarqueur = 1;
		std::vector<int> numerosEnv = {1, 2};
		int nbParam = 3;

		tableauNoir.taille = taillesMarqueurs[numeroMarqueur];
		redimensionneTableau(tableauNoir, nbParam);

		tableauNoir.Y = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, 1, marq[numeroMarqueur].begin());

		std::vector<sambada::reel> vecteurX(tableauNoir.nbPoints, 1.);
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[0]].begin(), dataEnv[numerosEnv[0]].end());
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[1]].begin(), dataEnv[numerosEnv[1]].end());
		tableauNoir.X = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, nbParam, vecteurX.begin());

		int codeErreur = regresseurLogistique.calculeRegression(tableauNoir);

		CHECK(codeErreur == 0);

		CHECK(tableauNoir.logLikelihood == Approx(-435.120101963281));
		CHECK(tableauNoir.composantEfron == Approx(145.477083900859));

		CHECK(tableauNoir.beta_hat(0,0) == Approx(-0.946516830318298));
		CHECK(tableauNoir.beta_hat(1,0) == Approx(-0.0183359381677985));
		CHECK(tableauNoir.beta_hat(2,0) == Approx(0.0050682175537951));
	}

	SECTION("Test that trivariate models computed with all points are correct")
	{
		/* Model:
		 * Hapmap41762-BTA-117570_GG bio3 prec7 tmax10 -432.491730351773 2.11694184536077 2.11992316011661 0 0.246792564543991 0.00244140337051846 -0.0067847484989576 0.00262954890906708 0.0039848355116093 872.983460703546 891.741857780262 5.25923737406656 -0.0533335260266855 0.0280629365409178 -0.00980981249150528 143.974102367147
		 * */

		int numeroMarqueur = 2;
		std::vector<int> numerosEnv = {0, 1, 2};
		int nbParam = 4;

		tableauNoir.taille = taillesMarqueurs[numeroMarqueur];
		redimensionneTableau(tableauNoir, nbParam);

		tableauNoir.Y = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, 1, marq[numeroMarqueur].begin());

		std::vector<sambada::reel> vecteurX(tableauNoir.nbPoints, 1.);
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[0]].begin(), dataEnv[numerosEnv[0]].end());
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[1]].begin(), dataEnv[numerosEnv[1]].end());
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[2]].begin(), dataEnv[numerosEnv[2]].end());
		tableauNoir.X = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, nbParam, vecteurX.begin());

		int codeErreur = regresseurLogistique.calculeRegression(tableauNoir);

		CHECK(codeErreur == 0);

		CHECK(tableauNoir.logLikelihood == Approx(-432.491730351773));
		CHECK(tableauNoir.composantEfron == Approx(143.974102367147));

		CHECK(tableauNoir.beta_hat(0,0) == Approx(5.25923737406656));
		CHECK(tableauNoir.beta_hat(1,0) == Approx(-0.0533335260266855));
		CHECK(tableauNoir.beta_hat(2,0) == Approx(0.0280629365409178));
		CHECK(tableauNoir.beta_hat(3,0) == Approx(-0.00980981249150528));
	}
	
	SECTION("Test that univariate models computed with a subset of points are correct")
	{
		/* Model:
		 * Hapmap28985-BTA-73836_CC prec7 -128.550369026278 20.0781288372353 17.947151730885 0 0.101238046518319 0.0724374446815909 0.0580063300555597 0.0955159824094222 0.127371630626503 261.100738052556 267.697372785652 1.04986719024447 -0.0144318014628657 44.9201224350144
		 * */

		int numeroMarqueur = 3;
		std::vector<int> numerosEnv = {1};
		int nbParam = 2;

		tableauNoir.taille = taillesMarqueurs[numeroMarqueur];
		redimensionneTableau(tableauNoir, nbParam);

		tableauNoir.Y = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, 1, marq[numeroMarqueur].begin());

		std::vector<sambada::reel> vecteurX(tableauNoir.nbPoints, 1.);
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[0]].begin(), dataEnv[numerosEnv[0]].end());
		tableauNoir.X = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, nbParam, vecteurX.begin());

		if (tableauNoir.taille < tableauNoir.nbPoints)
		{
			tableauNoir.X(tableauNoir.taille, 0, tableauNoir.nbPoints - 1, nbParam - 1) = 0;
			tableauNoir.Y(tableauNoir.taille, 0, tableauNoir.nbPoints - 1, 0) = 0;
		}

		int codeErreur = regresseurLogistique.calculeRegression(tableauNoir);

		CHECK(codeErreur == 0);

		CHECK(tableauNoir.logLikelihood == Approx(-128.550369026278));
		CHECK(tableauNoir.composantEfron == Approx(44.9201224350144));

		CHECK(tableauNoir.beta_hat(0,0) == Approx(1.04986719024447));
		CHECK(tableauNoir.beta_hat(1,0) == Approx(-0.0144318014628657));
	}

	SECTION("Test that bivariate models computed with a subset of points are correct")
	{
		/* Model:
		 * Hapmap28985-BTA-73836_CG bio3 tmax10 -247.006148393722 1.50224583560993 1.44255349116629 0 0.00634220368677763 0.00574960861683171 -0.00632600647836457 0.00711656361751645 0.0100057997402318 500.012296787444 511.986690428768 -5.52868771037956 0.0260439228674401 0.00949205994024362 85.3924668706675
		 * */

		int numeroMarqueur = 4;
		std::vector<int> numerosEnv = {0, 2};
		int nbParam = 3;

		tableauNoir.taille = taillesMarqueurs[numeroMarqueur];
		redimensionneTableau(tableauNoir, nbParam);

		tableauNoir.Y = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, 1, marq[numeroMarqueur].begin());

		std::vector<sambada::reel> vecteurX(tableauNoir.nbPoints, 1.);
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[0]].begin(), dataEnv[numerosEnv[0]].end());
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[1]].begin(), dataEnv[numerosEnv[1]].end());
		tableauNoir.X = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, nbParam, vecteurX.begin());

		if (tableauNoir.taille < tableauNoir.nbPoints)
		{
			tableauNoir.X(tableauNoir.taille, 0, tableauNoir.nbPoints - 1, nbParam - 1) = 0;
			tableauNoir.Y(tableauNoir.taille, 0, tableauNoir.nbPoints - 1, 0) = 0;
		}

		int codeErreur = regresseurLogistique.calculeRegression(tableauNoir);

		CHECK(codeErreur == 0);

		CHECK(tableauNoir.logLikelihood == Approx(-247.006148393722));
		CHECK(tableauNoir.composantEfron == Approx(85.3924668706675));

		CHECK(tableauNoir.beta_hat(0,0) == Approx(-5.52868771037956));
		CHECK(tableauNoir.beta_hat(1,0) == Approx(0.0260439228674401));
		CHECK(tableauNoir.beta_hat(2,0) == Approx(0.00949205994024362));
	}

	SECTION("Test that trivariate models computed with a subset of points are correct")
	{
		/* Model:
		 * Hapmap28985-BTA-73836_GG bio3 prec7 tmax10 -279.092073280418 0.0805375081471311 0.0811458681548369 0 0.249807039353342 0.000144264021073481 -0.0141858561321322 0.000134220171911892 0.000221624293088508 566.184146560835 583.7718651817 17.3793503311373 -0.22955684361421 0.00631684213375558 -0.00257202969179504 89.4029957634644
		 * */

		int numeroMarqueur = 5;
		std::vector<int> numerosEnv = {0, 1, 2};
		int nbParam = 4;

		tableauNoir.taille = taillesMarqueurs[numeroMarqueur];
		redimensionneTableau(tableauNoir, nbParam);

		tableauNoir.Y = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, 1, marq[numeroMarqueur].begin());

		std::vector<sambada::reel> vecteurX(tableauNoir.nbPoints, 1.);
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[0]].begin(), dataEnv[numerosEnv[0]].end());
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[1]].begin(), dataEnv[numerosEnv[1]].end());
		vecteurX.insert(vecteurX.end(), dataEnv[numerosEnv[2]].begin(), dataEnv[numerosEnv[2]].end());
		tableauNoir.X = scythe::Matrix<sambada::reel>(tableauNoir.nbPoints, nbParam, vecteurX.begin());

		if (tableauNoir.taille < tableauNoir.nbPoints)
		{
			tableauNoir.X(tableauNoir.taille, 0, tableauNoir.nbPoints - 1, nbParam - 1) = 0;
			tableauNoir.Y(tableauNoir.taille, 0, tableauNoir.nbPoints - 1, 0) = 0;
		}

		int codeErreur = regresseurLogistique.calculeRegression(tableauNoir);

		CHECK(codeErreur == 0);

		CHECK(tableauNoir.logLikelihood == Approx(-279.092073280418));
		CHECK(tableauNoir.composantEfron == Approx(89.4029957634644));

		CHECK(tableauNoir.beta_hat(0,0) == Approx(17.3793503311373));
		CHECK(tableauNoir.beta_hat(1,0) == Approx(-0.22955684361421));
		CHECK(tableauNoir.beta_hat(2,0) == Approx(0.00631684213375558 ));
		CHECK(tableauNoir.beta_hat(3,0) == Approx(-0.00257202969179504));
	}

}