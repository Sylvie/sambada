#include "SambadaSpatialAutocorrelationResults.h"
#include "catch.hpp"
//    std::vector <std::string> header = {"Marker", "Env_1", "Loglikelihood", "Gscore", "WaldScore", "NumError",
// "Efron", "McFadden", "McFaddenAdj", "CoxSnell", "Nagelkerke", "AIC", "BIC",
// "Beta_0", "Beta_1"};
// 0 : h=5, e=1, v=4
// 1: h=15, e=2, v=13
//2: h=17, e=3, v=14

void SambadaSpatialAutocorrelationResults::verifieTailles(int nombrePoints, int nombreVariables) const
{
	INFO(colleHeaders());
	verifieTailleHeader(nombreVariables);

	REQUIRE(etiquettes.size() == nombrePoints + 1);
	REQUIRE(valeurs.size() == nombrePoints + 1);

	int tailleEtiquettes = 1;

	for (int i(0); i <= nombrePoints; ++i)
	{
		INFO("Point numéro: " + std::to_string(i));
		CHECK(valeurs[i].size() == nombreVariables);
	}
}

void SambadaSpatialAutocorrelationResults::verifieTailleHeader(int nombreVariables) const
{
	CHECK(header.size() == (nombreVariables + 1));
}

void SambadaSpatialAutocorrelationResults::compare(const SambadaSpatialAutocorrelationResults &autre) const
{
	this->compare(autre, 0);
}

void SambadaSpatialAutocorrelationResults::comparePValeurs(const SambadaSpatialAutocorrelationResults &autre) const
{
	this->compare(autre, 0.25);
}

void SambadaSpatialAutocorrelationResults::compare(const SambadaSpatialAutocorrelationResults &autre, double marge) const
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
	int nombreLignes(etiquettes.size());
	CHECKED_IF(nombreLignes == autre.etiquettes.size())
	{
		for (int i(0); i < nombreLignes; ++i)
		{
			INFO("Modèle numéro: " + std::to_string(i));
			CHECK(etiquettes[i] == autre.etiquettes[i]);
		}
	}

	/* Valeurs */
	nombreLignes = valeurs.size();
	CHECKED_IF(nombreLignes == autre.valeurs.size())
	{
		for (int i(0); i < nombreLignes; ++i)
		{
			INFO("Modèle numéro: " + std::to_string(i) + " (" + etiquettes[i] + ")");
			int nombreValeurs(valeurs[i].size());
			CHECKED_IF(nombreValeurs == autre.valeurs[i].size())
			{
				int tailleEtiquette(1);
				for (int j(0); j < nombreValeurs; ++j)
				{
					INFO("Valeur numéro: " + std::to_string(j) + " (" + header[j + tailleEtiquette] + ")");
					CHECK(valeurs[i][j] == Approx(autre.valeurs[i][j]).margin(marge));
				}
			}
		}
	}
}

std::string SambadaSpatialAutocorrelationResults::colleHeaders() const
{
	return colleChaines(header);
}

std::string SambadaSpatialAutocorrelationResults::colleChaines(const std::vector<std::string> &morceaux) const
{
	std::string resultat("");
	for (size_t i(0); i < morceaux.size(); ++i)
	{
		resultat = resultat + morceaux[i] + " ";
	}
	return resultat;
}


