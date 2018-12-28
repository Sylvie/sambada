#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that regression results are correct for models of dimension 1", "[models-dim-1-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("A program to run, its expected output file, and the file of expected results")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/regressionResultsIntTests/");
        std::string fileNameOut0(pathToOutputFolder + "choice-mark-cattle-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "choice-mark-cattle-Out-1.txt");
        std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameLogs});

        std::string pathToInputFolder(SambadaIntegrationTestUtils::getTopSourceDirectory() + "test/integration/sambada/regressionResultsIntTests/");

        std::string fileNameParam(pathToInputFolder + "param-dim-1.txt");
        std::string fileNameEnv(pathToInputFolder + "choice-env-cattle.csv");
        std::string fileNameMark(pathToInputFolder + "choice-mark-cattle.txt");

        std::string fileNameExpectedResultsDim0(pathToInputFolder + "expected-results-cattle-dim-0.txt");
        std::string fileNameExpectedResultsDim1(pathToInputFolder + "expected-results-cattle-dim-1.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResultsDim0.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedNullResults(SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
        lecteurCorrige.close();
        expectedNullResults.verifieTailles(true, 0, 5);

        lecteurCorrige.open(fileNameExpectedResultsDim1.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResults(SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
        lecteurCorrige.close();
        expectedResults.verifieTailles(true, 1, 30);

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
                        results.verifieTailles(true, 0, 5);
                        results.compare(expectedNullResults);
                    }
                }

                lecteur.close();
            }

            THEN("the output file of dimension 1 is found")
            {
                std::ifstream lecteur(fileNameOut1.c_str());

                REQUIRE(lecteur.good());
                REQUIRE(lecteur.is_open());

                AND_WHEN("the file is read")
                {
                    SambadaRegressionResults results(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteur, true, 1));

                    THEN("the results match the expectation")
                    {
                        results.verifieTailles(true, 1, 30);
                        results.compare(expectedResults);
                    }
                }

                lecteur.close();
            }

            SambadaIntegrationTestUtils::removeFiles(outputFileNames);
        }
    }
}

