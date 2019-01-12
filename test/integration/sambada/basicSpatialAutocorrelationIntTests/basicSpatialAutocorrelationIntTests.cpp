#include "catch.hpp"
#include "SambadaSpatialAutocorrelationResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that spatial autocorrelation is computed correctly for a simple case",
         "[basic-spatial-autocorrelation-int]") {

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
        std::string fileNameOutASMark(pathToOutputFolder + "random-sample-AS-Mark.txt");
        std::string fileNameOutASMarkPVal(pathToOutputFolder + "random-sample-AS-Mark-pVal.txt");
        std::string fileNameOutASMarkSim(pathToOutputFolder + "random-sample-AS-Mark-Sim.txt");
        std::string fileNameLogs(pathToOutputFolder + "random-sample-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameOutASEnv, fileNameOutASEnvPVal, fileNameOutASEnvSim,
                                                  fileNameOutASMark, fileNameOutASMarkPVal, fileNameOutASMarkSim,fileNameLogs});

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/basicSpatialAutocorrelationIntTests/");

        std::string fileNameParams(pathToInputFolder + "param-random-sample.txt");
        std::string fileNameData(pathToInputFolder + "random-sample.txt");

        std::string fileNameExpectedResultsASEnv(pathToInputFolder + "expected-results-random-sample-AS-Env.txt");
        std::string fileNameExpectedResultsASEnvPVal(pathToInputFolder + "expected-results-random-sample-AS-Env-pVal.txt");
        std::string fileNameExpectedResultsASMark(pathToInputFolder + "expected-results-random-sample-AS-Mark.txt");
        std::string fileNameExpectedResultsASMarkPVal(pathToInputFolder + "expected-results-random-sample-AS-Mark-pVal.txt");

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

        lecteurCorrige.open(fileNameExpectedResultsASMark.c_str());
        INFO("Reading expected results AS Mark");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaSpatialAutocorrelationResults expectedResultsASMark(
                SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurCorrige, 1));
        lecteurCorrige.close();
        expectedResultsASMark.verifieTailles(100, 1);

        lecteurCorrige.open(fileNameExpectedResultsASMarkPVal.c_str());
        INFO("Reading expected results AS Mark pVal");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaSpatialAutocorrelationResults expectedResultsASMarkPVal(
                SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurCorrige, 1));
        lecteurCorrige.close();
        expectedResultsASMarkPVal.verifieTailles(100, 1);

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

                INFO("Reading results AS Mark");
                std::ifstream lecteurOutASMark(fileNameOutASMark.c_str());
                REQUIRE(lecteurOutASMark.good());
                REQUIRE(lecteurOutASMark.is_open());

                INFO("Reading results AS Mark pVal");
                std::ifstream lecteurOutASMarkPVal(fileNameOutASMarkPVal.c_str());
                REQUIRE(lecteurOutASMarkPVal.good());
                REQUIRE(lecteurOutASMarkPVal.is_open());

                AND_WHEN("the output files are read")
                {
                    SambadaSpatialAutocorrelationResults resultsASEnv(
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASEnv, 2));
                    SambadaSpatialAutocorrelationResults resultsASEnvPVal(
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASEnvPVal, 2));
                    SambadaSpatialAutocorrelationResults resultsASMark(
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASMark, 1));
                    SambadaSpatialAutocorrelationResults resultsASMarkPVal(
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASMarkPVal, 1));

                    THEN("the results match the expectations")
                    {
                        INFO("Verifying results env");
                        resultsASEnv.verifieTailles(100, 2);
                        resultsASEnv.compare(expectedResultsASEnv);

                        INFO("Verifying p-values env");
                        resultsASEnvPVal.verifieTailles(100, 2);
                        resultsASEnvPVal.comparePValeurs(expectedResultsASEnvPVal);

                        INFO("Verifying results mark");
                        resultsASMark.verifieTailles(100, 1);
                        resultsASMark.compare(expectedResultsASMark);

                        INFO("Verifying p-values mark");
                        resultsASMarkPVal.verifieTailles(100, 1);
                        resultsASMarkPVal.comparePValeurs(expectedResultsASMarkPVal);
                    }
                }

                lecteurOutASEnv.close();
                lecteurOutASEnvPVal.close();
                lecteurOutASMark.close();
                lecteurOutASMarkPVal.close();
            }
        }

        SambadaIntegrationTestUtils::removeFiles(outputFileNames);
    }
}

