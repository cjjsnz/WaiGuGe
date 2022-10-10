//
// Created by cjj on 2022/8/11.
//

#include "Motor.h"
#include "Definitions.h"

int DemoGobackZero(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode) {
    int lResult = MMC_SUCCESS;
    stringstream msg;
    msg << "set profile Homing position mode, node = " << p_usNodeId;
    LogInfo(msg.str());
    int Initialcurrent=150;
    int* ipCurrent;
    if(VCS_ActivateCurrentMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode))
        if(VCS_SetCurrentMustEx(p_DeviceHandle, p_usNodeId, Initialcurrent, &p_rlErrorCode))

            for(int i = 0;i<100;i++)
                {
                    sleep(1);
                    VCS_GetCurrentIsAveragedEx(p_DeviceHandle, p_usNodeId, ipCurrent, &p_rlErrorCode);

                    stringstream msg;
                    msg <<"Initial Current Must Be "<< Initialcurrent<<"mA"<< " current = " << * ipCurrent;
                    LogInfo(msg.str());
                    if(* ipCurrent>135)
                        DoHoming(p_DeviceHandle, p_usNodeId, homeacc,speedSwitch, speedIndex,  homeOffset, currentThreshold, homePosition, p_rlErrorCode);
                        sleep(1);
                        getMotorState(p_DeviceHandle, p_usNodeId, p_rlErrorCode,current+i,velocity+i,position+i,pCurrentM+i);
                        stringstream msg;
                        msg <<"Initial Current Must Be "<<pCurrentM[i]<<"mA"<< " current = " << current[i]<<"mA  velocity = " <<velocity[i]<<"rpm"<<" position = "<<position[i]<<"qc";
                        LogInfo(msg.str());


                }


    return lResult;
}


int DemoGobackZero(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode) {
    int lResult = MMC_SUCCESS;
    stringstream msg;
    msg << "set profile position mode, node = " << p_usNodeId;
    LogInfo(msg.str());

    int positionis;
    VCS_GetPositionIs(p_DeviceHandle, p_usNodeId,&positionis, &p_rlErrorCode);

    if(VCS_ClearFault(p_DeviceHandle, p_usNodeId,&p_rlErrorCode))
        if(VCS_SetEnableState(p_DeviceHandle, p_usNodeId,&p_rlErrorCode))
            if(VCS_ActivateProfilePositionMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode))
                if(VCS_SetPositionProfile(p_DeviceHandle, p_usNodeId,1000,10000,2000,&p_rlErrorCode))
                    if (VCS_MoveToPosition(p_DeviceHandle, p_usNodeId, 0, 1, 1, &p_rlErrorCode))
                        if (VCS_MoveToPosition(p_DeviceHandle, p_usNodeId, 0, 1, 0, &p_rlErrorCode))

                            /* if (VCS_ActivateProfilePositionMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0) {
                                 LogError("VCS_ActivateProfilePositionMode", lResult, p_rlErrorCode);
                                 lResult = MMC_FAILED;
                             } else {

                                 long targetPosition = 0;
                                 stringstream msg;
                                 msg << "move to position = " << targetPosition << ", node = " << p_usNodeId;
                                 LogInfo(msg.str());

                                if (VCS_MoveToPosition(p_DeviceHandle, p_usNodeId, targetPosition, 1, 0, &p_rlErrorCode) == 0) {
                                     LogError("VCS_MoveToPosition", lResult, p_rlErrorCode);
                                     lResult = MMC_FAILED;

                                 }
                                if(VCS_ActivatePositionMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode)==0)
                                {
                                    LogError("VCS_ActivatePositionMode", lResult, p_rlErrorCode);
                                    lResult = MMC_FAILED;

                                }
                                if(VCS_SetPositionMust(p_DeviceHandle, p_usNodeId, targetPosition,& p_rlErrorCode)==0)
                                {
                                    LogError("VCS_SetPositionMust", lResult, p_rlErrorCode);
                                    lResult = MMC_FAILED;

                                }*/
                            for(int i = 0;i<10;i++)
                            {
                                sleep(1);
                                getMotorState(p_DeviceHandle, p_usNodeId, p_rlErrorCode,current+i,velocity+i,position+i,pCurrentM+i);
                                stringstream msg;
                                msg <<"Current Must Be "<<pCurrentM[i]<< " current = " << current[i]<<" velocity = " <<velocity[i]<<"rpm"<<" position = "<<position[i]<<"qc";
                                LogInfo(msg.str());
                            }

    return lResult;
}