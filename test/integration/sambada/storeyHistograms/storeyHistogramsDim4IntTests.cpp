#include "catch.hpp"
#include "SambadaStoreyHistogram.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that Storey's p-values histograms are correct for models of dimension 4",
		"[storey-histograms-dim-4-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("A program to run, its expected output file, and the file of expected results")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/storeyHistograms/");
        std::string fileNameOut0(pathToOutputFolder + "choice-mark-cattle-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "choice-mark-cattle-Out-1.txt");
        std::string fileNameOut2(pathToOutputFolder + "choice-mark-cattle-Out-2.txt");
        std::string fileNameOut3(pathToOutputFolder + "choice-mark-cattle-Out-3.txt");
        std::string fileNameOut4(pathToOutputFolder + "choice-mark-cattle-Out-4.txt");
	    std::string fileNameHistogram(pathToOutputFolder + "choice-mark-cattle-storey.txt");
	    std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameOut2, fileNameOut3, fileNameOut4, fileNameHistogram, fileNameLogs});

        std::string pathToInputFolder(SambadaIntegrationTestUtils::getTopSourceDirectory() + "test/integration/sambada/storeyHistograms/");

        std::string fileNameParam(pathToInputFolder + "param-dim-4.txt");
        std::string fileNameEnv(pathToInputFolder + "choice-env-cattle.csv");
        std::string fileNameMark(pathToInputFolder + "choice-mark-cattle.txt");

	    std::string fileNameExpectedResults(pathToInputFolder + "expected-storey-histograms-dim-4.txt");

	    int numberHistograms(16);

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

            THEN("the output file of dimension 0 is found")
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

