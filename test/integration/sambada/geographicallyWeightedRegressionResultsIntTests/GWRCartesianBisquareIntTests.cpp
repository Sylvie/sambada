#include "catch.hpp"
#include "SambadaRegressionResults.hpp"
#include "SambadaIntegrationTestUtils.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

SCENARIO("Test that geographically weighted regression is computed correctly for cartesian coordinates and bisquare weighting ",
         "[gwr-cartesian-bisquare-int][gwr-int]")
{

	INFO("Working folder: " + SambadaIntegrationTestUtils::runCommand("pwd"));

	GIVEN("Some input files with info on population structure")
	{
		std::string program(SambadaIntegrationTestUtils::computePlatformSpecificProgramName("./binaries/sambada"));

		std::string pathToTestFolder("test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/");

		std::string pathToOutputFolder("./" + pathToTestFolder);

		std::string fileNameOut0(pathToOutputFolder + "cattle-pop-mark-Out-0.txt");
		std::string fileNameOut1(pathToOutputFolder + "cattle-pop-mark-Out-1.txt");
		std::string fileNameOut2(pathToOutputFolder + "cattle-pop-mark-Out-2.txt");
		std::string fileNameLogs(pathToOutputFolder + "cattle-pop-mark-log.txt");

		std::vector<std::string> outputFileNames({fileNameOut0, fileNameOut1, fileNameOut2, fileNameLogs});

		std::string pathToInputFolder(
				SambadaIntegrationTestUtils::getTopSourceDirectory() + pathToTestFolder);

		std::string fileNameParams(pathToInputFolder + "param-cartesian-bisquare.txt");
		std::string fileNameEnv(pathToInputFolder + "cattle-pop-env.csv");
		std::string fileNameMark(pathToInputFolder + "cattle-pop-mark.txt");

		std::string fileNameExpectedResultsDim0(pathToInputFolder + "expected-results-dim-0.txt");
		std::string fileNameExpectedResultsDim1(pathToInputFolder + "expected-results-cartesian-bisquare-dim-1.txt");
		std::string fileNameExpectedResultsDim2(pathToInputFolder + "expected-results-cartesian-bisquare-dim-2.txt");

		std::ifstream lecteurCorrige(fileNameExpectedResultsDim0.c_str());
		INFO("Reading expected results dim 0");
		REQUIRE(lecteurCorrige.good());
		REQUIRE(lecteurCorrige.is_open());
		SambadaRegressionResults expectedNullResults(
				SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 0));
		lecteurCorrige.close();
		expectedNullResults.verifieTailles(true, 0, 3, false, true);

		lecteurCorrige.open(fileNameExpectedResultsDim1.c_str());
		INFO("Reading expected results dim 1");
		REQUIRE(lecteurCorrige.good());
		REQUIRE(lecteurCorrige.is_open());
		SambadaRegressionResults expectedResultsDim1(
				SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 1));
		lecteurCorrige.close();
		expectedResultsDim1.verifieTailles(true, 1, 9, false, true);

		lecteurCorrige.open(fileNameExpectedResultsDim2.c_str());
		INFO("Reading expected results dim 2");
		REQUIRE(lecteurCorrige.good());
		REQUIRE(lecteurCorrige.is_open());
		SambadaRegressionResults expectedResultsDim2(
				SambadaIntegrationTestUtils::readRegressionResults(lecteurCorrige, true, 2));
		lecteurCorrige.close();
		expectedResultsDim2.verifieTailles(true, 2, 9, false, true);

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


				AND_WHEN("the output files are read")
				{
					SambadaRegressionResults resultsDim0(
							SambadaIntegrationTestUtils::readRegressionResults(lecteurOut0, true, 0));
					SambadaRegressionResults resultsDim1(
							SambadaIntegrationTestUtils::readRegressionResults(lecteurOut1, true, 1));
					SambadaRegressionResults resultsDim2(
							SambadaIntegrationTestUtils::readRegressionResults(lecteurOut2, true, 2));

					THEN("the results match the expectations")
					{
						INFO("Verifying results dim 0");
						resultsDim0.verifieTailles(true, 0, 3, false, true);
						resultsDim0.compare(expectedNullResults);

						INFO("Verifying results dim 1");
						resultsDim1.verifieTailles(true, 1, 9, false, true);
						resultsDim1.compare(expectedResultsDim1);

						INFO("Verifying results dim 2");
						resultsDim2.verifieTailles(true, 2, 9, false, true);
						resultsDim2.compare(expectedResultsDim2, 0.001);
					}
				}

				lecteurOut0.close();
				lecteurOut1.close();
				lecteurOut2.close();
			}
		}

		SambadaIntegrationTestUtils::removeFiles(outputFileNames);
	}
}

