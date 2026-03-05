#define _CRT_SECURE_NO_WARNINGS
#include "../stacktrace-windows/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <exception>

extern int exampleMain(int argc, char** argv);
extern const char* const g_logFileName;

int WinMain(
    HINSTANCE /* instance */,
    HINSTANCE /* prevInstance */,
    LPSTR /* commandLine */,
    int /*showCommand*/)

{
    FILE* logFile = fopen(g_logFileName, "w");
#ifdef _MSC_VER
    __try
    {
        int argc = 0;
        char** argv = nullptr;
        return exampleMain(argc, argv);
    }
    __except (exceptionFilter(logFile, GetExceptionInformation()))
    {
        ::exit(1);
    }
#else // _MSC_VER
    // TODO: Standard C++ try/catch only catches C++ exceptions, not hardware exceptions
    // such as access violations or stack overflows.
    try
    {
        int argc = 0;
        char** argv = nullptr;
        return exampleMain(argc, argv);
    }
    catch (const std::exception& e)
    {
        if (logFile)
        {
            fprintf(logFile, e.what());
            fflush(logFile);
        }
        ::exit(1);
    }
#endif
}