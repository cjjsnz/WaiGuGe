#include "MotorControl.h"
#include<sstream>
#include<iostream>
#include<string>
#ifndef VOID
#define VOID void
#endif
//静态成员变量的类外初始化
HANDLE CMotionControl::m_hKeyHandle;

CMotionControl* CMotionControl::m_pSelf;

//单电机电流模式 先是位置模式，当电流值大于阈值30ms后，进入电流模式
void CMotionControl::SinMotWithCur(WORD nodeID)
{
	m_pSelf->m_wNodeID = nodeID;
	//if (SinMotPntToPnt(nodeID))
		
}

long CMotionControl::GetPositon(WORD nodeID)
{
	int p;
	VCS_GetPositionIs(m_hKeyHandle, nodeID, &p, &m_dwErrorCode);
	return p;
}


//单电机电流模式 一直是电流模式运行
void CMotionControl::SinMotOnCur(WORD nodeID)
{
	VCS_ClearFault(m_hKeyHandle, nodeID, &m_dwErrorCode);
	VCS_SetEnableState(m_hKeyHandle, nodeID, &m_dwErrorCode);  //使能
	VCS_ActivateCurrentMode(m_hKeyHandle, nodeID, &m_dwErrorCode);//激活电流模式
	VCS_SetCurrentMust(m_hKeyHandle, nodeID, m_motor[nodeID-1].m_sProfileCur, &m_dwErrorCode);//设置电流模式电流值
}

// 单电机点对点位置运动
bool CMotionControl::SinMotPntToPnt(WORD nodeID)
{
	//int p;
	if (VCS_ClearFault(m_hKeyHandle, nodeID, &m_dwErrorCode))// 排除故障
	{
		if (VCS_SetEnableState(m_hKeyHandle, nodeID, &m_dwErrorCode))//使能
		{
			if (VCS_ActivateProfilePositionMode(m_hKeyHandle, nodeID, &m_dwErrorCode))//激活位置运动模式
			{
				if (VCS_SetPositionProfile(m_hKeyHandle, nodeID, m_motor[nodeID - 1].m_dwProfileVel,
					m_motor[nodeID - 1].m_dwProfileAcc, m_motor[nodeID - 1].m_dwProfileDec, &m_dwErrorCode))//设置位置参数（速度 加速度 减速度）
				{
					//VCS_GetPositionIs(m_hKeyHandle,nodeID,&p,&m_dwErrorCode);
					if (VCS_MoveToPosition(m_hKeyHandle, nodeID, m_motor[nodeID - 1].m_lProfilePos,m_motor[nodeID - 1].m_bIsAbsolute, true, &m_dwErrorCode))//开始运动至目标位置 （目标位置 是否绝对运动 是否立即运动）
					{
						//if(p== m_motor[nodeID - 1].m_lProfilePos)
						return true;
					}	
				}
			}
		}
	}
	return false;
}

//开机检测
bool CMotionControl::OpenDevice(char *my_DeviceName,char *my_ProtocolStackName,char *my_InterfaceName,char *my_PortName)
{
    char bMode;
	DWORD dwProfileVelocity;
	DWORD dwProfileAcceleration;
	DWORD dwProdileDeceleration;
	int lProfilePosition;
	bool bTemp=true;
	if(m_hKeyHandle)
	{
		VCS_CloseDevice(m_hKeyHandle,&m_dwErrorCode);
		m_hKeyHandle=0;
	}
	m_hKeyHandle=VCS_OpenDevice(my_DeviceName, my_ProtocolStackName, my_InterfaceName, my_PortName, &m_dwErrorCode);
	
	if(m_hKeyHandle)
	{
		for(WORD i=1;i<3;i++)
		{
		m_motor[i-1].UpdateProfileData(i,100,100,100,100,true,0);
		if(VCS_ClearFault(m_hKeyHandle,m_motor[i-1].m_wNodeID,&m_dwErrorCode))
			if(VCS_GetOperationMode(m_hKeyHandle,m_motor[i-1].m_wNodeID,&bMode,&m_dwErrorCode))
				if(VCS_SetPositionProfile(m_hKeyHandle,m_motor[i-1].m_wNodeID,m_motor[i-1].m_dwProfileVel,m_motor[i-1].m_dwProfileAcc,m_motor[i-1].m_dwProfileDec,&m_dwErrorCode))
					if(VCS_SetOperationMode(m_hKeyHandle,m_motor[i-1].m_wNodeID,OMD_PROFILE_POSITION_MODE,&m_dwErrorCode))
						if(VCS_GetPositionProfile(m_hKeyHandle,m_motor[i-1].m_wNodeID,&dwProfileVelocity,&dwProfileAcceleration,&dwProdileDeceleration,&m_dwErrorCode))
							if(VCS_GetPositionIs(m_hKeyHandle,m_motor[i-1].m_wNodeID,&lProfilePosition,&m_dwErrorCode))
								bTemp=true;
							else{bTemp=false;break;}
						else{bTemp=false;break;}						
				else{bTemp=false;break;}
			else{bTemp=false;break;}
		else{bTemp=false;break;}
		}
	}

	return bTemp;
}
//四个电机全部停止（使不能）
bool CMotionControl::DisableAllDevices()
{
	for (WORD i = 1; i < 3; ++i)
	{
		if (!VCS_SetDisableState(m_hKeyHandle, i, &m_dwErrorCode))
		{
			return false;
		}
	}
	return true;
}

