#include "SambadaIntegrationTestUtils.hpp"

#if defined(_WIN32) || defined(_WIN64)

// Source:
// https://stackoverflow.com/questions/10866944/how-can-i-read-a-child-processs-output

#include <Windows.h>
#include <stdio.h>
#include <algorithm>

std::string SambadaIntegrationTestUtils::runCommand(const std::string& commandLine) {
    PROCESS_INFORMATION processInfo;
    STARTUPINFOA startupInfo;
    SECURITY_ATTRIBUTES saAttr;

    HANDLE stdoutReadHandle = NULL;
    HANDLE stdoutWriteHandle = NULL;

    char cmdline[1024];
    std::string data;
    DWORD bytes_read;
    char tBuf[257];

    DWORD exitcode;

    std::string windowsCommandLine(commandLine);
    std::replace( windowsCommandLine.begin(), windowsCommandLine.end(), '/', '\\'); // replace all 'x' to 'y'
    windowsCommandLine.copy(cmdline, windowsCommandLine.size());
    cmdline[windowsCommandLine.size()] = '\0';

    memset(&saAttr, 0, sizeof(saAttr));
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT.
    if (!CreatePipe(&stdoutReadHandle, &stdoutWriteHandle, &saAttr, 5000))
    {
        std::string error("CreatePipe: " + std::to_string(GetLastError()) + "\n");
        // printf("CreatePipe: %u\n", GetLastError());
        return error;
    }

    // Ensure the read handle to the pipe for STDOUT is not inherited.
    if (!SetHandleInformation(stdoutReadHandle, HANDLE_FLAG_INHERIT, 0))
    {
        std::string error("SetHandleInformation: " + std::to_string(GetLastError()) + "\n");
        // printf("SetHandleInformation: %u\n", GetLastError());
        return error;
    }

    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.hStdError = stdoutWriteHandle;
    startupInfo.hStdOutput = stdoutWriteHandle;
    startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;

    // memset(&processInfo, 0, sizeof(processInfo));  // Not actually necessary

    // printf("Starting.\n");

    if (!CreateProcess(NULL, cmdline, NULL, NULL, TRUE,
                       CREATE_NO_WINDOW, NULL, 0, &startupInfo, &processInfo))
    {
        std::string error("CreateProcess: " + std::to_string(GetLastError()) + "\n");
        // printf("CreateProcess: %u\n", GetLastError());
        return error;
    }

    CloseHandle(stdoutWriteHandle);

    for (;;) {
        // printf("Just before ReadFile(...)\n");
        if (!ReadFile(stdoutReadHandle, tBuf, 256, &bytes_read, NULL))
        {
            std::string error("ReadFile: " + std::to_string(GetLastError()) + "\n");
            // printf("ReadFile: %u\n", GetLastError());
            break;
        }
        // printf("Just after ReadFile, read %u byte(s)\n", bytes_read);
        if (bytes_read > 0)
        {
            tBuf[bytes_read] = '\0';
            data.append(tBuf);
        }
    }

    // printf("Output: %s\n", data.c_str());

    if (WaitForSingleObject(processInfo.hProcess, INFINITE) != WAIT_OBJECT_0)
    {
        std::string error("WaitForSingleObject: " + std::to_string(GetLastError()) + "\n");
        // printf("WaitForSingleObject: %u\n", GetLastError());
        return error;
    }

    if (!GetExitCodeProcess(processInfo.hProcess, &exitcode))
    {
        std::string error("GetExitCodeProcess: " + std::to_string(GetLastError()) + "\n");
        // printf("GetExitCodeProcess: %u\n", GetLastError());
        return error;
    }

    // printf("Exit code: %u\n", exitcode);

    CloseHandle( processInfo.hProcess );
    CloseHandle( processInfo.hThread );

    return data;
}

#endif
