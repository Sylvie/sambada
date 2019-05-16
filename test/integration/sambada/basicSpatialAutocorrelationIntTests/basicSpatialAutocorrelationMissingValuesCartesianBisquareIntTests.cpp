#include "catch.hpp"
#include "SambadaSpatialAutocorrelationResults.hpp"
#include "SambadaIntegrationTestUtils.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that spatial autocorrelation is computed correctly with some missing environmental data with cartesian coordinates and \"bisquare\" weighting",
         "[basic-spatial-autocorrelation-missing-values-cartesian-bisquare-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Some input files with sampling locations")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/basicSpatialAutocorrelationIntTests/");

        std::string fileNameOut0(pathToOutputFolder + "random-sample-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "random-sample-Out-1.txt");
        std::string fileNameOutASEnv(pathToOutputFolder + "random-sample-AS-Env.txt");
        std::string fileNameOutASEnvPVal(pathToOutputFolder + "random-sample-AS-Env-pVal.txt");
        std::string fileNameOutASEnvSim(pathToOutputFolder + "random-sample-AS-Env-Sim.txt");
        std::string fileNameLogs(pathToOutputFolder + "random-sample-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameOutASEnv, fileNameOutASEnvPVal, fileNameOutASEnvSim, fileNameLogs});

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/basicSpatialAutocorrelationIntTests/");

        std::string fileNameParams(pathToInputFolder + "param-random-sample-missing-values-bisquare.txt");
        std::string fileNameData(pathToInputFolder + "random-sample-missing-values.txt");

        std::string fileNameExpectedResultsASEnv(pathToInputFolder + "expected-results-random-sample-missing-values-bisquare-AS-Env.txt");
        std::string fileNameExpectedResultsASEnvPVal(pathToInputFolder + "expected-results-random-sample-missing-values-bisquare-AS-Env-pVal.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResultsASEnv.c_str());
        INFO("Reading expected results AS Env");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaSpatialAutocorrelationResults expectedResultsASEnv(
                SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurCorrige, 2));
        lecteurCorrige.close();
        expectedResultsASEnv.verifieTailles(100, 2);

        lecteurCorrige.open(fileNameExpectedResultsASEnvPVal.c_str());
        INFO("Reading expected results AS Env pVal");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaSpatialAutocorrelationResults expectedResultsASEnvPVal(
                SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurCorrige, 2));
        lecteurCorrige.close();
        expectedResultsASEnvPVal.verifieTailles(100, 2);

        WHEN("Sambada is run to compute the spatial autocorrelation")
        {
            CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameData);
            INFO(output);

            THEN("the output files are found")
            {
                INFO("Reading results AS Env");
                std::ifstream lecteurOutASEnv(fileNameOutASEnv.c_str());
                REQUIRE(lecteurOutASEnv.good());
                REQUIRE(lecteurOutASEnv.is_open());

                INFO("Reading results AS pVal");
                std::ifstream lecteurOutASEnvPVal(fileNameOutASEnvPVal.c_str());
                REQUIRE(lecteurOutASEnvPVal.good());
                REQUIRE(lecteurOutASEnvPVal.is_open());

                AND_WHEN("the output files are read")
                {
                    SambadaSpatialAutocorrelationResults resultsASEnv(
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASEnv, 2));
                    SambadaSpatialAutocorrelationResults resultsASEnvPVal(
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASEnvPVal, 2));

                    THEN("the results match the expectations")
                    {
                        INFO("Verifying results env");
                        resultsASEnv.verifieTailles(100, 2);
                        resultsASEnv.compare(expectedResultsASEnv);

                        INFO("Verifying p-values env");
                        resultsASEnvPVal.verifieTailles(100, 2);
                        resultsASEnvPVal.comparePValeurs(expectedResultsASEnvPVal);
                    }
                }

                lecteurOutASEnv.close();
                lecteurOutASEnvPVal.close();
            }
        }

        SambadaIntegrationTestUtils::removeFiles(outputFileNames);
    }
}

