#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that regression results are correct when there are no new lines at the end of data files", "[no-newlines-eof-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Some input files with or without a new line at the end")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/noNewLinesEndOfFilesIntTests/");
        std::string fileNameOut0(pathToOutputFolder + "choice-mark-cattle-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "choice-mark-cattle-Out-1.txt");
        std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameLogs});

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/noNewLinesEndOfFilesIntTests/");

        std::string fileNameParams(pathToInputFolder + "param-with-newline-eof.txt");
        std::string fileNameParamsNoNewLine(pathToInputFolder + "param-no-newline-eof.txt");

        std::string fileNameEnv(pathToInputFolder + "choice-env-cattle.csv");
        std::string fileNameEnvNoNewLine(pathToInputFolder + "choice-env-cattle-no-newline-eof.csv");

        std::string fileNameMark(pathToInputFolder + "choice-mark-cattle.txt");
        std::string fileNameMarkNoNewLine(pathToInputFolder + "choice-mark-cattle-no-newline-eof.txt");

        std::string fileNameExpectedResultsDim0(pathToInputFolder + "expected-results-cattle-dim-0.txt");
        std::string fileNameExpectedResultsDim1(pathToInputFolder + "expected-results-cattle-dim-1.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResultsDim0.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedNullResults(SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
        lecteurCorrige.close();
        expectedNullResults.verifieTailles(true, 0, 5);

        lecteurCorrige.open(fileNameExpectedResultsDim1.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResults(SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
        lecteurCorrige.close();
        expectedResults.verifieTailles(true, 1, 30);

        CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

        WHEN("Sambada is run using the input files with new lines at the end")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnv + " " + fileNameMark);
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

        WHEN("Sambada is run using the parameter file without the new line at the end")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParamsNoNewLine + " " + fileNameEnv + " " + fileNameMark);
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

        WHEN("Sambada is run using the environmental file without the new line at the end")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnvNoNewLine + " " + fileNameMark);
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

        WHEN("Sambada is run using the molecular file without the new line at the end")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnv + " " + fileNameMarkNoNewLine);
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

        SambadaIntegrationTestUtils::removeFiles(outputFileNames);
    }
}

