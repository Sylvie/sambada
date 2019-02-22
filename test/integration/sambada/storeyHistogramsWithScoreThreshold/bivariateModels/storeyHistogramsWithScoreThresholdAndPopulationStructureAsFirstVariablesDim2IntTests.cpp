#include "catch.hpp"
#include "SambadaStoreyHistogram.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that Storey's p-values histograms are correct with score threshold and population structure as first variables for bivariate models",
		"[storey-histograms-with-score-threshold-and-pop-structure-as-first-variables-dim-2-int][storey-histograms-threshold-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("A program to run, its expected output file, and the file of expected results")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToTestFolder("test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/");

        std::string pathToOutputFolder("./" +  pathToTestFolder);
        std::string fileNameOut0(pathToOutputFolder + "cattle-pop-mark-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "cattle-pop-mark-Out-1.txt");
        std::string fileNameOut2(pathToOutputFolder + "cattle-pop-mark-Out-2.txt");
	    std::string fileNameHistogram(pathToOutputFolder + "cattle-pop-mark-storey.txt");
	    std::string fileNameLogs(pathToOutputFolder + "cattle-pop-mark-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameOut2, fileNameHistogram, fileNameLogs});

        std::string pathToInputFolder(SambadaIntegrationTestUtils::getTopSourceDirectory() + pathToTestFolder);

        std::string fileNameParam(pathToInputFolder + "param-with-pop-structure-as-first-variables-dim-2.txt");
        std::string fileNameEnv(pathToInputFolder + "../cattle-pop-env-first.csv");
        std::string fileNameMark(pathToInputFolder + "../cattle-pop-mark.txt");

	    std::string fileNameExpectedResults(pathToInputFolder + "expected-storey-histograms-with-pop-structure-as-first-variables-dim-2.txt");

	    int numberHistograms(10);

        std::ifstream lecteurCorrige(fileNameExpectedResults.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaStoreyHistogram expectedResults(SambadaIntegrationTestUtils::readStoreyHistogram(lecteurCorrige));
        lecteurCorrige.close();
	    expectedResults.verifieTailles(numberHistograms);

        CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

        WHEN("the program is run")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(program + " " + fileNameParam + " " + fileNameEnv + " " + fileNameMark);
            //INFO(output);

            THEN("the output file with the histograms is found")
            {
                std::ifstream lecteur(fileNameHistogram.c_str());

                REQUIRE(lecteur.good());
                REQUIRE(lecteur.is_open());

                AND_WHEN("the file is read")
                {
                    SambadaStoreyHistogram results(
                            SambadaIntegrationTestUtils::readStoreyHistogram(lecteur));

                    THEN("the results match the expectation")
                    {
                        results.verifieTailles(numberHistograms);
                        results.compare(expectedResults);
                    }
                }

                lecteur.close();
            }

            SambadaIntegrationTestUtils::removeFiles(outputFileNames);
        }
    }
}

