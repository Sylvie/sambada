#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void demo_perms(fs::perms p)
{
    std::cout << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
              << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
              << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
              << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
              << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
              << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
              << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
              << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
              << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-")
              << '\n';
}

SCENARIO("Test that result files are writen in the same folder as the molecular data file", "[results-same-folder-molecular-data-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("A program to run, its expected output file, and the file of expected results")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsMolecularData/");
        fs::create_directory(pathToOutputFolder.c_str());

        std::string fileNameOut0(pathToOutputFolder + "choice-mark-cattle-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "choice-mark-cattle-Out-1.txt");
        std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-log.txt");

        std::string pathToInputFolder(SambadaIntegrationTestUtils::getTopSourceDirectory() + "test/integration/sambada/resultFilesLocationIntTests/");
        std::string bareFileNameParam("param-without-outputfile-setting.txt");
        std::string bareFileNameEnv("choice-env-cattle.csv");
        std::string bareFileNameMark("choice-mark-cattle.txt");

        demo_perms(fs::status(fs::path((pathToInputFolder + bareFileNameParam).c_str())).permissions());
        demo_perms(fs::status(fs::path((pathToInputFolder + bareFileNameEnv).c_str())).permissions());
        demo_perms(fs::status(fs::path((pathToInputFolder + bareFileNameMark).c_str())).permissions());
        std::string fileNameParam(pathToOutputFolder + bareFileNameParam);
        std::string fileNameEnv(pathToOutputFolder + bareFileNameEnv);
        std::string fileNameMark(pathToOutputFolder + bareFileNameMark);

        std::experimental::filesystem::copy(fs::path((pathToInputFolder + bareFileNameParam).c_str()), fs::path(fileNameParam.c_str()));
        std::experimental::filesystem::copy(fs::path((pathToInputFolder + bareFileNameEnv).c_str()), fs::path(fileNameEnv.c_str()));
        std::experimental::filesystem::copy(fs::path((pathToInputFolder + bareFileNameMark).c_str()), fs::path(fileNameMark.c_str()));
        demo_perms(fs::status(fs::path(fileNameParam.c_str())).permissions());
        demo_perms(fs::status(fs::path(fileNameEnv.c_str())).permissions());
        demo_perms(fs::status(fs::path(fileNameMark.c_str())).permissions());

        fs::permissions(fs::path(fileNameParam.c_str()), fs::perms::owner_all | fs::perms::group_all | fs::perms::others_read);
        fs::permissions(fs::path(fileNameEnv.c_str()), fs::perms::owner_all | fs::perms::group_all | fs::perms::others_read);
        fs::permissions(fs::path(fileNameMark.c_str()), fs::perms::owner_all | fs::perms::group_all | fs::perms::others_read);
        demo_perms(fs::status(fs::path(fileNameParam.c_str())).permissions());
        demo_perms(fs::status(fs::path(fileNameEnv.c_str())).permissions());
        demo_perms(fs::status(fs::path(fileNameMark.c_str())).permissions());

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

        WHEN("the program is run")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(program + " " + fileNameParam + " " + fileNameEnv + " " + fileNameMark);
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

            std::remove(fileNameParam.c_str());
            std::remove(fileNameEnv.c_str());
            std::remove(fileNameMark.c_str());

            std::remove(fileNameOut0.c_str());
            std::remove(fileNameOut1.c_str());
            std::remove(fileNameLogs.c_str());

            std::remove(pathToOutputFolder.c_str());
        }
    }
}

