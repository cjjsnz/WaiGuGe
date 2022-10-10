//============================================================================
// Name        : HelloEposCmd.cpp
// Author      : Dawid Sienkiewicz
// Version     :
// Copyright   : maxon motor ag 2014-2021
// Description : Hello Epos in C++
//============================================================================
/*include**************************************************************************/
#include <iostream>//
#include "Definitions.h"
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/time.h>
#include <fstream>
#include <vector>
#include "tes.h"
#include "initial.h"
#include "serial.h"
#include <stdbool.h>
#include "server.h"
#include "emg_adc.h"
/**vector define*************************************************************************/
typedef void* HANDLE;
typedef int BOOL;
using namespace std;
initialdefination I1;
emgdata E4Chn;
enum EAppMode
{
    AM_UNKNOWN,
    AM_DEMO,
    AM_INTERFACE_LIST,
    AM_PROTOCOL_LIST,
    AM_VERSION_INFO
};
    EAppMode g_eAppMode =AM_DEMO;
    const int N=5;
    int current[N],velocity[N],position[N],pCurrentM[N];
    unsigned int homeacc=1000;
    unsigned int speedSwitch=500;
    unsigned int speedIndex=100;
    int homeOffset=0;
    unsigned short currentThreshold=500;
    int homePosition=0;
    const string g_programName = "HelloEposCmd";
/*if define**************************************************************************/
#ifndef MMC_SUCCESS
	#define MMC_SUCCESS 0
#endif
#ifndef MMC_FAILED
	#define MMC_FAILED 1
#endif
#ifndef MMC_MAX_LOG_MSG_SIZE
	#define MMC_MAX_LOG_MSG_SIZE 512
