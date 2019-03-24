#include "catch.hpp"
#include "SambadaInputData.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that Supervision splits input files correctly",
         "[input-files-splitting-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("An input file to split")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/supervision"));

	    std::string pathToTestFolder("test/integration/supervision/inputFileSplittingIntTests/");

	    std::string pathToOutputFolder("./" + pathToTestFolder);

        std::string fileNameOutEnv(pathToOutputFolder + "mol-data-env.txt");
        std::string fileNameOutMark0(pathToOutputFolder + "mol-data-mark-0-0.txt");
        std::string fileNameOutMark1(pathToOutputFolder + "mol-data-mark-1-4.txt");

        std::vector<std::string> outputFileNames({fileNameOutEnv, fileNameOutMark0, fileNameOutMark1});

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() + pathToTestFolder);

        std::string bareFileNameParams("param-split.txt");

#if defined(_WIN32) || defined(_WIN64)
        // FIXME: SMB-41: Supervision utilise des "\" par défaut sur Windows
		bareFileNameParams = "param-split-windows.txt";
#endif
        std::string fileNameParams(pathToInputFolder + bareFileNameParams);
        std::string fileNameMark(pathToInputFolder + "mol-data.txt");

	    std::string fileNameParamInOutputFolder(pathToOutputFolder + bareFileNameParams);
	    std::string fileNameMarkInOutputFolder(pathToOutputFolder + "mol-data.txt");
	    outputFileNames.insert(outputFileNames.end(), {fileNameParamInOutputFolder, fileNameMarkInOutputFolder});

	    std::string fileNameExpectedDataEnv(pathToInputFolder + "expected-env.txt");
        std::string fileNameExpectedDataMark0(pathToInputFolder + "expected-mark-0-0.txt");
	    std::string fileNameExpectedDataMark1(pathToInputFolder + "expected-mark-1-4.txt");

        std::ifstream lecteurCorrige(fileNameExpectedDataEnv.c_str());
        INFO("Reading expected env data");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaInputData expectedDataEnv(
                SambadaIntegrationTestUtils::readInputData(lecteurCorrige, 1));
        lecteurCorrige.close();
	    expectedDataEnv.verifieTailles(7, 1);

        lecteurCorrige.open(fileNameExpectedDataMark0.c_str());
        INFO("Reading expected molecular data (batch 0)");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
	    SambadaInputData expectedDataMark0(
                SambadaIntegrationTestUtils::readInputData(lecteurCorrige, 4));
        lecteurCorrige.close();
	    expectedDataMark0.verifieTailles(7,4);

        lecteurCorrige.open(fileNameExpectedDataMark1.c_str());
	    INFO("Reading expected molecular data (batch 1)");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
	    SambadaInputData expectedDataMark1(
                SambadaIntegrationTestUtils::readInputData(lecteurCorrige, 3));
        lecteurCorrige.close();
	    expectedDataMark1.verifieTailles(7,3);

        WHEN("Supervision is run")
        {
            CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

	        SambadaIntegrationTestUtils::copyFileAndUpdatePermissions(fileNameParams, fileNameParamInOutputFolder);
	        SambadaIntegrationTestUtils::copyFileAndUpdatePermissions(fileNameMark, fileNameMarkInOutputFolder);

	        std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParamInOutputFolder);
            INFO(output);

            THEN("the output files are found")
            {
                INFO("Reading env data");
                std::ifstream lecteurOutEnv(fileNameOutEnv.c_str());
                REQUIRE(lecteurOutEnv.good());
                REQUIRE(lecteurOutEnv.is_open());

                INFO("Reading molecular data (batch 0)");
                std::ifstream lecteurOutMark0(fileNameOutMark0.c_str());
                REQUIRE(lecteurOutMark0.good());
                REQUIRE(lecteurOutMark0.is_open());

                INFO("Reading molecular data (batch 1)");
                std::ifstream lecteurOutMark1(fileNameOutMark1.c_str());
                REQUIRE(lecteurOutMark1.good());
                REQUIRE(lecteurOutMark1.is_open());


	            AND_WHEN("the output files are read")
                {
                    SambadaInputData dataEnv(
                            SambadaIntegrationTestUtils::readInputData(lecteurOutEnv, 1));
	                SambadaInputData dataMark0(
                            SambadaIntegrationTestUtils::readInputData(lecteurOutMark0, 4));
	                SambadaInputData dataMark1(
                            SambadaIntegrationTestUtils::readInputData(lecteurOutMark1, 3));

	                THEN("the results match the expectations")
                    {
                        INFO("Verifying env data");
                        dataEnv.verifieTailles(7,1);
	                    dataEnv.compare(expectedDataEnv);

                        INFO("Verifying molecular data (batch 0)");
	                    dataMark0.verifieTailles(7,4);
	                    dataMark0.compare(expectedDataMark0);

	                    INFO("Verifying molecular data (batch 1)");
	                    dataMark1.verifieTailles(7,3);
	                    dataMark1.compare(expectedDataMark1);
                    }
                }

                lecteurOutEnv.close();
                lecteurOutMark0.close();
                lecteurOutMark1.close();
            }
        }

        SambadaIntegrationTestUtils::removeFiles(outputFileNames);
    }
}

