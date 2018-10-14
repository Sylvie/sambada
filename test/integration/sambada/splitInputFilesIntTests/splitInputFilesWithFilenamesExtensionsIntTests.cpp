#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that regression results are correct when the molecular data is split in several files with filename extensions",
         "[split-input-files-with-extensions-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Molecular data split in several files with extensions to their names")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/splitInputFilesIntTests/");

        std::string fileNameOut0FirstBatch(pathToOutputFolder + "choice-mark-cattle-mark-0-0-Out-0.txt");
        std::string fileNameOut0SecondBatch(pathToOutputFolder + "choice-mark-cattle-mark-1-3-Out-0.txt");

        std::string fileNameOut1FirstBatch(pathToOutputFolder + "choice-mark-cattle-mark-0-0-Out-1.txt");
        std::string fileNameOut1SecondBatch(pathToOutputFolder + "choice-mark-cattle-mark-1-3-Out-1.txt");

        std::string fileNameLogsFirstBatch(pathToOutputFolder + "choice-mark-cattle-mark-0-0-log.txt");
        std::string fileNameLogsSecondBatch(pathToOutputFolder + "choice-mark-cattle-mark-1-3-log.txt");

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/splitInputFilesIntTests/");

        std::string fileNameParamsFirstBatch(pathToInputFolder + "param-0-0.txt");
        std::string fileNameParamsSecondBatch(pathToInputFolder + "param-1-3.txt");

        std::string fileNameEnv(pathToInputFolder + "choice-env-cattle.csv");

        std::string fileNameMarkFirstBatch(pathToInputFolder + "choice-mark-cattle-mark-0-0.txt");
        std::string fileNameMarkSecondBatch(pathToInputFolder + "choice-mark-cattle-mark-1-3.txt");

        std::string fileNameExpectedResultsDim0FirstBatch(pathToInputFolder + "expected-results-cattle-dim-0-0-0.txt");
        std::string fileNameExpectedResultsDim1FirstBatch(pathToInputFolder + "expected-results-cattle-dim-1-0-0.txt");

        std::string fileNameExpectedResultsDim0SecondBatch(pathToInputFolder + "expected-results-cattle-dim-0-1-3.txt");
        std::string fileNameExpectedResultsDim1SecondBatch(pathToInputFolder + "expected-results-cattle-dim-1-1-3.txt");

        WHEN("Sambada runs the first batch of analyses")
        {
            std::ifstream lecteurCorrige(fileNameExpectedResultsDim0FirstBatch.c_str());
            REQUIRE(lecteurCorrige.good());
            REQUIRE(lecteurCorrige.is_open());
            SambadaRegressionResults expectedNullResults(
                    SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
            lecteurCorrige.close();
            expectedNullResults.verifieTailles(true, 0, 3);

            lecteurCorrige.open(fileNameExpectedResultsDim1FirstBatch.c_str());
            REQUIRE(lecteurCorrige.good());
            REQUIRE(lecteurCorrige.is_open());
            SambadaRegressionResults expectedResults(
                    SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
            lecteurCorrige.close();
            expectedResults.verifieTailles(true, 1, 18);

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParamsFirstBatch + " " + fileNameEnv + " " + fileNameMarkFirstBatch);
            INFO(output);

            THEN("the output files are found")
            {
                std::ifstream lecteurOut0(fileNameOut0FirstBatch.c_str());
                REQUIRE(lecteurOut0.good());
                REQUIRE(lecteurOut0.is_open());

                std::ifstream lecteurOut1(fileNameOut1FirstBatch.c_str());
                REQUIRE(lecteurOut1.good());
                REQUIRE(lecteurOut1.is_open());

                AND_WHEN("the output files are read")
                {
                    SambadaRegressionResults resultsDim0(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut0, true, 0));
                    SambadaRegressionResults resultsDim1(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut1, true, 1));

                    THEN("the results match the expectations")
                    {
                        resultsDim0.verifieTailles(true, 0, 3);
                        resultsDim0.compare(expectedNullResults);

                        resultsDim1.verifieTailles(true, 1, 18);
                        resultsDim1.compare(expectedResults);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();

                std::remove(fileNameLogsFirstBatch.c_str());
                std::remove(fileNameOut0FirstBatch.c_str());
                std::remove(fileNameOut1FirstBatch.c_str());
            }
        }

        WHEN("Sambada runs the second batch of analyses")
        {
            std::ifstream lecteurCorrige(fileNameExpectedResultsDim0SecondBatch.c_str());
            REQUIRE(lecteurCorrige.good());
            REQUIRE(lecteurCorrige.is_open());
            SambadaRegressionResults expectedNullResults(
                    SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
            lecteurCorrige.close();
            expectedNullResults.verifieTailles(true, 0, 2);

            lecteurCorrige.open(fileNameExpectedResultsDim1SecondBatch.c_str());
            REQUIRE(lecteurCorrige.good());
            REQUIRE(lecteurCorrige.is_open());
            SambadaRegressionResults expectedResults(
                    SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
            lecteurCorrige.close();
            expectedResults.verifieTailles(true, 1, 12);

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParamsSecondBatch + " " + fileNameEnv + " " + fileNameMarkSecondBatch);
            INFO(output);

            THEN("the output files are found")
            {
                std::ifstream lecteurOut0(fileNameOut0SecondBatch.c_str());
                REQUIRE(lecteurOut0.good());
                REQUIRE(lecteurOut0.is_open());

                std::ifstream lecteurOut1(fileNameOut1SecondBatch.c_str());
                REQUIRE(lecteurOut1.good());
                REQUIRE(lecteurOut1.is_open());

                AND_WHEN("the output files are read")
                {
                    SambadaRegressionResults resultsDim0(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut0, true, 0));
                    SambadaRegressionResults resultsDim1(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut1, true, 1));

                    THEN("the results match the expectations")
                    {
                        resultsDim0.verifieTailles(true, 0, 2);
                        resultsDim0.compare(expectedNullResults);

                        resultsDim1.verifieTailles(true, 1, 12);
                        resultsDim1.compare(expectedResults);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();

                std::remove(fileNameLogsSecondBatch.c_str());
                std::remove(fileNameOut0SecondBatch.c_str());
                std::remove(fileNameOut1SecondBatch.c_str());

            }
        }
    }
}