bool CMotionControl::IsZeroPosition()
{
	VCS_GetPositionIs(m_hKeyHandle, 1, &position, &m_dwErrorCode);
	//std::cout<<position << std::endl;
	if (position >= 0)
		return true;
	else
		return false;
}

void CMotionControl::DisableOneDevice()
{
	//std::cout << position << std::endl;
	VCS_SetDisableState(m_hKeyHandle, 1, &m_dwErrorCode);
}

//设置四个电机进入急停状态，然后四个电机全部停止
void CMotionControl::AllMotorsHalt()
{
	for (WORD i = 1; i < 5; ++i)
	{
		VCS_SetQuickStopState(m_hKeyHandle, i, &m_dwErrorCode);
	}
	DisableAllDevices();
}

//将显示的数据转换成脉冲数，电机1,2输出量为电机出轴转动角度制，电机3,4为电机转动圈数
long CMotionControl::DisplayToPulse(WORD node, long display)
{
	//将显示的数据转换成脉冲数
	//电机1,2输出量为电机出轴转动角度制，电机3,4为电机转动圈数
	return display * REDUCTION[node - 1];
}

//将电机脉冲数转换为电机1，2的转动角度或者电机3，4的转动圈数
double CMotionControl::PulseToDisplay(WORD node, double pulse)
{	//将电机脉冲数转换为电机1，2的转动角度或者电机3，4的转动圈数
	return pulse / REDUCTION[node - 1];
}

//判断电流平均值是否超过模式切换阈值，是否切换为电流模式
bool CMotionControl::CurrentJudge(WORD node)
{
	std::deque<short> ds[4] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
	short sum = 0;
	VCS_GetCurrentIsAveraged(m_hKeyHandle, node, &(m_motor[node - 1].m_sActualCur), &m_dwErrorCode);
	ds[node - 1].push_back(m_motor[node - 1].m_sActualCur);
	ds[node - 1].pop_front();
	for (short i : ds[node - 1])
	{
		sum += i;
	}
	double fAve = sum / 3;
	//std::cout << node << "\t" << fAve << "\n";
	return (abs(fAve) > abs(FORWARD_CURRENT_CRITICAL[node - 1]));//1号电机的电流为负
}

