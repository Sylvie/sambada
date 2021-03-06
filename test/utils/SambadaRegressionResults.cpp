#include "SambadaRegressionResults.hpp"
#include "catch.hpp"
//    std::vector <std::string> header = {"Marker", "Env_1", "Loglikelihood", "Gscore", "WaldScore", "NumError",
// "Efron", "McFadden", "McFaddenAdj", "CoxSnell", "Nagelkerke", "AIC", "BIC",
// "Beta_0", "Beta_1"};
// 0 : h=5, e=1, v=4
// 1: h=15, e=2, v=13
//2: h=17, e=3, v=14

void SambadaRegressionResults::verifieTailles(bool hasHeader, int dimension, int nombreModeles, bool hasPop, bool hasGWR) const
{
	if (hasHeader)
	{
		INFO(colleHeaders());
		verifieTailleHeader(dimension, hasPop);
	}

	REQUIRE(etiquettes.size() == nombreModeles);
	REQUIRE(valeurs.size() == nombreModeles);

	int tailleEtiquettes = dimension + 1;
	int tailleValeurs = 4;

	if (dimension > 0)
	{
		tailleValeurs = dimension + 12;
		if (hasPop)
		{
			tailleValeurs += 2;
		}

		if (hasGWR)
		{
			tailleValeurs += (9 + 2 * dimension);
		}
	}

	for (int i(0); i < nombreModeles; ++i)
	{
		INFO("Modèle numéro: " + std::to_string(i));
		CHECK(etiquettes[i].size() == tailleEtiquettes);
		CHECK(valeurs[i].size() == tailleValeurs);
	}
}

void SambadaRegressionResults::verifieTailleHeader(int dimension, bool hasPop) const
{
	if (dimension == 0)
	{
		CHECK(header.size() == 5);
	}
	else
	{
		if (!hasPop)
		{
			CHECK(header.size() == (2 * dimension + 13));
		}
		else
		{
			CHECK(header.size() == (2 * dimension + 15));
		}
	}
}

void SambadaRegressionResults::compare(const SambadaRegressionResults& autre, double epsilon) const
{

	/* Header */
	int tailleHeader(header.size());
	CHECKED_IF(tailleHeader == autre.header.size())
	{
		for (int i(0); i < tailleHeader; ++i)
		{
			INFO("Header numéro: " + std::to_string(i));
			CHECK(header[i] == autre.header[i]);
		}
	}

	/* Etiquettes */
	int nombreModeles(etiquettes.size());
	CHECKED_IF(nombreModeles == autre.etiquettes.size())
	{
		for (int i(0); i < nombreModeles; ++i)
		{
			INFO("Modèle numéro: " + std::to_string(i));
			int tailleEtiquette(etiquettes[i].size());
			CHECKED_IF(tailleEtiquette == autre.etiquettes[i].size())
			{
				for (int j(0); j < tailleEtiquette; ++j)
				{
					INFO("Étiquette numéro: " + std::to_string(j));
					CHECK(etiquettes[i][j] == autre.etiquettes[i][j]);
				}
			}
		}
	}

	/* Valeurs */
	nombreModeles = valeurs.size();
	CHECKED_IF(nombreModeles == autre.valeurs.size())
	{
		for (int i(0); i < nombreModeles; ++i)
		{
			INFO("Modèle numéro: " + std::to_string(i) + " (" + colleChaines(etiquettes[i]) + ")");
			int nombreValeurs(valeurs[i].size());
			CHECKED_IF(nombreValeurs == autre.valeurs[i].size())
			{
				int tailleEtiquette(etiquettes[i].size());
				for (int j(0); j < nombreValeurs; ++j)
				{
					INFO("Valeur numéro: " + std::to_string(j) + " (" + (j + tailleEtiquette < tailleHeader ? header[j + tailleEtiquette] : "?") + ")");
					CHECK(valeurs[i][j] == Approx(autre.valeurs[i][j]).epsilon(epsilon));
				}
			}
		}
	}
}

std::string SambadaRegressionResults::colleHeaders() const
{
	return colleChaines(header);
}

std::string SambadaRegressionResults::colleChaines(const std::vector<std::string>& morceaux) const
{
	std::string resultat("");
	for (size_t i(0); i < morceaux.size(); ++i)
	{
		resultat = resultat + morceaux[i] + " ";
	}
	return resultat;
}
