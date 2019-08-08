#include "catch.hpp"
#include "modeles/scriptorium/FlotSortieChaineFactory.hpp"
#include <sstream>

TEST_CASE("Test that FlotSortieChaineFactory creates and stores Output String Streams", "[flot-sortie-chaine-factory-unit]")
{
	sambada::FlotSortieChaineFactory factory;

	std::vector<std::string> nomsFlotsAttendus({"premierFlot", "deuxièmeFlot", "troisiemeFlot", "quatriemeFlot"});

	SECTION("Test that FlotSortieChaineFactory creates working output string stream")
	{
		std::string nomFlot("nom-flot");
		FlotSortie flot(factory.creeFlotSortie(nomFlot));

		std::string message("This is a message");
		*flot << message << std::flush;

		std::ostringstream* flotChaine = static_cast<std::ostringstream*>(flot.get());
		std::string messageLu(flotChaine->str());

		CHECK(messageLu == message);
	}

	SECTION("Test that FlotSortieChaineFactory returns the correct stream names")
	{
		int nombreFlots(4);

		for (int i(0); i < nombreFlots; ++i)
		{
			factory.creeFlotSortie(nomsFlotsAttendus[i]);
		}

		auto nomsFlots(factory.getNomsFlots());
		CHECKED_IF(nomsFlots.size() == nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				CHECK(nomsFlots[i] == nomsFlotsAttendus[i]);
			}
		}
	}

	SECTION("Test that FlotSortieChaineFactory returns the correct number of streams")
	{
		int nombreFlots(3);

		for (int i(0); i < nombreFlots; ++i)
		{
			factory.creeFlotSortie(nomsFlotsAttendus[i]);
		}

		CHECK(factory.getFlotsSortie().size() == 3);
	}

	SECTION("Test that FlotSortieChaineFactory returns the streams with content")
	{
		int nombreFlots(4);

		std::vector<FlotSortie> flots;
		for (int i(0); i < nombreFlots; ++i)
		{
			flots.push_back(factory.creeFlotSortie(nomsFlotsAttendus[i]));
		}

		*(flots[2]) << "Message 1 dans flot 3. ";
		*(flots[1]) << "Message 2 dans flot 2. ";
		*(flots[3]) << "Message 3 dans flot 4. ";
		*(flots[0]) << "Message 4 dans flot 1. ";

		*(flots[0]) << "Message 5 dans flot 1. ";
		*(flots[3]) << "Message 6 dans flot 4. ";
		*(flots[2]) << "Message 7 dans flot 3. ";
		*(flots[1]) << "Message 8 dans flot 2. ";

		*(flots[1]) << "Message 9 dans flot 2. ";
		*(flots[3]) << "Message 10 dans flot 4. ";
		*(flots[0]) << "Message 11 dans flot 1. ";
		*(flots[2]) << "Message 12 dans flot 3. ";

		std::vector<std::string> expectedMessages({
			"Message 4 dans flot 1. Message 5 dans flot 1. Message 11 dans flot 1. ",
			"Message 2 dans flot 2. Message 8 dans flot 2. Message 9 dans flot 2. ",
			"Message 1 dans flot 3. Message 7 dans flot 3. Message 12 dans flot 3. ",
			"Message 3 dans flot 4. Message 6 dans flot 4. Message 10 dans flot 4. "
		                                          });

		std::vector<FlotSortie> flotsSortie(factory.getFlotsSortie());

		CHECKED_IF(flotsSortie.size() == nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				std::ostringstream* flotChaine = static_cast<std::ostringstream*>(flots[i].get());
				CHECK(flotChaine->str() == expectedMessages[i]);
			}
		}
	}

}