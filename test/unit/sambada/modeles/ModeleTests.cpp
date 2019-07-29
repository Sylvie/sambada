#include "catch.hpp"
#include "modeles/Modele.hpp"

TEST_CASE("Test that EtiquetteModeleComparateur compares EtiquetteModeles correctly", "[etiquette-modele-comparateur-unit]")
{
	sambada::EtiquetteModeleComparateur comparateur;

	SECTION("Test that empty EtiquettesModeles are equal")
	{
		sambada::EtiquetteModele premiereEtiquette;
		sambada::EtiquetteModele secondeEtiquette;

		CHECK_FALSE(comparateur(premiereEtiquette, secondeEtiquette));
		CHECK_FALSE(comparateur(secondeEtiquette, premiereEtiquette));
	}
}