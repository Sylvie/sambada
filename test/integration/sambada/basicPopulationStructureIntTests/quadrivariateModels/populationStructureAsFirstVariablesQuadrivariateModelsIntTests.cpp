#include "catch.hpp"
#include "SambadaRegressionResults.hpp"
#include "SambadaIntegrationTestUtils.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that regression results are correct when the population structure is placed before the environmental variables for quadrivariate models",
         "[pop-struct-as-first-var-quadri-int][pop-struct-int]") {

    INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

    GIVEN("Some input files with info on population structure")
    {
        std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

        std::string pathToOutputFolder("./test/integration/sambada/basicPopulationStructureIntTests/quadrivariateModels/");

        std::string fileNameOut0(pathToOutputFolder + "cattle-pop-mark-first-Out-0.txt");
        std::string fileNameOut1(pathToOutputFolder + "cattle-pop-mark-first-Out-1.txt");
        std::string fileNameOut2(pathToOutputFolder + "cattle-pop-mark-first-Out-2.txt");
        std::string fileNameOut3(pathToOutputFolder + "cattle-pop-mark-first-Out-3.txt");
	    std::string fileNameOut4(pathToOutputFolder + "cattle-pop-mark-first-Out-4.txt");
        std::string fileNameLogs(pathToOutputFolder + "cattle-pop-mark-first-log.txt");

        std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameOut2, fileNameOut3, fileNameOut4, fileNameLogs});

        std::string pathToInputFolder(
                SambadaIntegrationTestUtils::getTopSourceDirectory() +
                "test/integration/sambada/basicPopulationStructureIntTests/quadrivariateModels/");

        std::string fileNameParams(pathToInputFolder + "param-quadri-pop-first.txt");
        std::string fileNameEnv(pathToInputFolder + "../cattle-pop-env-first.csv");
        std::string fileNameMark(pathToInputFolder + "../cattle-pop-mark.txt");

        std::string fileNameExpectedResultsDim0(pathToInputFolder + "expected-results-quadri-dim-0.txt");
        std::string fileNameExpectedResultsDim1(pathToInputFolder + "expected-results-quadri-dim-1.txt");
	    std::string fileNameExpectedResultsDim2(pathToInputFolder + "expected-results-quadri-pop-first-dim-2.txt");
        std::string fileNameExpectedResultsDim3(pathToInputFolder + "expected-results-quadri-pop-first-dim-3.txt");
        std::string fileNameExpectedResultsDim4(pathToInputFolder + "expected-results-quadri-pop-first-dim-4.txt");

        std::ifstream lecteurCorrige(fileNameExpectedResultsDim0.c_str());
        INFO("Reading expected results dim 0");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedNullResults(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
        lecteurCorrige.close();
        expectedNullResults.verifieTailles(true, 0, 30);

        lecteurCorrige.open(fileNameExpectedResultsDim1.c_str());
        INFO("Reading expected results dim 1");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsDim1(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
        lecteurCorrige.close();
        expectedResultsDim1.verifieTailles(true, 1, 240);

        lecteurCorrige.open(fileNameExpectedResultsDim2.c_str());
        INFO("Reading expected results dim 2");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsDim2(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 2));
        lecteurCorrige.close();
        expectedResultsDim2.verifieTailles(true, 2, 840);

        lecteurCorrige.open(fileNameExpectedResultsDim3.c_str());
        INFO("Reading expected results dim 3");
        REQUIRE(lecteurCorrige.good());
        REQUIRE(lecteurCorrige.is_open());
        SambadaRegressionResults expectedResultsDim3(
                SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 3));
        lecteurCorrige.close();
        expectedResultsDim3.verifieTailles(true, 3, 30);

	    lecteurCorrige.open(fileNameExpectedResultsDim4.c_str());
	    INFO("Reading expected results dim 4");
	    REQUIRE(lecteurCorrige.good());
	    REQUIRE(lecteurCorrige.is_open());
	    SambadaRegressionResults expectedResultsDim4(
			    SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 4));
	    lecteurCorrige.close();
	    expectedResultsDim4.verifieTailles(true, 4, 150, true);

        WHEN("Sambada is run using the first variables as population structure")
        {
            CHECK_FALSE(SambadaIntegrationTestUtils::doesAnyFileExist(outputFileNames));

            std::string output = SambadaIntegrationTestUtils::runCommand(
                    program + " " + fileNameParams + " " + fileNameEnv + " " + fileNameMark);
            //INFO(output);

            THEN("the output files are found")
            {
                INFO("Reading results dim 0");
                std::ifstream lecteurOut0(fileNameOut0.c_str());
                REQUIRE(lecteurOut0.good());
                REQUIRE(lecteurOut0.is_open());

                INFO("Reading results dim 1");
                std::ifstream lecteurOut1(fileNameOut1.c_str());
                REQUIRE(lecteurOut1.good());
                REQUIRE(lecteurOut1.is_open());

                INFO("Reading results dim 2");
                std::ifstream lecteurOut2(fileNameOut2.c_str());
                REQUIRE(lecteurOut2.good());
                REQUIRE(lecteurOut2.is_open());

                INFO("Reading results dim 3");
                std::ifstream lecteurOut3(fileNameOut3.c_str());
                REQUIRE(lecteurOut3.good());
                REQUIRE(lecteurOut3.is_open());

	            INFO("Reading results dim 4");
	            std::ifstream lecteurOut4(fileNameOut4.c_str());
	            REQUIRE(lecteurOut4.good());
	            REQUIRE(lecteurOut4.is_open());

	            AND_WHEN("the output files are read")
                {
                    SambadaRegressionResults resultsDim0(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut0, true, 0));
                    SambadaRegressionResults resultsDim1(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut1, true, 1));
                    SambadaRegressionResults resultsDim2(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut2, true, 2));
                    SambadaRegressionResults resultsDim3(
                            SambadaIntegrationTestUtils::readRegressionResults(lecteurOut3, true, 3));
	                SambadaRegressionResults resultsDim4(
			                SambadaIntegrationTestUtils::readRegressionResults(lecteurOut4, true, 4));

	                THEN("the results match the expectations")
                    {
                        INFO("Verifying results dim 0");
                        resultsDim0.verifieTailles(true, 0, 30);
                        resultsDim0.compare(expectedNullResults);

                        INFO("Verifying results dim 1");
                        resultsDim1.verifieTailles(true, 1, 240);
                        resultsDim1.compare(expectedResultsDim1);

                        INFO("Verifying results dim 2");
                        resultsDim2.verifieTailles(true, 2, 840);
                        resultsDim2.compare(expectedResultsDim2, 0.001);

                        INFO("Verifying results dim 3");
                        resultsDim3.verifieTailles(true, 3, 30);
                        resultsDim3.compare(expectedResultsDim3);

                        INFO("Verifying results dim 4");
	                    resultsDim4.verifieTailles(true, 4, 150, true);
	                    resultsDim4.compare(expectedResultsDim4, 0.001);
                    }
                }

                lecteurOut0.close();
                lecteurOut1.close();
                lecteurOut2.close();
                lecteurOut3.close();
	            lecteurOut4.close();
            }
        }

        SambadaIntegrationTestUtils::removeFiles(outputFileNames);
    }
}

