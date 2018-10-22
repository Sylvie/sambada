#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that regression results are correct when there is a single data file and the filenames have no extensions",
         "[no-extensions-filenames-unique-data-file-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Some input files with or without extensions to their names")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/noExtensionsToFilenamesIntTests/");

        std::string fileNameOut0(pathToOutputFolder + "choice-data-cattle-Out-0.csv");
        std::string fileNameOut0NoExt(pathToOutputFolder + "choice-data-cattle-Out-0");

        std::string fileNameOut1(pathToOutputFolder + "choice-data-cattle-Out-1.csv");
        std::string fileNameOut1NoExt(pathToOutputFolder + "choice-data-cattle-Out-1");

        std::string fileNameLogs(pathToOutputFolder + "choice-data-cattle-log.csv");
        std::string fileNameLogsNoExt(pathToOutputFolder + "choice-data-cattle-log");

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/noExtensionsToFilenamesIntTests/");

        std::string fileNameParams(pathToInputFolder + "param-unique-data-file.txt");
        std::string fileNameParamsNoExt(pathToInputFolder + "param-unique-data-file");
        std::string fileNameParamsDataFileNoExt(pathToInputFolder + "param-unique-data-file-no-ext.txt");

        std::string fileNameData(pathToInputFolder + "choice-data-cattle.csv");
        std::string fileNameDataNoExt(pathToInputFolder + "choice-data-cattle");

        std::string fileNameExpectedResultsDim0(pathToInputFolder + "expected-results-cattle-dim-0.txt");
        std::string fileNameExpectedResultsDim1(pathToInputFolder + "expected-results-cattle-dim-1.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResultsDim0.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedNullResults(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
        lecteurCorrige.close();
        expectedNullResults.verifieTailles(true, 0, 5);

        lecteurCorrige.open(fileNameExpectedResultsDim1.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResults(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
        lecteurCorrige.close();
        expectedResults.verifieTailles(true, 1, 30);

        WHEN("Sambada is run using the input files with filename extensions")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameData);
            INFO(output);

            THEN("the output files are found")
            {
                std::ifstream lecteurOut0(fileNameOut0.c_str());
                REQUIRE(lecteurOut0.good());
                REQUIRE(lecteurOut0.is_open());

                std::ifstream lecteurOut1(fileNameOut1.c_str());
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
                        resultsDim0.verifieTailles(true, 0, 5);
                        resultsDim0.compare(expectedNullResults);

                        resultsDim1.verifieTailles(true, 1, 30);
                        resultsDim1.compare(expectedResults);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();
            }
        }

        WHEN("Sambada is run using the parameter file without the filename extension")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParamsNoExt + " " + fileNameData);
            INFO(output);

            // Checking if the last line of param was read
            std::size_t location(output.find("SAVETYPE"));
            CHECK(location != std::string::npos);
            std::istringstream iss(output.substr(location));
            std::string token("");
            iss >> token >> std::ws;
            CHECK(token == "SAVETYPE");
            iss >> token >> std::ws;
            CHECK(token == "END");
            iss >> token >> std::ws;
            CHECK(token == "BEST");
            iss >> token >> std::ws;
            CHECK(token == "0.01");

            THEN("the output files are found")
            {
                std::ifstream lecteurOut0(fileNameOut0.c_str());
                REQUIRE(lecteurOut0.good());
                REQUIRE(lecteurOut0.is_open());

                std::ifstream lecteurOut1(fileNameOut1.c_str());
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
                        resultsDim0.verifieTailles(true, 0, 5);
                        resultsDim0.compare(expectedNullResults);

                        resultsDim1.verifieTailles(true, 1, 30);
                        resultsDim1.compare(expectedResults);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();
            }
        }

        WHEN("Sambada is run using the data file without the filename extension")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameDataNoExt);
            INFO(output);

            THEN("the output files are found")
            {
                std::ifstream lecteurOut0(fileNameOut0.c_str());
                REQUIRE(lecteurOut0.good());
                REQUIRE(lecteurOut0.is_open());

                std::ifstream lecteurOut1(fileNameOut1.c_str());
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
                        resultsDim0.verifieTailles(true, 0, 5);
                        resultsDim0.compare(expectedNullResults);

                        resultsDim1.verifieTailles(true, 1, 30);
                        resultsDim1.compare(expectedResults);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();
            }
        }
    }
}

