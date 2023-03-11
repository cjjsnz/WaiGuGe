#pragma once
//Motor.h  Motor类头文件
#include "Definitions.h"
typedef unsigned int DWORD;
typedef unsigned short WORD;
class CMotor
{
public:
	WORD m_wNodeID;
	long m_lProfilePos;
    int m_lActualPos;//位置
	DWORD m_dwProfileVel;
	int m_dwActualVel;//速度
	DWORD m_dwProfileAcc;//加速度
	DWORD m_dwProfileDec;//减速度
	bool m_bIsAbsolute; //是否为绝对运动
	short m_sProfileCur,m_sActualCur;//电流

	CMotor();
	void UpdateProfileData(WORD node, long profile_position, DWORD profile_velocity, DWORD profile_acceleration, DWORD profile_deceleration, bool is_absolute, short profile_current);
	void UpdateProfileData(CMotor*);
	void UpdateActualData(long actual_position, DWORD actual_velocity, short actual_current);
};