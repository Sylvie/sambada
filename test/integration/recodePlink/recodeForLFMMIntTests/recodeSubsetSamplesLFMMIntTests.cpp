#include "catch.hpp"
#include "SambadaInputData.hpp"
#include "SambadaIntegrationTestUtils.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that RecodeLFMM recodes data correctly when a subset of samples is selected",
         "[recode-lfmm-data-subset-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("An input file to split")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/recode-plink-lfmm"));

	    std::string pathToTestFolder("test/integration/recodePlink/recodeForLFMMIntTests/");

	    std::string pathToOutputFolder("./" + pathToTestFolder);

        std::string fileNameRecodedData(pathToOutputFolder + "testPlink.txt");

        std::vector<std::string> outputFileNames({fileNameRecodedData});

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() + pathToTestFolder);

        std::string fileNamePlinkData(pathToInputFolder + "testPlink");
	    std::string fileNameSubsetSamples(pathToInputFolder + "selected-sample-names.txt");

	    std::string fileNameExpectedOutput(pathToInputFolder + "expected-recoded-data-lfmm-subset-samples.txt");

	    int nbIndiv(786), nbIndivSelectionnes(136), nbLignes(nbIndivSelectionnes), nbSnp(5), nbColonnesRecodees(5);

        std::ifstream lecteurCorrige(fileNameExpectedOutput.c_str());
        INFO("Reading expected recoded data");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaInputData expectedRecodedData(
                SambadaIntegrationTestUtils::readInputData(lecteurCorrige, nbColonnesRecodees));
        lecteurCorrige.close();
	    expectedRecodedData.verifieTailles(nbLignes, nbColonnesRecodees);

	    WHEN("RecodePlink is run")
        {
            CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

	        std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + std::to_string(nbIndiv) + " " + std::to_string(nbSnp) + " " +
                    fileNamePlinkData + " " + fileNameRecodedData + " " + fileNameSubsetSamples);
            //INFO(output);

            THEN("the output files are found")
            {
                INFO("Reading results");
                std::ifstream lecteurOut(fileNameRecodedData.c_str());
                REQUIRE(lecteurOut.good());
                REQUIRE(lecteurOut.is_open());

	            AND_WHEN("the output files are read")
                {
                    SambadaInputData recodedData(
                            SambadaIntegrationTestUtils::readInputData(lecteurOut, nbColonnesRecodees));

	                THEN("the results match the expectations")
                    {
                        INFO("Verifying recoded data");
	                    recodedData.verifieTailles(nbLignes, nbColonnesRecodees);
	                    recodedData.compare(expectedRecodedData);

                    }
                }

	            lecteurOut.close();
            }
        }

        SambadaIntegrationTestUtils::removeFiles(outputFileNames);
    }
}

