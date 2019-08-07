#include "catch.hpp"
#include "modeles/scriptorium/FlotSortieChaineFactory.hpp"
#include <sstream>

TEST_CASE("Test that FlotSortieChaineFactory creates and stores Output String Streams", "[flot-sortie-chaine-factory-unit]")
{
	sambada::FlotSortieChaineFactory factory;

	std::string nomFlot("nom-flot");

	SECTION("Test that FlotSortieChaineFactory creates working output string stream")
	{
		FlotSortie flot(factory.creeFlotSortie(nomFlot));

		std::string message("This is a message");
		*flot << message << std::flush;

		std::ostringstream* flotChaine = static_cast<std::ostringstream*>(flot.get());
		std::string messageLu(flotChaine->str());

		CHECK(messageLu == message);
	}

}