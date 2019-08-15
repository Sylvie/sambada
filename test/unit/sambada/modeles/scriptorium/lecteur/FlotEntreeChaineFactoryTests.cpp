#include "catch.hpp"
#include "modeles/scriptorium/lecteur/FlotEntreeChaineFactory.hpp"
#include <sstream>

TEST_CASE("Test that FlotEntreeChaineFactory creates Input String Streams with given contents", "[flot-entree-chaine-factory-unit]")
{
	sambada::FlotEntreeChaineFactory factory;

	std::vector<std::string> nomsFlotsAttendus({"premierFlot", "deuxièmeFlot", "troisiemeFlot", "quatriemeFlot"});

	SECTION("Test that FlotEntreeChaineFactory creates working input string stream")
	{
		std::string nomFlot("nom-flot");
		std::string message("This is a message");

		factory.setContenusFlotsEntree({message});

		sambada::FlotEntree flot(factory.creeFlotEntree(nomFlot));

		std::istringstream *flotChaine = static_cast<std::istringstream *>(flot.get());
		std::string messageLu("");

		std::getline(*flotChaine, messageLu);

		CHECK(messageLu == message);
	}

	SECTION("Test that FlotEntreeChaineFactory returns the correct stream names")
	{
		int nombreFlots(4);

		for (int i(0); i < nombreFlots; ++i)
		{
			factory.creeFlotEntree(nomsFlotsAttendus[i]);
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

	SECTION("Test that FlotEntreeChaineFactory returns the streams with content")
	{
		int nombreFlots(4);
		std::vector<std::string> messages
				({
						 "Contenu du premier flot.",
						 "Contenu du deuxième flot.",
						 "Contenu du troisième flot.",
						 "Contenu du quatrième flot."
				 });

		factory.setContenusFlotsEntree(messages);

		std::vector<sambada::FlotEntree> flots;
		for (int i(0); i < nombreFlots; ++i)
		{
			flots.push_back(factory.creeFlotEntree(nomsFlotsAttendus[i]));
		}

		CHECKED_IF(flots.size() == nombreFlots)
		{
			for (int i(0); i < nombreFlots; ++i)
			{
				std::istringstream *flotChaine = static_cast<std::istringstream *>(flots[i].get());
				std::string messageLu("");
				std::getline(*flotChaine, messageLu);
				CHECK(messageLu == messages[i]);
			}
		}
	}

	SECTION("Test that FlotEntreeChaineFactory returns empty streams when no more content is available")
	{
		int nombreFlots(3);
		std::vector<std::string> messages(
				{
						"Contenu du premier flot.",
						"Contenu du deuxième flot.",
						"Contenu du troisième flot."
				});

		factory.setContenusFlotsEntree(messages);

		std::vector<sambada::FlotEntree> flots;
		for (int i(0); i <= nombreFlots; ++i)
		{
			flots.push_back(factory.creeFlotEntree(nomsFlotsAttendus[i]));
		}

		CHECKED_IF(flots.size() == (nombreFlots + 1))
		{
			std::istringstream *flotChaine = static_cast<std::istringstream *>(flots[nombreFlots].get());
			CHECK(flotChaine->str() == "");
		}
	}

}