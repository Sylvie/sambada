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
			CHECK(parametres.entrees.size() == 23);
		}

		CHECKED_IF(parametres.entrees.size() == 23)
		{
			SECTION("Test that the initial parameters are marked as absent")
			{
				for (auto entree = parametres.entrees.begin(); entree != parametres.entrees.end(); ++entree)
				{
					CHECK_FALSE(entree->present);
				}
			}

			SECTION("Test that the initial parameters have no contents")
			{
				for (auto entree = parametres.entrees.begin(); entree != parametres.entrees.end(); ++entree)
				{
					CHECK(entree->contents.size() == 0);
				}
			}

			SECTION("Test that \"INPUTFILE\" is initialised correctly")
			{
				int paramCourant(0);
				CHECK(parametres.entrees[paramCourant].name == "INPUTFILE");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["INPUTFILE"] == paramCourant);
			}

			SECTION("Test that \"OUTPUTFILE\" is initialised correctly")
			{
				int paramCourant(1);
				CHECK(parametres.entrees[paramCourant].name == "OUTPUTFILE");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["OUTPUTFILE"] == paramCourant);
			}

			SECTION("Test that \"WORDDELIM\" is initialised correctly")
			{
				int paramCourant(2);
				CHECK(parametres.entrees[paramCourant].name == "WORDDELIM");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["WORDDELIM"] == paramCourant);
			}

			SECTION("Test that \"LOG\" is initialised correctly")
			{
				int paramCourant(3);
				CHECK(parametres.entrees[paramCourant].name == "LOG");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["LOG"] == paramCourant);
			}

			SECTION("Test that \"UNCONVERGEDMODELS\" is initialised correctly")
			{
				int paramCourant(4);
				CHECK(parametres.entrees[paramCourant].name == "UNCONVERGEDMODELS");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["UNCONVERGEDMODELS"] == paramCourant);
			}

			SECTION("Test that \"HEADERS\" is initialised correctly")
			{
				int paramCourant(5);
				CHECK(parametres.entrees[paramCourant].name == "HEADERS");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["HEADERS"] == paramCourant);
			}

			SECTION("Test that \"NUMVARENV\" is initialised correctly")
			{
				int paramCourant(6);
				CHECK(parametres.entrees[paramCourant].name == "NUMVARENV");
				CHECK(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["NUMVARENV"] == paramCourant);
			}

			SECTION("Test that \"NUMMARK\" is initialised correctly")
			{
				int paramCourant(7);
				CHECK(parametres.entrees[paramCourant].name == "NUMMARK");
				CHECK(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["NUMMARK"] == paramCourant);
			}

			SECTION("Test that \"NUMINDIV\" is initialised correctly")
			{
				int paramCourant(8);
				CHECK(parametres.entrees[paramCourant].name == "NUMINDIV");
				CHECK(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["NUMINDIV"] == paramCourant);
			}

			SECTION("Test that \"IDINDIV\" is initialised correctly")
			{
				int paramCourant(9);
				CHECK(parametres.entrees[paramCourant].name == "IDINDIV");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["IDINDIV"] == paramCourant);
			}

			SECTION("Test that \"COLSUPENV\" is initialised correctly")
			{
				int paramCourant(10);
				CHECK(parametres.entrees[paramCourant].name == "COLSUPENV");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["COLSUPENV"] == paramCourant);
			}

			SECTION("Test that \"COLSUPMARK\" is initialised correctly")
			{
				int paramCourant(11);
				CHECK(parametres.entrees[paramCourant].name == "COLSUPMARK");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["COLSUPMARK"] == paramCourant);
			}

			SECTION("Test that \"SUBSETVARENV\" is initialised correctly")
			{
				int paramCourant(12);
				CHECK(parametres.entrees[paramCourant].name == "SUBSETVARENV");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["SUBSETVARENV"] == paramCourant);
			}

			SECTION("Test that \"SUBSETMARK\" is initialised correctly")
			{
				int paramCourant(13);
				CHECK(parametres.entrees[paramCourant].name == "SUBSETMARK");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["SUBSETMARK"] == paramCourant);
			}

			SECTION("Test that \"DIMMAX\" is initialised correctly")
			{
				int paramCourant(14);
				CHECK(parametres.entrees[paramCourant].name == "DIMMAX");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["DIMMAX"] == paramCourant);
			}

			SECTION("Test that \"SPATIAL\" is initialised correctly")
			{
				int paramCourant(15);
				CHECK(parametres.entrees[paramCourant].name == "SPATIAL");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["SPATIAL"] == paramCourant);
			}

			SECTION("Test that \"AUTOCORR\" is initialised correctly")
			{
				int paramCourant(16);
				CHECK(parametres.entrees[paramCourant].name == "AUTOCORR");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECKED_IF(parametres.entrees[paramCourant].prereq.size() == 1)
				{
					CHECK(parametres.entrees[paramCourant].prereq[0] == "SPATIAL");
				}
				CHECK(parametres.index["AUTOCORR"] == paramCourant);
			}

			SECTION("Test that \"GWR\" is initialised correctly")
			{
				int paramCourant(17);
				CHECK(parametres.entrees[paramCourant].name == "GWR");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECKED_IF(parametres.entrees[paramCourant].prereq.size() == 1)
				{
					CHECK(parametres.entrees[paramCourant].prereq[0] == "SPATIAL");
				}
				CHECK(parametres.index["GWR"] == paramCourant);
			}

			SECTION("Test that \"SHAPEFILE\" is initialised correctly")
			{
				int paramCourant(18);
				CHECK(parametres.entrees[paramCourant].name == "SHAPEFILE");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECKED_IF(parametres.entrees[paramCourant].prereq.size() == 1)
				{
					CHECK(parametres.entrees[paramCourant].prereq[0] == "SPATIAL");
				}
				CHECK(parametres.index["SHAPEFILE"] == paramCourant);
			}

			SECTION("Test that \"DISCRETEVAR\" is initialised correctly")
			{
				int paramCourant(19);
				CHECK(parametres.entrees[paramCourant].name == "DISCRETEVAR");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["DISCRETEVAR"] == paramCourant);
			}

			SECTION("Test that \"SAVETYPE\" is initialised correctly")
			{
				int paramCourant(20);
				CHECK(parametres.entrees[paramCourant].name == "SAVETYPE");
				CHECK(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["SAVETYPE"] == paramCourant);
			}

			SECTION("Test that \"POPULATIONVAR\" is initialised correctly")
			{
				int paramCourant(21);
				CHECK(parametres.entrees[paramCourant].name == "POPULATIONVAR");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["POPULATIONVAR"] == paramCourant);
			}


			SECTION("Test that \"STOREY\" is initialised correctly")
			{
				int paramCourant(22);
				CHECK(parametres.entrees[paramCourant].name == "STOREY");
				CHECK_FALSE(parametres.entrees[paramCourant].mandatory);
				CHECK(parametres.entrees[paramCourant].prereq.size() == 0);
				CHECK(parametres.index["STOREY"] == paramCourant);
			}
		}
	}
}