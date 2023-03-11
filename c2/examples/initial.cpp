
#include "initial.h"

using namespace std;
        void initialdefination ::LogError(string functionName, int p_lResult, unsigned int p_ulErrorCode)
        {
            cerr << g_programName << ": " << functionName << " failed (result=" << p_lResult << ", errorCode=0x" << std::hex << p_ulErrorCode << ")"<< endl;
        }
        void initialdefination ::LogInfo(string message)
        {
            cout << message << endl;
        }
        void initialdefination ::PrintUsage()
        {
            cout << "Usage: HelloEposCmd" << endl;
            cout << "\t-h : this help" << endl;
            cout << "\t-n : node id (default 1)" << endl;
            cout << "\t-d   : device name (EPOS2, EPOS4, default - EPOS4)"  << endl;
            cout << "\t-s   : protocol stack name (MAXON_RS232, CANopen, MAXON SERIAL V2, default - MAXON SERIAL V2)"  << endl;
            cout << "\t-i   : interface name (RS232, USB, CAN_ixx_usb 0, CAN_kvaser_usb 0,... default - USB)"  << endl;
            cout << "\t-p   : port name (COM1, USB0, CAN0,... default - USB0)" << endl;
            cout << "\t-b   : baudrate (115200, 1000000,... default - 1000000)" << endl;
            cout << "\t-l   : list available interfaces (valid device name and protocol stack required)" << endl;
            cout << "\t-r   : list supported protocols (valid device name required)" << endl;
            cout << "\t-v   : display device version" << endl;
        }
        void initialdefination ::SeparatorLine()
        {
            const int lineLength = 65;
            for(int i=0; i<lineLength; i++)
            {
                cout << "-";
            }
            cout << endl;
        }
        void initialdefination ::PrintSettings()
        {
            stringstream msg;

            msg << "default settings:" << endl;
            msg << "node id             = " << g_usNodeId << endl;
            msg << "node id             = " << g_usNodeId2<< endl; 
            msg << "device name         = '" << g_deviceName << "'" << endl;
            msg << "protocal stack name = '" << g_protocolStackName << "'" << endl;
            msg << "interface name      = '" << g_interfaceName << "'" << endl;
            msg << "port name           = '" << g_portName << "'"<< endl;
            msg << "baudrate            = " << g_baudrate;

            LogInfo(msg.str());

            SeparatorLine();
        }
        void initialdefination :: SetDefaultParameters()
        {
            //USB
            g_usNodeId = 1;
            g_usNodeId2 =2;
            g_deviceName = "EPOS2";
            g_protocolStackName = "MAXON SERIAL V2";
            g_interfaceName = "USB";
            g_portName = "USB0";
            g_baudrate = 1000000;
        }
        int initialdefination ::CloseDevice(unsigned int* p_pErrorCode)
        {
            int lResult = MMC_FAILED;

            *p_pErrorCode = 0;

            LogInfo("Close device");

            if(VCS_CloseDevice(g_pKeyHandle, p_pErrorCode)!=0 && *p_pErrorCode == 0)
            {
                lResult = MMC_SUCCESS;
            }

            return lResult;
        }

        int initialdefination ::MaxFollowingErrorDemo(unsigned int& p_rlErrorCode)
        {
            int lResult = MMC_SUCCESS;
            const unsigned int EXPECTED_ERROR_CODE = 0x8611;
            unsigned int lDeviceErrorCode = 0;

            lResult = VCS_ActivateProfilePositionMode(g_pKeyHandle, g_usNodeId, &p_rlErrorCode);

            if(lResult)
            {
                lResult = VCS_SetMaxFollowingError(g_pKeyHandle, g_usNodeId, 1, &p_rlErrorCode);
            }

            if(lResult)
            {
                lResult = VCS_MoveToPosition(g_pKeyHandle, g_usNodeId, 1000, 1, 1, &p_rlErrorCode);
            }

            if(lResult)
            {
                lResult = VCS_GetDeviceErrorCode(g_pKeyHandle, g_usNodeId, 1, &lDeviceErrorCode, &p_rlErrorCode);
            }

            if(lResult)
            {
                lResult = lDeviceErrorCode == EXPECTED_ERROR_CODE ? MMC_SUCCESS : MMC_FAILED;
            }

            return lResult;
        }
        void initialdefination ::PrintHeader()
        {
            SeparatorLine();

            LogInfo("Epos Command Library Example Program, (c) maxonmotor ag 2014-2019");

            SeparatorLine();
        }
        int initialdefination ::PrintAvailablePorts(char* p_pInterfaceNameSel)
        {
            int lResult = MMC_FAILED;
            int lStartOfSelection = 1;
            int lMaxStrSize = 255;
            char* pPortNameSel = new char[lMaxStrSize];
            int lEndOfSelection = 0;
            unsigned int ulErrorCode = 0;

            do
            {
                if(!VCS_GetPortNameSelection((char*)g_deviceName.c_str(), (char*)g_protocolStackName.c_str(), p_pInterfaceNameSel, lStartOfSelection, pPortNameSel, lMaxStrSize, &lEndOfSelection, &ulErrorCode))
                {
                    lResult = MMC_FAILED;
                    LogError("GetPortNameSelection", lResult, ulErrorCode);
                    break;
                }
                else
                {
                    lResult = MMC_SUCCESS;
                    printf("            port = %s\n", pPortNameSel);
                }

                lStartOfSelection = 0;
            }
            while(lEndOfSelection == 0);

            return lResult;
        }
        int initialdefination ::PrintAvailableInterfaces()
        {
            int lResult = MMC_FAILED;
            int lStartOfSelection = 1;
            int lMaxStrSize = 255;
            char* pInterfaceNameSel = new char[lMaxStrSize];
            int lEndOfSelection = 0;
            unsigned int ulErrorCode = 0;

            do
            {
                if(!VCS_GetInterfaceNameSelection((char*)g_deviceName.c_str(), (char*)g_protocolStackName.c_str(), lStartOfSelection, pInterfaceNameSel, lMaxStrSize, &lEndOfSelection, &ulErrorCode))
                {
                    lResult = MMC_FAILED;
                    LogError("GetInterfaceNameSelection", lResult, ulErrorCode);
                    break;
                }
                else
                {
                    lResult = MMC_SUCCESS;

                    printf("interface = %s\n", pInterfaceNameSel);

                    PrintAvailablePorts(pInterfaceNameSel);
                }

                lStartOfSelection = 0;
            }
            while(lEndOfSelection == 0);

            SeparatorLine();

            delete[] pInterfaceNameSel;

            return lResult;
        }
        int initialdefination ::PrintDeviceVersion()
        {
            int lResult = MMC_FAILED;
            unsigned short usHardwareVersion = 0;
            unsigned short usSoftwareVersion = 0;
            unsigned short usApplicationNumber = 0;
            unsigned short usApplicationVersion = 0;
            unsigned int ulErrorCode = 0;

            if(VCS_GetVersion(g_pKeyHandle, g_usNodeId, &usHardwareVersion, &usSoftwareVersion, &usApplicationNumber, &usApplicationVersion, &ulErrorCode))
            {
                printf("%s Hardware Version    = 0x%04x\n      Software Version    = 0x%04x\n      Application Number  = 0x%04x\n      Application Version = 0x%04x\n",
                       g_deviceName.c_str(), usHardwareVersion, usSoftwareVersion, usApplicationNumber, usApplicationVersion);
                lResult = MMC_SUCCESS;
            }

            return lResult;
        }
        int initialdefination :: PrintAvailableProtocols()
        {
            int lResult = MMC_FAILED;
            int lStartOfSelection = 1;
            int lMaxStrSize = 255;
            char* pProtocolNameSel = new char[lMaxStrSize];
            int lEndOfSelection = 0;
            unsigned int ulErrorCode = 0;

            do
            {
                if(!VCS_GetProtocolStackNameSelection((char*)g_deviceName.c_str(), lStartOfSelection, pProtocolNameSel, lMaxStrSize, &lEndOfSelection, &ulErrorCode))
                {
                    lResult = MMC_FAILED;
                    LogError("GetProtocolStackNameSelection", lResult, ulErrorCode);
                    break;
                }
                else
                {
                    lResult = MMC_SUCCESS;

                    printf("protocol stack name = %s\n", pProtocolNameSel);
                }

                lStartOfSelection = 0;
            }
            while(lEndOfSelection == 0);

            SeparatorLine();

            delete[] pProtocolNameSel;

            return lResult;
        }
