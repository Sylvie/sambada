#include "catch.hpp"
#include "parametres/Parametres.hpp"
#include "parametres/LecteurParametres.hpp"

TEST_CASE("Test that LecteurParametres can read the Parametres", "[lecteur-parametres-unit]")
{
	sambada::Parametres parametres;
	sambada::LecteurParametres lecteur;

	lecteur.initialiseParametres(parametres);

	SECTION("Test that LecteurParametres can initialise the Parametres")
	{
		SECTION("Test that the Parametres have the correct number of entries")
		{
			CHECK(parametres.entrees.size() == 0);

		}
	}

}