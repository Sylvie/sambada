#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that wrong number of individuals in molecular data is detected during initialisation", "[wrong-number-individuals-molecular-data-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Some molecular files with too many or too few individuals")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/wrongNumberOfIndividualsIntTests/");
        std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-log.txt");

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() + "test/integration/sambada/wrongNumberOfIndividualsIntTests/");

        std::string fileNameParams(pathToInputFolder + "param-wrong-number-individuals.txt");

        std::string fileNameEnv(pathToInputFolder + "choice-env-cattle.csv");

        std::string fileNameMark(pathToInputFolder + "choice-mark-cattle.txt");
        std::string fileNameMarkTooManyIndividuals(pathToInputFolder + "choice-mark-cattle-too-many-individuals.txt");
        std::string fileNameMarkTooFewIndividuals(pathToInputFolder + "choice-mark-cattle-too-few-individuals.txt");
        std::string fileNameMarkTooManyIndividualsNoNewLine(pathToInputFolder + "choice-mark-cattle-too-many-individuals-no-newline-eof.txt");
        std::string fileNameMarkTooFewIndividualsNoNewLine(pathToInputFolder + "choice-mark-cattle-too-few-individuals-no-newline-eof.txt");

        WHEN("Sambada is run using the molecular file with too many individuals")
        {
            std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-too-many-individuals-log.txt");

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnv + " " + fileNameMarkTooManyIndividuals);
            INFO(output);

            THEN("the log file contains the error message")
            {
                CHECK(output.find("Incorrect points number, too many lines in data file.") !=
                      std::string::npos);
            }

            std::remove(fileNameLogs.c_str());
        }

        WHEN("Sambada is run using the molecular file with too few individuals")
        {
            std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-too-few-individuals-log.txt");

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnv + " " + fileNameMarkTooFewIndividuals);
            INFO(output);

            THEN("the log file contains the error message")
            {
                CHECK(output.find(
                        "Nombre de points incorrect. Molecular file contains 776 samples, while there should be 777.") !=
                      std::string::npos);
            }

            std::remove(fileNameLogs.c_str());
        }

        WHEN("Sambada is run using the molecular file with too many individuals and no new line at end of file")
        {
            std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-too-many-individuals-log-no-newline-eof.txt");

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnv + " " + fileNameMarkTooManyIndividualsNoNewLine);
            INFO(output);

            THEN("the log file contains the error message")
            {
                CHECK(output.find("Incorrect points number, too many lines in data file.") !=
                      std::string::npos);
            }

            std::remove(fileNameLogs.c_str());
        }

        WHEN("Sambada is run using the molecular file with too few individuals and no new line at end of file")
        {
            std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-too-few-individuals-log-no-newline-eof.txt");

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnv + " " + fileNameMarkTooFewIndividualsNoNewLine);
            INFO(output);

            THEN("the log file contains the error message")
            {
                CHECK(output.find(
                        "Nombre de points incorrect. Molecular file contains 776 samples, while there should be 777.") !=
                      std::string::npos);
            }

            std::remove(fileNameLogs.c_str());
        }
    }
}

