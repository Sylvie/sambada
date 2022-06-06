#include "SambadaIntegrationTestUtils.hpp"

#if defined(__unix__) || defined(__APPLE__)

std::string SambadaIntegrationTestUtils::runCommand(const std::string& command) {

    std::string data;
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    std::string commandLine(command + " 2>&1");

    stream = popen(commandLine.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

#endif
