#include "catch.hpp"
#include "variables/FamilleVariablesFactory.hpp"

TEST_CASE("FamilleVariableFactory can create FamilleVariables", "[famille-variables-factory-unit]") {
	sambada::FamilleVariablesFactory factory;
	const int nombrePoints(4);
	sambada::MatriceBools masque(nombrePoints, 5);

	sambada::FamilleVariables famille;

	SECTION("No variables nor generations leads to empty family")
	{
		famille = factory.genereFamille(0, 0, masque);

		CHECK(famille.size() == 0);
	}

	SECTION("No variables leads to empty family")
	{
		famille = factory.genereFamille(0, 5, sambada::MatriceBools(nombrePoints, 0));

		CHECK(famille.size() == 0);
	}

	SECTION("No generations leads to empty family")
	{
		famille = factory.genereFamille(5, 0, masque);

		CHECK(famille.size() == 0);
	}

	SECTION("Negative number of variables leads to empty family")
	{
		famille = factory.genereFamille(-4, 3, sambada::MatriceBools(nombrePoints, 0));

		CHECK(famille.size() == 0);
	}

	SECTION("Negative number of generations leads to empty family")
	{
		famille = factory.genereFamille(5, -2, masque);

		CHECK(famille.size() == 0);
	}
}