#include "catch.hpp"
#include "SambadaStoreyHistogram.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that Storey's p-values histograms are correct with score threshold and \"SAVETYPE REAL\"  for models of dimension 1",
        "[storey-histograms-score-threshold-savetype-real-dim-1-int][storey-histograms-threshold-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("A program to run, its expected output file, and the file of expected results")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToTestFolder("test/integration/sambada/storeyHistogramsScoreThresholdIntTests/univariateModels/");

        std::string pathToOutputFolder("./" + pathToTestFolder);
        std::string fileNameOut0(pathToOutputFolder + "cattle-pop-mark-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "cattle-pop-mark-Out-1.txt");
        std::string fileNameHistogram(pathToOutputFolder + "cattle-pop-mark-storey.txt");
        std::string fileNameLogs(pathToOutputFolder + "cattle-pop-mark-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameHistogram, fileNameLogs});

        std::string pathToInputFolder(SambadaIntegrationTestUtils::getTopSourceDirectory() + pathToTestFolder);

        std::string fileNameParam(pathToInputFolder + "param-dim-1.txt");
        std::string fileNameEnv(pathToInputFolder + "../cattle-pop-env-last.csv");
        std::string fileNameMark(pathToInputFolder + "../cattle-pop-mark.txt");

        std::string fileNameExpectedResultsStorey(pathToInputFolder + "expected-storey-histograms-dim-1.txt");
        std::string fileNameExpectedResultsDim0(pathToInputFolder + "expected-results-dim-0.txt");
        std::string fileNameExpectedResultsDim1(pathToInputFolder + "expected-results-dim-1.txt");

        int numberHistograms(4);

        std::ifstream lecteurCorrige(fileNameExpectedResultsStorey.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaStoreyHistogram expectedResults(SambadaIntegrationTestUtils::readStoreyHistogram(lecteurCorrige));
        lecteurCorrige.close();
        expectedResults.verifieTailles(numberHistograms);

        lecteurCorrige.open(fileNameExpectedResultsDim0.c_str());
        INFO("Reading expected results dim 0");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedNullResults(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
        lecteurCorrige.close();
        expectedNullResults.verifieTailles(true, 0, 30);

        lecteurCorrige.open(fileNameExpectedResultsDim1.c_str());
        INFO("Reading expected results dim 1");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsDim1(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
        lecteurCorrige.close();
        expectedResultsDim1.verifieTailles(true, 1, 151);

        CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

        WHEN("the program is run")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(program + " " + fileNameParam + " " + fileNameEnv + " " + fileNameMark);
            //INFO(output);

            THEN("the output files are found")
            {
                INFO("Reading histograms");
                std::ifstream lecteur(fileNameHistogram.c_str());
                REQUIRE(lecteur.good());
                REQUIRE(lecteur.is_open());

                INFO("Reading results dim 0");
                std::ifstream lecteurOut0(fileNameOut0.c_str());
                REQUIRE(lecteurOut0.good());
                REQUIRE(lecteurOut0.is_open());

                INFO("Reading results dim 1");
                std::ifstream lecteurOut1(fileNameOut1.c_str());
                REQUIRE(lecteurOut1.good());
                REQUIRE(lecteurOut1.is_open());

                AND_WHEN("the output files are read")
                {
                    SambadaStoreyHistogram results(
                            SambadaIntegrationTestUtils::readStoreyHistogram(lecteur));
                    SambadaRegressionResults resultsDim0(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut0, true, 0));
                    SambadaRegressionResults resultsDim1(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut1, true, 1));

                    THEN("the results match the expectation")
                    {
                        INFO("Verifying histograms");
                        results.verifieTailles(numberHistograms);
                        results.compare(expectedResults);

                        INFO("Verifying results dim 0");
                        resultsDim0.verifieTailles(true, 0, 30);
                        resultsDim0.compare(expectedNullResults);

                        INFO("Verifying results dim 1");
                        resultsDim1.verifieTailles(true, 1, 151);
                        resultsDim1.compare(expectedResultsDim1);
                    }
                }

                lecteur.close();
                lecteurOut0.close();
                lecteurOut1.close();
            }

            SambadaIntegrationTestUtils::removeFiles(outputFileNames);
        }
    }
}

