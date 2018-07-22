#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that regression results are correct for models of dimension 1", "[models-dim-1-int]") {

    std::cout << "Starting there: " << SambadaIntegrationTestUtils::runCommand("pwd") << std::endl;

    GIVEN("A program to run, its expected output file, and the file of expected results")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));


        std::string fileName("./test/integration/sambada/choice-mark-cattle-Out-1.txt");

        std::string pathToInputDirectory(SambadaIntegrationTestUtils::getTopSourceDirectory() + "test/integration/sambada/regressionResultsIntTests/");


        std::string fileNameParam(pathToInputDirectory + "param-dim-1.txt");
        std::string fileNameEnv(pathToInputDirectory + "choice-env-cattle.csv");
        std::string fileNameMark(pathToInputDirectory + "choice-mark-cattle.txt");

        std::string fileNameExpectedResults(pathToInputDirectory + "expected-results-cattle-dim-1.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResults.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResults(SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
        expectedResults.verifieTailles(true, 1, 30);

        WHEN("the program is run")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(program + " " + fileNameParam + " " + fileNameEnv + " " + fileNameMark);
            //INFO(output);

            std::ifstream lecteur(fileName.c_str());

            THEN("the output file is found")
            {
                REQUIRE(lecteur.good());
                REQUIRE(lecteur.is_open());
            }

            AND_WHEN("the file is read")
            {
                SambadaRegressionResults results(SambadaIntegrationTestUtils::readRegressionResults(lecteur, true, 1));

                THEN("the results have the expected size")
                {
                    results.verifieTailles(true, 1, 30);
                }

                AND_THEN("the results match the expectation")
                {
                    results.compare(expectedResults);
                }
            }

            lecteur.close();
            //std::remove(fileName.c_str());
        }
    }
}