#endif
/***************************************************************************/
int   OpenDevice(unsigned int* p_pErrorCode);
int   ParseArguments(int argc, char** argv);
int   PrepareDemo(unsigned int* p_pErrorCode);
void  getMotorState(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode,  int* pCurrent,int* pVelocity ,int* pPosition ,int* pCurrentMust);
/***************************************************************************/
int   DemoProfilePositionMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode);
int   DemoProfileCurrentMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode);
int   Demo(unsigned int* p_pErrorCode);
bool  DoHoming(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int homeacc, unsigned int speedSwitch, unsigned int speedIndex, int homeOffset, unsigned short currentThreshold, int homePosition, unsigned int & p_rlErrorCode);
int   DemoGobackZero(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode);
int   positionlimit(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode);
int  GobackHome(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode);
int  emg_adc_convert(void);
/***************************************************************************/
using namespace std;
// 功能：将filename 中的数据（共cols列）读取到_vector中，_vector可视为二维数组
int OpenDevice(unsigned int* p_pErrorCode)
{
	int lResult = MMC_FAILED;

	char* pDeviceName = new char[255];
	char* pProtocolStackName = new char[255];
	char* pInterfaceName = new char[255];
	char* pPortName = new char[255];

	strcpy(pDeviceName, I1.g_deviceName.c_str());
	strcpy(pProtocolStackName, I1.g_protocolStackName.c_str());
	strcpy(pInterfaceName, I1.g_interfaceName.c_str());
	strcpy(pPortName, I1.g_portName.c_str());

	I1.LogInfo("Open device...");

	I1.g_pKeyHandle = VCS_OpenDevice(pDeviceName, pProtocolStackName, pInterfaceName, pPortName, p_pErrorCode);

	if(I1.g_pKeyHandle!=0 && *p_pErrorCode == 0)
	{
		unsigned int lBaudrate = 0;
		unsigned int lTimeout = 0;

		if(VCS_GetProtocolStackSettings(I1.g_pKeyHandle, &lBaudrate, &lTimeout, p_pErrorCode)!=0)
		{
			if(VCS_SetProtocolStackSettings(I1.g_pKeyHandle, I1.g_baudrate, lTimeout, p_pErrorCode)!=0)
			{
				if(VCS_GetProtocolStackSettings(I1.g_pKeyHandle, &lBaudrate, &lTimeout, p_pErrorCode)!=0)
				{
					if(I1.g_baudrate==(int)lBaudrate)
					{
						lResult = MMC_SUCCESS;
					}
				}
			}
		}
	}
	else
	{
        I1.g_pKeyHandle = 0;
	}

	delete []pDeviceName;
	delete []pProtocolStackName;
	delete []pInterfaceName;
	delete []pPortName;

	return lResult;
}
int DemoProfilePositionMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode)
{
	int lResult = MMC_SUCCESS;
	stringstream msg;
	msg << "set profile position mode, node = " << p_usNodeId;
	I1.LogInfo(msg.str());

	if(VCS_ActivateProfilePositionMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
	{
		I1.LogError("VCS_ActivateProfilePositionMode", lResult, p_rlErrorCode);
		lResult = MMC_FAILED;
	}
	else
	{
		list<long> positionList;

		positionList.push_back(0);
		positionList.push_back(-10000);
		positionList.push_back(5000);

		for(list<long>::iterator it = positionList.begin(); it !=positionList.end(); it++)
		{
			long targetPosition = (*it);
			stringstream msg;
			msg<<"move to position = " << targetPosition << ", node = " << p_usNodeId;
			I1.LogInfo(msg.str());

			if(VCS_MoveToPosition(p_DeviceHandle, p_usNodeId, targetPosition, 1, 0, &p_rlErrorCode) == 0)
			{
				I1.LogError("VCS_MoveToPosition", lResult, p_rlErrorCode);
				lResult = MMC_FAILED;
				break;
			}

            for(int i = 0;i<N;i++)
            {
                sleep(1);
                getMotorState(p_DeviceHandle, p_usNodeId, p_rlErrorCode,current+i,velocity+i,position+i,pCurrentM+i);
                stringstream msg;
                msg <<"Current Must Be "<<pCurrentM[i]<< " current = " << current[i]<<" velocity = " <<velocity[i]<<"rpm"<<" position = "<<position[i]<<"inc";
                I1.LogInfo(msg.str());
            }
		}

		if(lResult == MMC_SUCCESS)
		{
			I1.LogInfo("halt position movement");

			if(VCS_HaltPositionMovement(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
			{
                I1.LogError("VCS_HaltPositionMovement", lResult, p_rlErrorCode);
				lResult = MMC_FAILED;
			}
		}
	}

	return lResult;
}
int GobackHome(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode){
    int lResult = MMC_SUCCESS;
    stringstream msg;
    msg << "Go Back Home, node = " << p_usNodeId;
    I1.LogInfo(msg.str());

    if(VCS_ActivateProfilePositionMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
    {
        I1.LogError("VCS_ActivateProfilePositionMode", lResult, p_rlErrorCode);
        lResult = MMC_FAILED;
    }

    if(VCS_MoveToPosition(p_DeviceHandle, p_usNodeId, homePosition, 1, 0, &p_rlErrorCode) == 0)
    {
        I1.LogError("VCS_MoveToPosition", lResult, p_rlErrorCode);
        lResult = MMC_FAILED;

    }

    for(int i = 0;i<10;i++)
    {
        sleep(1);
        getMotorState(p_DeviceHandle, p_usNodeId, p_rlErrorCode,current+i,velocity+i,position+i,pCurrentM+i);
        stringstream msg;
        msg <<"Current Must Be "<<pCurrentM[i]<< " current = " << current[i]<<" velocity = " <<velocity[i]<<"rpm"<<" position = "<<position[i]<<"qc";
        I1.LogInfo(msg.str());
    }
   return lResult;
}
bool DemoProfileVelocityMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode)
{
	int lResult = MMC_SUCCESS;
	stringstream msg;

	msg << "set profile velocity mode, node = " << p_usNodeId;

    I1.LogInfo(msg.str());

	if(VCS_ActivateProfileVelocityMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
	{
        I1.LogError("VCS_ActivateProfileVelocityMode", lResult, p_rlErrorCode);
		lResult = MMC_FAILED;
	}
	else
	{
		list<long> velocityList;

		velocityList.push_back(100);
		velocityList.push_back(500);
		velocityList.push_back(1000);

		for(list<long>::iterator it = velocityList.begin(); it !=velocityList.end(); it++)
		{
			long targetvelocity = (*it);

			stringstream msg;
			msg << "move with target velocity = " << targetvelocity << " rpm, node = " << p_usNodeId;
            I1.LogInfo(msg.str());

			if(VCS_MoveWithVelocity(p_DeviceHandle, p_usNodeId, targetvelocity, &p_rlErrorCode) == 0)
			{
				lResult = MMC_FAILED;
                I1.LogError("VCS_MoveWithVelocity", lResult, p_rlErrorCode);
				break;
			}

            for(int i = 0;i<N;i++)
            {
                sleep(1);
                getMotorState(p_DeviceHandle, p_usNodeId, p_rlErrorCode,current+i,velocity+i,position+i,pCurrentM+i);
                stringstream msg;
                msg <<"Current Must Be "<<pCurrentM[i]<< " current = " << current[i]<<" velocity = " <<velocity[i]<<"rpm"<<" position = "<<position[i]<<"inc";
                I1.LogInfo(msg.str());
            }
		}

		if(lResult == MMC_SUCCESS)
		{
            I1.LogInfo("halt velocity movement");

			if(VCS_HaltVelocityMovement(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
			{
				lResult = MMC_FAILED;
                I1.LogError("VCS_HaltVelocityMovement", lResult, p_rlErrorCode);
			}
		}
	}

	return lResult;
}
int DemoProfileCurrentMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode)
{
    int lResult = MMC_SUCCESS;


    stringstream msg;

    msg << "set profile current mode, node = " << p_usNodeId;

    I1.LogInfo(msg.str());

    if(VCS_ActivateCurrentMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode)==0)
            //VCS_ActivateProfileVelocityMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
    {
        I1.LogError("VCS_ActivateCurrentVelocityMode", lResult, p_rlErrorCode);
        lResult = MMC_FAILED;
    }
    else
    {
        list<long> currentList;

        currentList.push_back(130);
        currentList.push_back(-300);
        currentList.push_back(300);
        currentList.push_front(-200);

        for(list<long>::iterator it = currentList.begin(); it !=currentList.end(); it++)
        {
            long targetcurrent = (*it);

            stringstream msg;
            msg << "move with target current = " << targetcurrent << " mA, node = " << p_usNodeId;
            I1.LogInfo(msg.str());

            if(VCS_SetCurrentMustEx(p_DeviceHandle, p_usNodeId, targetcurrent, &p_rlErrorCode)==0)
                    //VCS_MoveWithVelocity(p_DeviceHandle, p_usNodeId, targetvelocity, &p_rlErrorCode) == 0)
            {
                lResult = MMC_FAILED;
                I1.LogError("VCS_SetCurrentMustEx", lResult, p_rlErrorCode);
                break;
            }

            for(int i = 0;i<N;i++)
            {
                sleep(1);
                getMotorState(p_DeviceHandle, p_usNodeId, p_rlErrorCode,current+i,velocity+i,position+i,pCurrentM+i);
                stringstream msg;
                msg <<"Current Must Be "<<pCurrentM[i]<<"mA"<< " current = " << current[i]<<"mA  velocity = " <<velocity[i]<<"rpm"<<" position = "<<position[i]<<"qc";
                I1.LogInfo(msg.str());

            }
            /*
            string file ="tes.txt";
            //txt文件中有4列
            int columns = 1;
            vector<double *> output_vector;
            if (!read_scanf(file, columns, output_vector))
            {
                return 0;
            }

            int rows = output_vector.size();
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    cout << output_vector[i][j] << " ";
                    VCS_SetCurrentMustEx(p_DeviceHandle, p_usNodeId, output_vector[i][j], &p_rlErrorCode);
                    for(int i = 0;i<1;i++)
                    {
                        sleep(1);
                        getMotorState(p_DeviceHandle, p_usNodeId, p_rlErrorCode,current+i,velocity+i,position+i,pCurrentM+i);
                        stringstream msg;
                        msg <<"Test Current Must Be "<<pCurrentM[i]<<"mA"<< " current = " << current[i]<<"mA  velocity = " <<velocity[i]<<"rpm"<<" position = "<<position[i]<<"qc";
                        I1.LogInfo(msg.str());

                    }
                }
                cout << endl;
            }
            */
           while(1){
                  E4Chn.getemgdata();
                  VCS_SetCurrentMustEx(p_DeviceHandle, p_usNodeId, E4Chn.emg1Chn, &p_rlErrorCode);

           }
          


        }
      // DoHoming(p_DeviceHandle, p_usNodeId, homeacc,speedSwitch, speedIndex,  homeOffset, currentThreshold, homePosition, p_rlErrorCode);
        GobackHome(p_DeviceHandle, p_usNodeId, p_rlErrorCode);
        if(lResult == MMC_SUCCESS)
        {
            I1.LogInfo("halt current movement");

            if(VCS_HaltVelocityMovement(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
            {
                lResult = MMC_FAILED;
                I1.LogError("VCS_HaltVelocityMovement", lResult, p_rlErrorCode);
            }
        }
    }

    return lResult;
}
int DemoGobackZero(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode) {
    int lResult = MMC_SUCCESS;
    stringstream msg;
    msg << "set profile Homing position mode, node = " << p_usNodeId;

    I1.LogInfo(msg.str());
    int Initialcurrent=250;
    int ipCurrent;
    if(VCS_ActivateCurrentMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode))
        if(VCS_SetCurrentMustEx(p_DeviceHandle, p_usNodeId, Initialcurrent, &p_rlErrorCode))

            for(int i = 0;i<10;i++)
            {
                sleep(1);
                VCS_GetCurrentIsAveragedEx(p_DeviceHandle, p_usNodeId, &ipCurrent, &p_rlErrorCode);

                stringstream msg;
                msg <<"Initial Current Must Be "<< Initialcurrent<<"mA"<< " current = " << ipCurrent<<"mA";
                I1.LogInfo(msg.str());

                if(ipCurrent>200)
                {
                    DoHoming(p_DeviceHandle, p_usNodeId, homeacc,speedSwitch, speedIndex,  homeOffset, currentThreshold, homePosition, p_rlErrorCode);
                    sleep(1);
                    getMotorState(p_DeviceHandle, p_usNodeId, p_rlErrorCode,current+i,velocity+i,position+i,pCurrentM+i);

                    msg <<" Current Must Be "<<pCurrentM[i]<<"mA"<< " current = " << current[i]<<"mA  velocity = " <<velocity[i]<<"rpm"<<" position = "<<position[i]<<"qc";
                    I1.LogInfo(msg.str());
                }

            }


    return lResult;
}
bool DoHoming(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int homeacc, unsigned int speedSwitch, unsigned int speedIndex, int homeOffset, unsigned short currentThreshold, int homePosition, unsigned int & p_rlErrorCode)
{
    int lResult = MMC_SUCCESS;
    stringstream msg;

    msg << "set homing mode, node = " << p_usNodeId;
    I1.LogInfo(msg.str());

    if(VCS_ActivateHomingMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
    {
        I1.LogError("VCS_ActivateHomingMode", lResult, p_rlErrorCode);
        lResult = MMC_FAILED;
    }
    else
    {
        if (VCS_ClearFault(p_DeviceHandle, p_usNodeId, &p_rlErrorCode))
            if (VCS_SetEnableState(p_DeviceHandle, p_usNodeId, &p_rlErrorCode))
                if (VCS_SetHomingParameter(p_DeviceHandle, p_usNodeId, homeacc, speedSwitch, speedIndex, homeOffset,currentThreshold, homePosition, &p_rlErrorCode))
                    if (VCS_FindHome(p_DeviceHandle, p_usNodeId, 35, &p_rlErrorCode))
                        //if (VCS_WaitForHomingAttained(p_DeviceHandle, p_usNodeId, 20, &p_rlErrorCode))
                           // if (VCS_DefinePosition(p_DeviceHandle, p_usNodeId, 30000, &p_rlErrorCode))
                                // VCS_GetHomingParameter(p_DeviceHandle, p_usNodeId, &homeacc, &speedSwitch,&speedIndex, &homeOffset, &currentThreshold, &homePosition, &p_rlErrorCode);
                                for(int i = 0;i<3;i++)
                                {
                                    sleep(1);
                                    getMotorState(p_DeviceHandle, p_usNodeId, p_rlErrorCode,current+i,velocity+i,position+i,pCurrentM+i);
                                    stringstream msg;
                                    msg <<"Initial Current Must Be "<<pCurrentM[i]<<"mA"<< " current = " << current[i]<<"mA  velocity = " <<velocity[i]<<"rpm"<<" position = "<<position[i]<<"qc";
                                    I1.LogInfo(msg.str());
                                }

                                return true;
        return false;
    }
    return lResult;
}

void getMotorState(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode,int* pCurrent,int* pVelocity ,int* pPosition ,int* pCurrentMust)
{

    //VCS_GetCurrentIsEx(p_DeviceHandle, p_usNodeId, pCurrent, &p_rlErrorCode);
    VCS_GetCurrentIsAveragedEx(p_DeviceHandle, p_usNodeId, pCurrent, &p_rlErrorCode);
    //VCS_GetVelocityIs(p_DeviceHandle, p_usNodeId, pVelocity, &p_rlErrorCode);
    VCS_GetVelocityIsAveraged(p_DeviceHandle, p_usNodeId, pVelocity, &p_rlErrorCode);
    VCS_GetPositionIs(p_DeviceHandle, p_usNodeId, pPosition, &p_rlErrorCode);
    VCS_GetCurrentMustEx(p_DeviceHandle, p_usNodeId, pCurrentMust, &p_rlErrorCode);
}
int ParseArguments(int argc, char** argv)
{
    int lOption;
    int lResult = MMC_SUCCESS;

    opterr = 0;

    while((lOption = getopt(argc, argv, "hlrvd:s:i:p:b:n:")) != -1)
    {
        switch (lOption)
        {
            case 'h':
                I1.PrintUsage();
                lResult = 1;
                break;
            case 'd':
                I1.g_deviceName = optarg;
                break;
            case 's':
                I1.g_protocolStackName = optarg;
                break;
            case 'i':
                I1.g_interfaceName= optarg;
                break;
            case 'p':
                I1.g_portName = optarg;
                break;
            case 'b':
                I1.g_baudrate = atoi(optarg);
                break;
            case 'n':
                I1.g_usNodeId= (unsigned short)atoi(optarg);
                break;
            case 'l':
                g_eAppMode = AM_INTERFACE_LIST;
                break;
            case 'r':
                g_eAppMode = AM_PROTOCOL_LIST;
                break;
            case 'v':
                g_eAppMode = AM_VERSION_INFO;
                break;
            case '?':  // unknown option...
                stringstream msg;
                msg << "Unknown option: '" << char(optopt) << "'!";
                I1.LogInfo(msg.str());
                I1.PrintUsage();
                lResult = MMC_FAILED;
                break;
        }
    }

    return lResult;
}
int PrepareDemo(unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	BOOL oIsFault = 0;

	if(VCS_GetFaultState(I1.g_pKeyHandle, I1.g_usNodeId, &oIsFault, p_pErrorCode ) == 0)
	{
		I1.LogError("VCS_GetFaultState", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}

	if(lResult==0)
	{
		if(oIsFault)
		{
			stringstream msg;
			msg << "clear fault, node = '" << I1.g_usNodeId<< "'";
			I1.LogInfo(msg.str());

			if(VCS_ClearFault(I1.g_pKeyHandle, I1.g_usNodeId, p_pErrorCode) == 0)
			{
				I1.LogError("VCS_ClearFault", lResult, *p_pErrorCode);
				lResult = MMC_FAILED;
			}
		}

		if(lResult==0)
		{
			BOOL oIsEnabled = 0;

			if(VCS_GetEnableState(I1.g_pKeyHandle, I1.g_usNodeId, &oIsEnabled, p_pErrorCode) == 0)
			{
				I1.LogError("VCS_GetEnableState", lResult, *p_pErrorCode);
				lResult = MMC_FAILED;
			}

			if(lResult==0)
			{
				if(!oIsEnabled)
				{
					if(VCS_SetEnableState(I1.g_pKeyHandle,I1.g_usNodeId, p_pErrorCode) == 0)
					{
                        I1.LogError("VCS_SetEnableState", lResult, *p_pErrorCode);
						lResult = MMC_FAILED;
					}
				}
			}
		}
	}
	return lResult;
}
int Demo(unsigned int* p_pErrorCode)
{
    int lResult = MMC_SUCCESS;
    unsigned int lErrorCode = 0;

    if(DemoGobackZero(I1.g_pKeyHandle, I1.g_usNodeId, lErrorCode)!=MMC_SUCCESS)
    {
        return lResult;

    }
            lResult = DemoProfileCurrentMode(I1.g_pKeyHandle, I1.g_usNodeId, lErrorCode);
            if(lResult != MMC_SUCCESS)
            {
                I1.LogError("DemoProfileCurrentMode", lResult, lErrorCode);
            }
            else
            {
                if(VCS_SetDisableState(I1.g_pKeyHandle, I1.g_usNodeId, &lErrorCode) == 0)
                {
                    I1.LogError("VCS_SetDisableState", lResult, lErrorCode);
                    lResult = MMC_FAILED;
                }
            }



    return lResult;
}
int main(int argc, char** argv)
{
    //int c=emg_adc_convert();
	int lResult = MMC_FAILED;
	unsigned int ulErrorCode = 0;
    //openserial();
	I1.PrintHeader();
	I1.SetDefaultParameters();
	if((lResult = ParseArguments(argc, argv))!=MMC_SUCCESS)
	{
		return lResult;
	}
	I1.PrintSettings();
	switch(g_eAppMode)
	{
		case AM_DEMO:
		{
			if((lResult = OpenDevice(&ulErrorCode))!=MMC_SUCCESS)
			{
                I1.LogError("OpenDevice", lResult, ulErrorCode);
				return lResult;
			}

			if((lResult = PrepareDemo(&ulErrorCode))!=MMC_SUCCESS)
			{
                I1.LogError("PrepareDemo", lResult, ulErrorCode);

				return lResult;
			}

			if((lResult = Demo(&ulErrorCode))!=MMC_SUCCESS)
			{
                I1.LogError("Demo", lResult, ulErrorCode);

				return lResult;
			}

			if((lResult = I1.CloseDevice(&ulErrorCode))!=MMC_SUCCESS)
			{
                I1.LogError("CloseDevice", lResult, ulErrorCode);
				return lResult;
			}
		} break;
		case AM_INTERFACE_LIST:
            I1.PrintAvailableInterfaces();
			break;
		case AM_PROTOCOL_LIST:
            I1.PrintAvailableProtocols();
			break;
		case AM_VERSION_INFO:
		{
			if((lResult =OpenDevice(&ulErrorCode))!=MMC_SUCCESS)
			{
                I1.LogError("OpenDevice", lResult, ulErrorCode);
				return lResult;
			}

			if((lResult = I1.PrintDeviceVersion()) != MMC_SUCCESS)
		    {
                I1.LogError("PrintDeviceVersion", lResult, ulErrorCode);
				return lResult;
		    }

			if((lResult = I1.CloseDevice(&ulErrorCode))!=MMC_SUCCESS)
			{
                I1.LogError("CloseDevice", lResult, ulErrorCode);
				return lResult;
			}
		} break;
		case AM_UNKNOWN:
			printf("unknown option\n");
			break;
	}

	return lResult;
    return 0;
}