#include "catch.hpp"
#include "modeles/Modele.hpp"

TEST_CASE("Test that EtiquetteModeleComparateur compares EtiquetteModeles correctly", "[etiquette-modele-comparateur-unit]")
{
	sambada::EtiquetteModeleComparateur comparateur;

	sambada::EtiquetteModele premiereEtiquette;
	sambada::EtiquetteModele secondeEtiquette;

	SECTION("Test that empty EtiquettesModeles are equal")
	{
		CHECK_FALSE(comparateur(premiereEtiquette, secondeEtiquette));
		CHECK_FALSE(comparateur(secondeEtiquette, premiereEtiquette));
	}

	SECTION("Test that EtiquettesModeles with a marqueur only are equal")
	{
		premiereEtiquette.marqueur = 5;
		secondeEtiquette.marqueur = 5;

		CHECK_FALSE(comparateur(premiereEtiquette, secondeEtiquette));
		CHECK_FALSE(comparateur(secondeEtiquette, premiereEtiquette));
	}

	SECTION("Test that EtiquettesModeles with the same marqueur and environment are equal")
	{
		premiereEtiquette.marqueur = 5;
		secondeEtiquette.marqueur = 5;

		premiereEtiquette.environnement = {1, 6};
		secondeEtiquette.environnement = {1, 6};

		CHECK_FALSE(comparateur(premiereEtiquette, secondeEtiquette));
		CHECK_FALSE(comparateur(secondeEtiquette, premiereEtiquette));
	}

	SECTION("Test that EtiquettesModeles with the same marqueur and different environments are compared correctly")
	{
		premiereEtiquette.marqueur = 5;
		secondeEtiquette.marqueur = 5;

		premiereEtiquette.environnement = {1, 6};
		secondeEtiquette.environnement = {3};

		CHECK(comparateur(premiereEtiquette, secondeEtiquette));
		CHECK_FALSE(comparateur(secondeEtiquette, premiereEtiquette));
	}

	SECTION("Test that EtiquettesModeles with different marqueurs and same environment are compared correctly")
	{
		premiereEtiquette.marqueur = 5;
		secondeEtiquette.marqueur = 3;

		premiereEtiquette.environnement = {1, 6};
		secondeEtiquette.environnement = {1, 6};

		CHECK_FALSE(comparateur(premiereEtiquette, secondeEtiquette));
		CHECK(comparateur(secondeEtiquette, premiereEtiquette));
	}

	SECTION("Test that EtiquettesModeles with different marqueurs and environments are compared correctly")
	{
		premiereEtiquette.marqueur = 3;
		secondeEtiquette.marqueur = 5;

		premiereEtiquette.environnement = {1, 6};
		secondeEtiquette.environnement = {2};

		CHECK(comparateur(premiereEtiquette, secondeEtiquette));
		CHECK_FALSE(comparateur(secondeEtiquette, premiereEtiquette));
	}
}