#include "catch.hpp"
#include "SambadaRegressionResults.hpp"
#include "SambadaIntegrationTestUtils.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that regression results for monomophic models are correct for models of dimension 0", "[monomorphic-models-dim-0-int][monomorphic-models-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("A program to run, its expected output file, and the file of expected results")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

	    std::string pathToTestFolder("test/integration/sambada/monomorphicModelsRegressionResultsIntTests/");

	    std::string pathToOutputFolder("./" + pathToTestFolder);
	    std::string fileNameOut0(pathToOutputFolder + "choice-mark-cattle-Out-0.txt");
        std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameLogs});

        std::string pathToInputFolder(SambadaIntegrationTestUtils::getTopSourceDirectory() + pathToTestFolder);

        std::string fileNameParam(pathToInputFolder + "param-dim-0.txt");
        std::string fileNameEnv(pathToInputFolder + "choice-env-cattle.csv");
        std::string fileNameMark(pathToInputFolder + "choice-mark-cattle.txt");

        std::string fileNameExpectedResultsDim0(pathToInputFolder + "expected-results-cattle-dim-0.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResultsDim0.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedNullResults(SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
        lecteurCorrige.close();
        expectedNullResults.verifieTailles(true, 0, 1);

        CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

        WHEN("the program is run")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(program + " " + fileNameParam + " " + fileNameEnv + " " + fileNameMark);
            //INFO(output);

            THEN("the output file of dimension 0 is found")
            {
                std::ifstream lecteur(fileNameOut0.c_str());

                REQUIRE(lecteur.good());
                REQUIRE(lecteur.is_open());

                AND_WHEN("the file is read")
                {
                    SambadaRegressionResults results(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteur, true, 0));

                    THEN("the results match the expectation")
                    {
                        results.verifieTailles(true, 0, 1);
                        results.compare(expectedNullResults);
                    }
                }

                lecteur.close();
            }

            SambadaIntegrationTestUtils::removeFiles(outputFileNames);
        }
    }
}