//跟踪文件轨迹
/*void CMotionControl::TraceTracking(CString trace, WORD runTimeCnt)  //跟踪文件轨迹 runTimeCnt 运行次数，文件命名用
{
	if (!IsHomed())////如果四个电机不在初始位置
	{
		AllMotorsHoming(); ////所有电机回0
		Sleep(2000);//预留两秒回零点时间
	}
	m_pSelf->m_usTimeCnt = 0;
	for (WORD node = 1; node < 5; ++node)
	{
		VCS_SetEnableState(m_hKeyHandle, node, &m_dwErrorCode);
		VCS_ActivateProfilePositionMode(m_hKeyHandle, node, &m_dwErrorCode);
	}
	//------------Read Trace form File----------------------------
	m_pSelf->m_fTraceFile.open(trace);

	std::stringstream recordFile;
	recordFile << "Record\\record_" << runTimeCnt << ".txt";//文件名及相对路径
	m_pSelf->m_fRecordFile.open(recordFile.str(), std::ios::out);//文件名，写入模式
	if (m_pSelf->m_fTraceFile.is_open() && m_pSelf->m_fRecordFile.is_open())
	{
		SYSTEMTIME systm;
		GetLocalTime(&systm);
		WORD month = systm.wMonth;
		WORD day = systm.wDay;
		WORD hour = systm.wHour;
		WORD minute = systm.wMinute;

		m_pSelf->m_fRecordFile << "File Name:" << CT2A(trace) << '\n';
		m_pSelf->m_fRecordFile << "Time:" << month << '/' << day << "   " << hour << ':' << minute << '\n';
		m_pSelf->m_fRecordFile << "Frequence:10Hz\n";
		m_pSelf->m_fRecordFile << "Pos1\tPos2\tPos3\tPos4\t\tVel1\tVel2\tVel3\tVel4\t\tCur1\tCur2\tCur3\tCur4\t\tDtPs1\tDtPs2\tDtPs3\tDtPs4\n";
		SetTimer(m_hWnd, IDT_TRACK, TRACK_CYCLE, (TIMERPROC)TrackTimerProc);
	}
	//------------Read Trace form File----------------------------
}*/

//从文件trace读入pos信息，执行，输入pos,vec,cur,deltapos信息到record
/*VOID CMotionControl::TrackTimerProc(HWND hwnd, UINT message, UINT_PTR idTimer, DWORD dwTime)
{
	//--------Read Trace from File------------------------
	if (!m_pSelf->m_fTraceFile.eof())
	{
		static std::vector<long> pos(4, 0);//创建一个vector，元素个数为4，且值均为0。
		static std::vector<long>lastPos;
		static std::vector<bool> svbCurOver(4, false);
		long vel = 0, acc = 1000;

		lastPos = pos;

		for (WORD node = 1; node < 5; ++node)
		{
			if (!svbCurOver[node - 1])
			{
				svbCurOver[node - 1] = m_pSelf->CurrentJudge(node);//判断电机是否为电流模式
				m_pSelf->m_fTraceFile >> pos[node - 1];//将trace文件中的数据输入到pos
				vel = (pos[node - 1] - lastPos[node - 1]) * 1000 * 60 / (m_pSelf->TRACK_CYCLE * 4096);//pos为脉冲数，速度单位rpm
				VCS_SetPositionProfile(m_pSelf->m_hKeyHandle, node, vel, acc, acc, &m_pSelf->m_dwErrorCode);
				VCS_MoveToPosition(m_pSelf->m_hKeyHandle, node, pos[node - 1], true, true, &m_pSelf->m_dwErrorCode);

				if (m_pSelf->CurrentJudge(node))
				{
					svbCurOver[node - 1] = true;
					if (node == 1)
						VCS_SetQuickStopState(m_hKeyHandle, 1, &m_pSelf->m_dwErrorCode);
					else
					{
						VCS_ActivateCurrentMode(m_hKeyHandle, node, &m_pSelf->m_dwErrorCode);
						VCS_SetCurrentMust(m_hKeyHandle, node, m_pSelf->FORWARD_CURRENT[node - 1], &m_pSelf->m_dwErrorCode);
					}
				}
			}
		}

		if (svbCurOver[0] && svbCurOver[1] && svbCurOver[2] && svbCurOver[3])
		{
			m_pSelf->m_fTraceFile.close();
			m_pSelf->m_fRecordFile.close();
			KillTimer(hwnd, idTimer);
		}

		std::vector<double> deltaPos(4);
		for (WORD i = 0; i < 4; ++i)//GetMotorInfo()
		{
			m_pSelf->GetMotorInfo(i + 1);
			deltaPos[i] = pos[i] - m_pSelf->m_motor[i].m_lActualPos;
		}
		m_pSelf->m_fRecordFile << m_pSelf->m_motor[0].m_lActualPos << '\t' << m_pSelf->m_motor[1].m_lActualPos << '\t'
			<< m_pSelf->m_motor[2].m_lActualPos << '\t' << m_pSelf->m_motor[3].m_lActualPos << "\t\t"
			<< m_pSelf->m_motor[0].m_dwActualVel << '\t' << m_pSelf->m_motor[1].m_dwActualVel << '\t'
			<< m_pSelf->m_motor[2].m_dwActualVel << '\t' << m_pSelf->m_motor[3].m_dwActualVel << "\t\t"
			<< m_pSelf->m_motor[0].m_sActualCur << '\t' << m_pSelf->m_motor[1].m_sActualCur << '\t'
			<< m_pSelf->m_motor[2].m_sActualCur << '\t' << m_pSelf->m_motor[3].m_sActualCur << "\t\t"
			<< deltaPos[0] << '\t' << deltaPos[1] << '\t' << deltaPos[2] << '\t' << deltaPos[3] << "\t\n";



	}
	else//reach the end of the file
	{
		m_pSelf->m_fTraceFile.close();
		m_pSelf->m_fRecordFile.close();
		KillTimer(hwnd, idTimer);
	}
	//--------Read Trace from File------------------------
	m_pSelf->m_usTimeCnt++;
	return VOID();
}*/

