#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that \"POPULATIONVAR\" cannot be set at the same time as \"SAVETYPE BEST\"",
         "[pop-struct-with-savetype-best-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Some input files with info on population structure")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/basicPopulationStructureIntTests/");

        std::string fileNameLogs(pathToOutputFolder + "cattle-pop-mark-first-savetype-best-log.txt");

        std::vector<std::string> outputFileNames({fileNameLogs});

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/basicPopulationStructureIntTests/");

        std::string fileNameParams(pathToInputFolder + "param-pop-first-savetype-best.txt");
        std::string fileNameEnv(pathToInputFolder + "cattle-pop-env-first.csv");
        std::string fileNameMark(pathToInputFolder + "cattle-pop-mark.txt");

        WHEN("Sambada is run using the population variables and savetype=best at the same time")
        {
            CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnv + " " + fileNameMark);
            INFO(output);

	        THEN("the log file contains the error message")
	        {
		        CHECK(output.find("POPULATIONVAR : Please set \"SAVETYPE ALL\" when enabling \"POPULATIONVAR\".") !=
		              std::string::npos);
	        }

        }

        SambadaIntegrationTestUtils::removeFiles(outputFileNames);
    }
}

