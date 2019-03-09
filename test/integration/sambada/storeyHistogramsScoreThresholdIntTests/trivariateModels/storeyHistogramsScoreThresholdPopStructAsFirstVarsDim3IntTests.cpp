#include "catch.hpp"
#include "SambadaStoreyHistogram.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that Storey's p-values histograms are correct with score threshold, \"SAVETYPE REAL\" and population structure as first variables for trivariate models",
		"[storey-histograms-score-threshold-savetype-real-pop-structure-as-first-variables-int][storey-histograms-threshold-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("A program to run, its expected output file, and the file of expected results")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToTestFolder("test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/");

        std::string pathToOutputFolder("./" + pathToTestFolder);
        std::string fileNameOut0(pathToOutputFolder + "cattle-pop-mark-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "cattle-pop-mark-Out-1.txt");
        std::string fileNameOut2(pathToOutputFolder + "cattle-pop-mark-Out-2.txt");
        std::string fileNameOut3(pathToOutputFolder + "cattle-pop-mark-Out-3.txt");
	    std::string fileNameHistogram(pathToOutputFolder + "cattle-pop-mark-storey.txt");
	    std::string fileNameLogs(pathToOutputFolder + "cattle-pop-mark-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameOut2, fileNameOut3, fileNameHistogram, fileNameLogs});

        std::string pathToInputFolder(SambadaIntegrationTestUtils::getTopSourceDirectory() + pathToTestFolder);

        std::string fileNameParam(pathToInputFolder + "param-with-pop-structure-as-first-variables-dim-3.txt");
        std::string fileNameEnv(pathToInputFolder + "../cattle-pop-env-first.csv");
        std::string fileNameMark(pathToInputFolder + "../cattle-pop-mark.txt");

	    std::string fileNameExpectedResults(pathToInputFolder + "expected-storey-histograms-with-pop-structure-as-first-variables-dim-3.txt");
        std::string fileNameExpectedResultsDim0(pathToInputFolder + "expected-results-no-pop-dim-0.txt");
        std::string fileNameExpectedResultsDim1(pathToInputFolder + "expected-results-pop-first-dim-1.txt");
        std::string fileNameExpectedResultsDim2(pathToInputFolder + "expected-results-pop-first-dim-2.txt");
        std::string fileNameExpectedResultsDim3(pathToInputFolder + "expected-results-pop-first-dim-3.txt");

	    int numberHistograms(14);

        std::ifstream lecteurCorrige(fileNameExpectedResults.c_str());
        INFO("Reading expected histograms");
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

        lecteurCorrige.open(fileNameExpectedResultsDim2.c_str());
        INFO("Reading expected results dim 2");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsDim2(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 2));
        lecteurCorrige.close();
        expectedResultsDim2.verifieTailles(true, 2, 7);

        lecteurCorrige.open(fileNameExpectedResultsDim3.c_str());
        INFO("Reading expected results dim 3");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsDim3(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 3));
        lecteurCorrige.close();
        expectedResultsDim3.verifieTailles(true, 3, 40, true);

        CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

        WHEN("Sambada is run using the first variables as population structure")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(program + " " + fileNameParam + " " + fileNameEnv + " " + fileNameMark);
            //INFO(output);

            THEN("the output file with the histograms is found")
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

                INFO("Reading results dim 2");
                std::ifstream lecteurOut2(fileNameOut2.c_str());
                REQUIRE(lecteurOut2.good());
                REQUIRE(lecteurOut2.is_open());

                INFO("Reading results dim 3");
                std::ifstream lecteurOut3(fileNameOut3.c_str());
                REQUIRE(lecteurOut3.good());
                REQUIRE(lecteurOut3.is_open());

                AND_WHEN("the output files are read")
                {
                    SambadaStoreyHistogram results(
                            SambadaIntegrationTestUtils::readStoreyHistogram(lecteur));
                    SambadaRegressionResults resultsDim0(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut0, true, 0));
                    SambadaRegressionResults resultsDim1(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut1, true, 1));
                    SambadaRegressionResults resultsDim2(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut2, true, 2));
                    SambadaRegressionResults resultsDim3(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut3, true, 3));

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

                        INFO("Verifying results dim 2");
                        resultsDim2.verifieTailles(true, 2, 7);
                        resultsDim2.compare(expectedResultsDim2);

                        INFO("Verifying results dim 3");
                        resultsDim3.verifieTailles(true, 3, 40, true);
                        resultsDim3.compare(expectedResultsDim3);

                    }
                }

                lecteur.close();
                lecteurOut0.close();
                lecteurOut1.close();
                lecteurOut2.close();
                lecteurOut3.close();
            }

            SambadaIntegrationTestUtils::removeFiles(outputFileNames);
        }
    }
}

