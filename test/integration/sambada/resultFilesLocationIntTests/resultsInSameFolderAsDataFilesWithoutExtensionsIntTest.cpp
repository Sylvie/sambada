#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

extern std::string infoPerms(fs::path path);

SCENARIO("Test that result files are writen in the same folder as the molecular data file when it has no extension to its filename", "[results-same-folder-molecular-data-without-extension-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("A program to run, its expected output file, and the file of expected results")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder(
                "./test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsMolecularDataNoExt/");
        std::string pathToOutputFolderWithoutLeadingDot(
                "test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsMolecularDataNoExt/");
        fs::create_directory(pathToOutputFolder.c_str());

        std::string fileNameOut0(pathToOutputFolder + "choice-mark-cattle-Out-0");
        std::string fileNameOut1(pathToOutputFolder + "choice-mark-cattle-Out-1");
        std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-log");

        // Defining filenames and path to original data
        std::string pathToInputFolder(SambadaIntegrationTestUtils::getTopSourceDirectory() +
                                      "test/integration/sambada/resultFilesLocationIntTests/");
        std::string bareFileNameParam("param-without-outputfile-setting.txt");
        std::string bareFileNameEnv("choice-env-cattle");
        std::string bareFileNameMark("choice-mark-cattle");
        fs::path pathOriginalParam(fs::path(pathToInputFolder + bareFileNameParam));
        fs::path pathOriginalEnv(fs::path(pathToInputFolder + bareFileNameEnv));
        fs::path pathOriginalMark(fs::path(pathToInputFolder + bareFileNameMark));
        INFO("Original param:\t" + infoPerms(pathOriginalParam));
        INFO("Original env:\t\t" + infoPerms(pathOriginalEnv));
        INFO("Original mark:\t" + infoPerms(pathOriginalMark));

        // Defining filenames and path to copied data
        std::string fileNameParam(pathToOutputFolder + bareFileNameParam);
        std::string fileNameEnv(pathToOutputFolder + bareFileNameEnv);
        std::string fileNameMark(pathToOutputFolder + bareFileNameMark);
        std::string fileNameMarkWithoutLeadingDot(pathToOutputFolderWithoutLeadingDot + bareFileNameMark);
        fs::path pathParam(fileNameParam.c_str());
        fs::path pathEnv(fileNameEnv.c_str());
        fs::path pathMark(fileNameMark.c_str());
        fs::path pathMarkWithoutLeadingDot(fileNameMarkWithoutLeadingDot.c_str());

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameLogs, pathParam, pathEnv, pathMark});

        CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

        // Copying data
        std::experimental::filesystem::copy(pathOriginalParam, pathParam, fs::copy_options::overwrite_existing);
        std::experimental::filesystem::copy(pathOriginalEnv, pathEnv, fs::copy_options::overwrite_existing);
        std::experimental::filesystem::copy(pathOriginalMark, pathMark, fs::copy_options::overwrite_existing);
        INFO("Copied param:\t\t" + infoPerms(pathParam));
        INFO("Copied env:\t\t" + infoPerms(pathEnv));
        INFO("Copied mark:\t\t" + infoPerms(pathMark));

        // Changing data permissions
        fs::permissions(pathParam, fs::perms::owner_all | fs::perms::group_all | fs::perms::others_read);
        fs::permissions(fs::path(fileNameEnv.c_str()),
                        fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read |
                        fs::perms::group_write | fs::perms::others_read);
        fs::permissions(fs::path(fileNameMark.c_str()),
                        fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read |
                        fs::perms::group_write | fs::perms::others_read);
        INFO("Chmod'ed param:\t" + infoPerms(pathParam));
        INFO("Chmod'ed env:\t\t" + infoPerms(pathEnv));
        INFO("Chmod'ed mark:\t" + infoPerms(pathMark));

        // Reading expected results
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

        WHEN("the program is run  using the path with a leading dot")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParam + " " + fileNameEnv + " " + fileNameMark);
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

        WHEN("the program is run  using the path without a leading dot")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParam + " " + fileNameEnv + " " + fileNameMarkWithoutLeadingDot);
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

        std::remove(pathToOutputFolder.c_str());
    }
}

