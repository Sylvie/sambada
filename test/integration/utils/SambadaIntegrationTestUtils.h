#ifndef SAMBADA_SAMBADAINTEGRATIONTESTUTILS_H
#define SAMBADA_SAMBADAINTEGRATIONTESTUTILS_H

#include "SambadaRegressionResults.h"
#include <string>

class SambadaIntegrationTestUtils {
public:
    static std::string computePlatformSpecificProgramName(const std::string &baseProgramName);
    static std::string runCommand(const std::string &commandLine);
    static std::string getTopSourceDirectory();
    static SambadaRegressionResults readRegressionResults(std::ifstream& lecteur, bool hasHeader, int dimension);
    static void copyFileAndUpdatePermissions(const std::string& inputFile, const std::string& outputFile);
    static bool doesFileExist(const std::string& filename);
    static bool doesAnyFileExist(const std::vector<std::string>& filenames);
    static void removeFiles(const std::vector<std::string>& filenames);
};

#endif //SAMBADA_SAMBADAINTEGRATIONTESTUTILS_H
