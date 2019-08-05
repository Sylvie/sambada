#include "catch.hpp"
#include "modeles/scriptorium/FlotSortieFichierFactory.hpp"
#include "SambadaIntegrationTestUtils.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that FlotSortieFichierFactory provides an ofstream", "[flot-sortie-fichier-factory-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("A FlotSortieFichierFactory and a filename")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

	    std::string pathToTestFolder("test/integration/sambada/flotFactoriesIntTests/");

	    std::string pathToOutputFolder("./" + pathToTestFolder);
        std::string fileName(pathToOutputFolder + "my-output-file.txt");

        std::vector<std::string> outputFileNames({fileName});

		sambada::FlotSortieFichierFactory factory;


        CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

        WHEN("the factory method is called and the stream used")
        {
            std::string message = "This is a logistic model.";

            FlotSortie flot(factory.creeFlotSortie(fileName));

            *flot << message << std::flush;

            THEN("the output file is found")
            {
                std::ifstream lecteur(fileName.c_str());

                REQUIRE(lecteur.good());
                REQUIRE(lecteur.is_open());

                AND_WHEN("the file is read")
                {
                	std::string readMessage;

					std::getline(lecteur, readMessage);

                    THEN("the results match the expectation")
                    {
                        CHECK(readMessage == message);
                    }
                }

                lecteur.close();
            }

            SambadaIntegrationTestUtils::removeFiles(outputFileNames);
        }
    }
}

