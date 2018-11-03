#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

SCENARIO(
        "Test that regression results are correct when the molecular data is split in several files with filename extensions",
        "[split-input-files-with-extensions-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Molecular data split in several files with extensions to their names")
    {
        // Common declarations

        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/splitInputFilesIntTests/");

        std::string pathToOutputFolder("./test/integration/sambada/splitInputFilesIntTests/");

        std::string fileNameEnv(pathToInputFolder + "choice-env-cattle.csv");

        // Expected results

        std::string fileNameExpectedResultsDim0FirstBatch(pathToInputFolder + "expected-results-cattle-dim-0-0-0.txt");
        std::string fileNameExpectedResultsDim1FirstBatch(pathToInputFolder + "expected-results-cattle-dim-1-0-0.txt");

        std::string fileNameExpectedResultsDim0SecondBatch(pathToInputFolder + "expected-results-cattle-dim-0-1-3.txt");
        std::string fileNameExpectedResultsDim1SecondBatch(pathToInputFolder + "expected-results-cattle-dim-1-1-3.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResultsDim0FirstBatch.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedNullResultsFirstBatch(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
        lecteurCorrige.close();
        expectedNullResultsFirstBatch.verifieTailles(true, 0, 3);

        lecteurCorrige.open(fileNameExpectedResultsDim1FirstBatch.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsFirstBatch(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
        lecteurCorrige.close();
        expectedResultsFirstBatch.verifieTailles(true, 1, 18);

        lecteurCorrige.open(fileNameExpectedResultsDim0SecondBatch.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedNullResultsSecondBatch(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
        lecteurCorrige.close();
        expectedNullResultsSecondBatch.verifieTailles(true, 0, 2);

        lecteurCorrige.open(fileNameExpectedResultsDim1SecondBatch.c_str());
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsSecondBatch(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
        lecteurCorrige.close();
        expectedResultsSecondBatch.verifieTailles(true, 1, 12);

        // Output files with extensions

        std::string fileNameOut0FirstBatch(pathToOutputFolder + "choice-mark-cattle-mark-0-0-Out-0.txt");
        std::string fileNameOut0SecondBatch(pathToOutputFolder + "choice-mark-cattle-mark-1-3-Out-0.txt");

        std::string fileNameOut1FirstBatch(pathToOutputFolder + "choice-mark-cattle-mark-0-0-Out-1.txt");
        std::string fileNameOut1SecondBatch(pathToOutputFolder + "choice-mark-cattle-mark-1-3-Out-1.txt");

        std::string fileNameLogsFirstBatch(pathToOutputFolder + "choice-mark-cattle-mark-0-0-log.txt");
        std::string fileNameLogsSecondBatch(pathToOutputFolder + "choice-mark-cattle-mark-1-3-log.txt");

        // Output files without extensions

        std::string fileNameOut0FirstBatchOutputFileNoExt(pathToOutputFolder + "choice-mark-cattle-mark-0-0-Out-0");
        std::string fileNameOut0SecondBatchOutputFileNoExt(pathToOutputFolder + "choice-mark-cattle-mark-1-3-Out-0");

        std::string fileNameOut1FirstBatchOutputFileNoExt(pathToOutputFolder + "choice-mark-cattle-mark-0-0-Out-1");
        std::string fileNameOut1SecondBatchOutputFileNoExt(pathToOutputFolder + "choice-mark-cattle-mark-1-3-Out-1");

        std::string fileNameLogsFirstBatchOutputFileNoExt(pathToOutputFolder + "choice-mark-cattle-mark-0-0-log");
        std::string fileNameLogsSecondBatchOutputFileNoExt(pathToOutputFolder + "choice-mark-cattle-mark-1-3-log");

        // Testing with param outputfile with extension

        std::string fileNameParamsFirstBatch(pathToInputFolder + "param-0-0-outputfile-with-ext.txt");
        std::string fileNameParamsSecondBatch(pathToInputFolder + "param-1-3-outputfile-with-ext.txt");

        std::string fileNameMarkFirstBatch(pathToInputFolder + "choice-mark-cattle-mark-0-0.txt");
        std::string fileNameMarkSecondBatch(pathToInputFolder + "choice-mark-cattle-mark-1-3.txt");

        // Testing with param outputfile without extension
        std::string fileNameParamsFirstBatchOutputFileNoExt(pathToInputFolder + "param-0-0-outputfile-no-ext.txt");
        std::string fileNameParamsSecondBatchOutputFileNoExt(pathToInputFolder + "param-1-3-outputfile-no-ext.txt");

        // Testing without param outputfile
        std::string fileNameParamsFirstBatchNoOutputFile(pathToInputFolder + "param-0-0-no-outputfile.txt");
        std::string fileNameParamsSecondBatchNoOutputFile(pathToInputFolder + "param-1-3-no-outputfile.txt");

        std::string fileNameMarkFirstBatchNoOutputFile(pathToOutputFolder + "choice-mark-cattle-mark-0-0.txt");
        std::string fileNameMarkSecondBatchNoOutputFile(pathToOutputFolder + "choice-mark-cattle-mark-1-3.txt");

        WHEN("Sambada runs the first batch of analyses using outputfile param with extension")
        {

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
                        resultsDim0.compare(expectedNullResultsFirstBatch);

                        resultsDim1.verifieTailles(true, 1, 18);
                        resultsDim1.compare(expectedResultsFirstBatch);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();

                std::remove(fileNameLogsFirstBatch.c_str());
                std::remove(fileNameOut0FirstBatch.c_str());
                std::remove(fileNameOut1FirstBatch.c_str());
            }
        }

        WHEN("Sambada runs the second batch of analyses using outputfile param with extension")
        {

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
                        resultsDim0.compare(expectedNullResultsSecondBatch);

                        resultsDim1.verifieTailles(true, 1, 12);
                        resultsDim1.compare(expectedResultsSecondBatch);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();

                std::remove(fileNameLogsSecondBatch.c_str());
                std::remove(fileNameOut0SecondBatch.c_str());
                std::remove(fileNameOut1SecondBatch.c_str());
            }
        }

        WHEN("Sambada runs the first batch of analyses using outputfile param without extension")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParamsFirstBatchOutputFileNoExt + " " + fileNameEnv + " " +
                    fileNameMarkFirstBatch);
            INFO(output);

            THEN("the output files are found")
            {
                std::ifstream lecteurOut0(fileNameOut0FirstBatchOutputFileNoExt.c_str());
                REQUIRE(lecteurOut0.good());
                REQUIRE(lecteurOut0.is_open());

                std::ifstream lecteurOut1(fileNameOut1FirstBatchOutputFileNoExt.c_str());
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
                        resultsDim0.compare(expectedNullResultsFirstBatch);

                        resultsDim1.verifieTailles(true, 1, 18);
                        resultsDim1.compare(expectedResultsFirstBatch);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();

                std::remove(fileNameLogsFirstBatchOutputFileNoExt.c_str());
                std::remove(fileNameOut0FirstBatchOutputFileNoExt.c_str());
                std::remove(fileNameOut1FirstBatchOutputFileNoExt.c_str());
            }
        }

        WHEN("Sambada runs the second batch of analyses using outputfile param without extension")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParamsSecondBatchOutputFileNoExt + " " + fileNameEnv + " " +
                    fileNameMarkSecondBatch);
            INFO(output);

            THEN("the output files are found")
            {
                std::ifstream lecteurOut0(fileNameOut0SecondBatchOutputFileNoExt.c_str());
                REQUIRE(lecteurOut0.good());
                REQUIRE(lecteurOut0.is_open());

                std::ifstream lecteurOut1(fileNameOut1SecondBatchOutputFileNoExt.c_str());
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
                        resultsDim0.compare(expectedNullResultsSecondBatch);

                        resultsDim1.verifieTailles(true, 1, 12);
                        resultsDim1.compare(expectedResultsSecondBatch);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();

                std::remove(fileNameLogsSecondBatchOutputFileNoExt.c_str());
                std::remove(fileNameOut0SecondBatchOutputFileNoExt.c_str());
                std::remove(fileNameOut1SecondBatchOutputFileNoExt.c_str());
            }
        }

        WHEN("Sambada runs the first batch of analyses without the outputfile param")
        {
            SambadaIntegrationTestUtils::copyFileAndUpdatePermissions(fileNameMarkFirstBatch, fileNameMarkFirstBatchNoOutputFile);

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParamsFirstBatchNoOutputFile + " " + fileNameEnv + " " +
                    fileNameMarkFirstBatchNoOutputFile);
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
                        resultsDim0.compare(expectedNullResultsFirstBatch);

                        resultsDim1.verifieTailles(true, 1, 18);
                        resultsDim1.compare(expectedResultsFirstBatch);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();

                std::remove(fileNameLogsFirstBatch.c_str());
                std::remove(fileNameOut0FirstBatch.c_str());
                std::remove(fileNameOut1FirstBatch.c_str());

                std::remove(fileNameMarkFirstBatchNoOutputFile.c_str());
            }
        }

        WHEN("Sambada runs the second batch of analyses without the outputfile param")
        {
            SambadaIntegrationTestUtils::copyFileAndUpdatePermissions(fileNameMarkSecondBatch, fileNameMarkSecondBatchNoOutputFile);

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParamsSecondBatchNoOutputFile + " " + fileNameEnv + " " +
                    fileNameMarkSecondBatchNoOutputFile);
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
                        resultsDim0.compare(expectedNullResultsSecondBatch);

                        resultsDim1.verifieTailles(true, 1, 12);
                        resultsDim1.compare(expectedResultsSecondBatch);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();

                std::remove(fileNameLogsSecondBatch.c_str());
                std::remove(fileNameOut0SecondBatch.c_str());
                std::remove(fileNameOut1SecondBatch.c_str());

                std::remove(fileNameMarkSecondBatchNoOutputFile.c_str());
            }
        }
    }
}