//上电复位
void CMotionControl::InitialReset()
{
	m_pSelf->m_usTimeCnt = 0;
	for (WORD i = 1; i < 5; ++i)
	{
		VCS_SetEnableState(m_hKeyHandle, i, &m_dwErrorCode);//使能
		VCS_ActivateVelocityMode(m_hKeyHandle, i, &m_dwErrorCode);//激活速度模式
		VCS_SetVelocityMust(m_hKeyHandle, i, INITIAL_VELOCITY[i - 1], &m_dwErrorCode);//设置速度值参数
		m_pSelf->m_bInitFinished[i - 1] = false;
	}
	//SetTimer(m_hWnd, IDT_INIT, INIT_CYCLE, (TIMERPROC)InitTimerProc); //设置每隔10ms触发一次InitTimerProc函数
}

//获取位置实际值、速度实际平均值和电流实际平均值
bool CMotionControl::GetMotorInfo(int nodeID)
{
	if (VCS_GetPositionIs(m_hKeyHandle, nodeID, &m_motor[nodeID - 1].m_lActualPos, &m_dwErrorCode))
		if (VCS_GetVelocityIsAveraged(m_hKeyHandle, nodeID, &m_motor[nodeID - 1].m_dwActualVel, &m_dwErrorCode))
			if (VCS_GetCurrentIsAveraged(m_hKeyHandle, nodeID, &m_motor[nodeID - 1].m_sActualCur, &m_dwErrorCode))
				return true;
	return false;
}

//单个电机急停
bool CMotionControl::SingleMotorHalt(WORD nodeID)
{
	if (VCS_SetQuickStopState(m_hKeyHandle, nodeID, &m_dwErrorCode))
		return true;
	return false;
}

//单个电机回初始位置
bool CMotionControl::SingleMotorHoming(WORD nodeID)
{
	if (VCS_ClearFault(m_hKeyHandle, nodeID, &m_dwErrorCode))//清除故障
		if (VCS_SetEnableState(m_hKeyHandle, nodeID, &m_dwErrorCode))//使能
			if (VCS_ActivateProfilePositionMode(m_hKeyHandle, nodeID, &m_dwErrorCode))//激活位置模式
				if (VCS_SetPositionProfile(m_hKeyHandle, nodeID, FORWARD_VELOCITY[nodeID - 1], HOMING_ACCELERATION[nodeID -1], HOMING_DECELERATION[nodeID -1], &m_dwErrorCode))//设置位置模式的速度、加速度、减速度参数
					if (VCS_MoveToPosition(m_hKeyHandle, nodeID, HOMING_POSITION[nodeID - 1], true, true, &m_dwErrorCode))//设置初始位置，立即移动到初始位置（绝对值，立即）
						if (VCS_MoveToPosition(m_hKeyHandle, nodeID, 0, true, false, &m_dwErrorCode))//回0
							return true;
	return false;
}

//3、4homing,再1、2homing
bool CMotionControl::AllMotorsHoming()
{
	for (WORD node = 1; node < 3; node++)
		VCS_ClearFault(m_hKeyHandle, node, &m_dwErrorCode);
	if (!(SingleMotorHoming(1) && SingleMotorHoming(2)))return false;//1、2电机回0
	return true;
}

//判断四个电机是否都为初始位置
bool CMotionControl::IsHomed()
{
	for (WORD i = 1; i < 5; ++i)
	{
		VCS_GetPositionIs(m_hKeyHandle, i, &m_motor[i - 1].m_lActualPos, &m_dwErrorCode);
		if (abs(m_motor[i - 1].m_lActualPos) > 500) return false;
	}
	return true;
}
