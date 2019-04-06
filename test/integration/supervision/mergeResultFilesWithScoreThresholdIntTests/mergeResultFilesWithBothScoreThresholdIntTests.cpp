#include "catch.hpp"
#include "SambadaRegressionResults.hpp"
#include "SambadaIntegrationTestUtils.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that Supervision merges results correctly with a both score threshold",
         "[merge-result-files-both-score-threshold-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Some result files from Sambada")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/supervision"));

        std::string pathToTestFolder("test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/");

        std::string pathToOutputFolder("./" + pathToTestFolder);

        std::string fileNameOut0(pathToOutputFolder + "mol-data-res-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "mol-data-res-1.txt");
        std::string fileNameOut2(pathToOutputFolder + "mol-data-res-2.txt");
        std::string fileNameOut3(pathToOutputFolder + "mol-data-res-3.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameOut2, fileNameOut3});

        CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() + pathToTestFolder);

        std::string fileNameMark(pathToOutputFolder + "mol-data.txt");
        int numBlocs(3), tailleBlocs(12), dimensionMax(3);
        std::string scoreThresholdType = "both";
        int scoreThreshold(10);

        // Copie des fichiers de résultats dans le dossier de travail
        for (int i(0); i <= dimensionMax; ++i)
        {
            for (int j(0); j < numBlocs; ++j)
            {
                std::string fileName("mol-data-mark-" + std::to_string(j) + "-" +
                std::to_string(j*tailleBlocs) + "-Out-" + std::to_string(i) + ".txt");
                SambadaIntegrationTestUtils::copyFileAndUpdatePermissions(pathToInputFolder + fileName, pathToOutputFolder + fileName);
                outputFileNames.push_back(pathToOutputFolder + fileName);
            }
        }

        std::string fileNameExpectedResultsDim0(pathToInputFolder + "expected-results-dim-0.txt");
        std::string fileNameExpectedResultsDim1(pathToInputFolder + "expected-results-both-dim-1.txt");
        std::string fileNameExpectedResultsDim2(pathToInputFolder + "expected-results-both-dim-2.txt");
        std::string fileNameExpectedResultsDim3(pathToInputFolder + "expected-results-both-dim-3.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResultsDim0.c_str());
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
        expectedResultsDim1.verifieTailles(true, 1, 128);

        lecteurCorrige.open(fileNameExpectedResultsDim2.c_str());
        INFO("Reading expected results dim 2");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsDim2(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 2));
        lecteurCorrige.close();
        expectedResultsDim2.verifieTailles(true, 2, 89);

        lecteurCorrige.open(fileNameExpectedResultsDim3.c_str());
        INFO("Reading expected results dim 3");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsDim3(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 3));
        lecteurCorrige.close();
        expectedResultsDim3.verifieTailles(true, 3, 27);

        WHEN("Supervision is run on the split result files")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameMark + " " +
                    std::to_string(numBlocs) + " " +
                    std::to_string(tailleBlocs) + " " + std::to_string(dimensionMax) + " " +
                    scoreThresholdType + " " + std::to_string(scoreThreshold));
            //INFO(output);

            THEN("the output files are found")
            {
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
                    SambadaRegressionResults resultsDim0(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut0, true, 0));
                    SambadaRegressionResults resultsDim1(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut1, true, 1));
                    SambadaRegressionResults resultsDim2(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut2, true, 2));
                    SambadaRegressionResults resultsDim3(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut3, true, 3));

                    THEN("the results match the expectations")
                    {
                        INFO("Verifying results dim 0");
                        resultsDim0.verifieTailles(true, 0, 30);
                        resultsDim0.compare(expectedNullResults);

                        INFO("Verifying results dim 1");
                        resultsDim1.verifieTailles(true, 1, 128);
                        resultsDim1.compare(expectedResultsDim1);

                        INFO("Verifying results dim 2");
                        resultsDim2.verifieTailles(true, 2, 89);
                        resultsDim2.compare(expectedResultsDim2);

                        INFO("Verifying results dim 3");
                        resultsDim3.verifieTailles(true, 3, 27);
                        resultsDim3.compare(expectedResultsDim3);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();
                lecteurOut2.close();
                lecteurOut3.close();
            }
        }

       SambadaIntegrationTestUtils::removeFiles(outputFileNames);
    }
}

