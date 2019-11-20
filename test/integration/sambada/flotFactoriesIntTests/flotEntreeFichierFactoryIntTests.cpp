#include "catch.hpp"
#include "modeles/scriptorium/lecteur/FlotEntreeFichierFactory.hpp"
#include "SambadaIntegrationTestUtils.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that FlotEntreeFichierFactory provides an ifstream", "[flot-entree-fichier-factory-int]")
{

	INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

	GIVEN("A FlotEntreeFichierFactory and a filename")
	{
		std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

		std::string pathToTestFolder("test/integration/sambada/flotFactoriesIntTests/");

		std::string pathToInputFolder(SambadaIntegrationTestUtils::getTopSourceDirectory() + pathToTestFolder);
		std::string fileName(pathToInputFolder + "input-file.txt");

		std::vector<std::string> outputFileNames({});

		sambada::FlotEntreeFichierFactory factory;

		CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

		WHEN("the factory method is called and the stream used")
		{
			std::string expectedMessage = "This is an important message.";

			sambada::FlotEntree flot(factory.creeFlotEntree(fileName));

			std::string readMessage("");
			std::getline(*flot, readMessage);

			THEN("the results match the expectation")
			{
				CHECK(readMessage == expectedMessage);
			}
		}

		SambadaIntegrationTestUtils::removeFiles(outputFileNames);

	}

}



