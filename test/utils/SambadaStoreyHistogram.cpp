#include "SambadaStoreyHistogram.h"
#include "catch.hpp"

#include <iostream>

const size_t SambadaStoreyHistogram::nombreIntervalles;
const size_t SambadaStoreyHistogram::nombreLignesHeader;
constexpr size_t SambadaStoreyHistogram::nombreIntervallesHeaders[nombreLignesHeader];

void SambadaStoreyHistogram::verifieTailles(int nombreLignesValeurs) const
{

	int nombreTotalLignes(nombreLignesValeurs + nombreLignesHeader);

	REQUIRE(etiquettes.size() == nombreTotalLignes);
	REQUIRE(header.size() == nombreLignesHeader);
	REQUIRE(valeurs.size() == nombreLignesValeurs);

	for (int i(0); i < nombreLignesHeader; ++i)
	{
		INFO("Header numéro: " + std::to_string(i));
		CHECK(header[i].size() == SambadaStoreyHistogram::nombreIntervallesHeaders[i]);
	}

	CHECK(infWasFound == true);

	for (int i(0); i < nombreLignesValeurs; ++i)
	{
		INFO("Ligne numéro: " + std::to_string(i));
		CHECK(valeurs[i].size() == SambadaStoreyHistogram::nombreIntervalles);
	}
}

void SambadaStoreyHistogram::compare(const SambadaStoreyHistogram& autre) const
{

	/* Headers */
	CHECK(header.size() == nombreLignesHeader);
	CHECK(autre.header.size() == nombreLignesHeader);
	CHECKED_IF(nombreLignesHeader == autre.header.size())
	{
		for (int i(0); i < nombreLignesHeader; ++i)
		{
			INFO("Header numéro: " + std::to_string(i));
			CHECK(header[i].size() == nombreIntervallesHeaders[i]);
			CHECK(autre.header[i].size() == nombreIntervallesHeaders[i]);
			CHECKED_IF(nombreIntervallesHeaders[i] == autre.header[i].size())
			{
				for (int j(0); j < nombreIntervallesHeaders[i]; ++j)
				{
					CHECK(header[i][j] == Approx(autre.header[i][j]));
				}
			}
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
			INFO("Ligne numéro: " + std::to_string(i) + " (" + etiquettes[i+nombreLignesHeader] + ")");
			int nombreValeurs(valeurs[i].size());
			CHECKED_IF(nombreValeurs == autre.valeurs[i].size())
			{
				int tailleEtiquette(etiquettes[i].size());
				for (int j(0); j < nombreValeurs; ++j)
				{
					INFO("Valeur numéro: " + std::to_string(j) + " (" + std::to_string(header[0][j]) + ")");
					CHECK(valeurs[i][j] == Approx(autre.valeurs[i][j]));
				}
			}
		}
	}
}
