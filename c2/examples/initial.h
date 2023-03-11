#pragma once
#include <iostream>
#ifndef INIIAL_H
#define INITIAL_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <sstream>
#include <getopt.h>
#include "Definitions.h"
#ifndef MMC_SUCCESS
#define MMC_SUCCESS 0
#endif
#ifndef MMC_FAILED
#define MMC_FAILED 1
#endif
#ifndef MMC_MAX_LOG_MSG_SIZE
#define MMC_MAX_LOG_MSG_SIZE 512
#endif
using namespace std;
class initialdefination {
public:
    const string g_programName = "HelloEposCmd";
    void *g_pKeyHandle = 0;
  
    unsigned short g_usNodeId = 1;
    unsigned short g_usNodeId2= 2;
    string g_deviceName;
    string g_protocolStackName;
    string g_interfaceName;
    string g_portName;
    int g_baudrate = 0;
public:
    void LogInfo(string message);
    void LogError(string functionName, int p_lResult, unsigned int p_ulErrorCode);//95
    void PrintUsage();//80
    void SeparatorLine();
    void PrintSettings();
    void SetDefaultParameters();
    int CloseDevice(unsigned int *p_pErrorCode);
    int ParseArguments(int argc, char **argv);
    int MaxFollowingErrorDemo(unsigned int &p_rlErrorCode);
    void PrintHeader();
    int PrintAvailablePorts(char *p_pInterfaceNameSel);
    int PrintAvailableInterfaces();
    int PrintDeviceVersion();
    int PrintAvailableProtocols();
};
#endif