#include "SambadaInputData.hpp"
#include "catch.hpp"

void SambadaInputData::verifieTailles(int nombreLignes, int nombreColonnes) const
{
	REQUIRE(data.size() == nombreLignes);

	for (int i(0); i < nombreLignes; ++i)
	{
		INFO("Ligne numéro: " + std::to_string(i));
		CHECK(data[i].size() == nombreColonnes);
	}
}

void SambadaInputData::compare(const SambadaInputData& autre) const
{
	/* Valeurs */
	int nombreLignes = data.size();
	CHECKED_IF(nombreLignes == autre.data.size())
	{
		for (int i(0); i < nombreLignes; ++i)
		{
			INFO("Ligne numéro: " + std::to_string(i));
			int nombreValeurs(data[i].size());
			CHECKED_IF(nombreValeurs == autre.data[i].size())
			{
				for (int j(0); j < nombreValeurs; ++j)
				{
					INFO("Valeur numéro: " + std::to_string(j));
					CHECK(data[i][j] == autre.data[i][j]);
				}
			}
		}
	}
}
