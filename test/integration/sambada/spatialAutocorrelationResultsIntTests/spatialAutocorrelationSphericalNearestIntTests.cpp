#include "catch.hpp"
#include "SambadaSpatialAutocorrelationResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that spatial autocorrelation is computed correctly for spherical coordinates and nearest neighbours weighting",
         "[spatial-autocorrelation-spherical-nearest-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Some input files with sampling locations")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/spatialAutocorrelationResultsIntTests/");

        std::string fileNameOut0(pathToOutputFolder + "cattle-pop-mark-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "cattle-pop-mark-Out-1.txt");
        std::string fileNameOutASEnv(pathToOutputFolder + "cattle-pop-mark-AS-Env.txt");
        std::string fileNameOutASEnvPVal(pathToOutputFolder + "cattle-pop-mark-AS-Env-pVal.txt");
        std::string fileNameOutASEnvSim(pathToOutputFolder + "cattle-pop-mark-AS-Env-Sim.txt");
        std::string fileNameOutASMark(pathToOutputFolder + "cattle-pop-mark-AS-Mark.txt");
        std::string fileNameOutASMarkPVal(pathToOutputFolder + "cattle-pop-mark-AS-Mark-pVal.txt");
        std::string fileNameOutASMarkSim(pathToOutputFolder + "cattle-pop-mark-AS-Mark-Sim.txt");
        std::string fileNameLogs(pathToOutputFolder + "cattle-pop-mark-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameOutASEnv, fileNameOutASEnvPVal, fileNameOutASEnvSim,
                                                  fileNameOutASMark, fileNameOutASMarkPVal, fileNameOutASMarkSim,fileNameLogs});

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/spatialAutocorrelationResultsIntTests/");

        std::string fileNameParams(pathToInputFolder + "param-spherical-nearest.txt");
        std::string fileNameEnv(pathToInputFolder + "cattle-pop-env.csv");
	    std::string fileNameMark(pathToInputFolder + "cattle-pop-mark.txt");
	    int nombrePoints(804), nombreVariablesEnv(7), nombreMark(30);

        std::string fileNameExpectedResultsASEnv(pathToInputFolder + "expected-results-spherical-nearest-env.txt");
        std::string fileNameExpectedResultsASEnvPVal(pathToInputFolder + "expected-results-spherical-nearest-env-pval.txt");
        std::string fileNameExpectedResultsASMark(pathToInputFolder + "expected-results-spherical-nearest-mark.txt");
        std::string fileNameExpectedResultsASMarkPVal(pathToInputFolder + "expected-results-spherical-nearest-mark-pval.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResultsASEnv.c_str());
        INFO("Reading expected results AS Env");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaSpatialAutocorrelationResults expectedResultsASEnv(
                SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurCorrige, nombreVariablesEnv));
        lecteurCorrige.close();
        expectedResultsASEnv.verifieTailles(nombrePoints, nombreVariablesEnv);

        lecteurCorrige.open(fileNameExpectedResultsASEnvPVal.c_str());
        INFO("Reading expected results AS Env pVal");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaSpatialAutocorrelationResults expectedResultsASEnvPVal(
                SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurCorrige, nombreVariablesEnv));
        lecteurCorrige.close();
        expectedResultsASEnvPVal.verifieTailles(nombrePoints, nombreVariablesEnv);

        lecteurCorrige.open(fileNameExpectedResultsASMark.c_str());
        INFO("Reading expected results AS Mark");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaSpatialAutocorrelationResults expectedResultsASMark(
                SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurCorrige, nombreMark));
        lecteurCorrige.close();
        expectedResultsASMark.verifieTailles(nombrePoints, nombreMark);

        lecteurCorrige.open(fileNameExpectedResultsASMarkPVal.c_str());
        INFO("Reading expected results AS Mark pVal");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaSpatialAutocorrelationResults expectedResultsASMarkPVal(
                SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurCorrige, nombreMark));
        lecteurCorrige.close();
        expectedResultsASMarkPVal.verifieTailles(nombrePoints, nombreMark);

        WHEN("Sambada is run to compute the spatial autocorrelation")
        {
            CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnv + " " + fileNameMark);
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
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASEnv, nombreVariablesEnv));
                    SambadaSpatialAutocorrelationResults resultsASEnvPVal(
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASEnvPVal, nombreVariablesEnv));
                    SambadaSpatialAutocorrelationResults resultsASMark(
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASMark, nombreMark));
                    SambadaSpatialAutocorrelationResults resultsASMarkPVal(
                            SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(lecteurOutASMarkPVal, nombreMark));

                    THEN("the results match the expectations")
                    {
                        INFO("Verifying results env");
                        resultsASEnv.verifieTailles(nombrePoints, nombreVariablesEnv);
                        resultsASEnv.compare(expectedResultsASEnv);

                        INFO("Verifying p-values env");
                        resultsASEnvPVal.verifieTailles(nombrePoints, nombreVariablesEnv);
                        resultsASEnvPVal.comparePValeurs(expectedResultsASEnvPVal);

                        INFO("Verifying results mark");
                        resultsASMark.verifieTailles(nombrePoints, nombreMark);
                        resultsASMark.compare(expectedResultsASMark);

                        INFO("Verifying results p-values mark");
                        resultsASMarkPVal.verifieTailles(nombrePoints, nombreMark);
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

