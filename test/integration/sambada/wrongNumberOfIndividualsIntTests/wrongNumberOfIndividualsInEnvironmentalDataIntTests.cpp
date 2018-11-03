#include "catch.hpp"
#include "SambadaRegressionResults.h"
#include "SambadaIntegrationTestUtils.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

SCENARIO("Test that wrong number of individuals in environmental data is detected during initialisation",
         "[wrong-number-individuals-env-data-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Some environmental files with too many or too few individuals")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/wrongNumberOfIndividualsIntTests/");
        std::string fileNameLogs(pathToOutputFolder + "choice-mark-cattle-log.txt");

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/wrongNumberOfIndividualsIntTests/");

        std::string fileNameParams(pathToInputFolder + "param-wrong-number-individuals.txt");

        std::string fileNameEnv(pathToInputFolder + "choice-env-cattle.csv");
        std::string fileNameEnvTooManyIndividuals(pathToInputFolder + "choice-env-cattle-too-many-individuals.csv");
        std::string fileNameEnvTooFewIndividuals(pathToInputFolder + "choice-env-cattle-too-few-individuals.csv");
        std::string fileNameEnvTooManyIndividualsNoNewLine(
                pathToInputFolder + "choice-env-cattle-too-many-individuals-no-newline-eof.csv");
        std::string fileNameEnvTooFewIndividualsNoNewLine(
                pathToInputFolder + "choice-env-cattle-too-few-individuals-no-newline-eof.csv");

        std::string fileNameMark(pathToInputFolder + "choice-mark-cattle.txt");

        WHEN("Sambada is run using the environmental file with too many individuals")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnvTooManyIndividuals + " " + fileNameMark);
            INFO(output);

            THEN("the log file contains the error message")
            {
                CHECK(output.find("Incorrect number of points, too many lines in environmental data file.") !=
                      std::string::npos);
            }

            std::remove(fileNameLogs.c_str());
        }

        WHEN("Sambada is run using the environmental file with too few individuals")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnvTooFewIndividuals + " " + fileNameMark);
            INFO(output);

            THEN("the log file contains the error message")
            {
                CHECK(output.find(
                        "Incorrect number of points. Environmental file contains 776 samples, while there should be 777.") !=
                      std::string::npos);
            }

            std::remove(fileNameLogs.c_str());
        }

        WHEN("Sambada is run using the environmental file with too many individuals and no new line at end of file")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnvTooManyIndividualsNoNewLine + " " + fileNameMark);
            INFO(output);

            THEN("the log file contains the error message")
            {
                CHECK(output.find("Incorrect number of points, too many lines in environmental data file.") !=
                      std::string::npos);
            }

            std::remove(fileNameLogs.c_str());
        }

        WHEN("Sambada is run using the environmental file with too few individuals and no new line at end of file")
        {
            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnvTooFewIndividualsNoNewLine + " " + fileNameMark);
            INFO(output);

            THEN("the log file contains the error message")
            {
                CHECK(output.find(
                        "Incorrect number of points. Environmental file contains 776 samples, while there should be 777.") !=
                      std::string::npos);
            }

            std::remove(fileNameLogs.c_str());
        }
    }
}

