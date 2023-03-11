#include "Motor.h"
CMotor::CMotor()
{
	UpdateProfileData(1,0, 0, 0, 0, true,0);
	UpdateActualData(0,0,0);
}

void CMotor::UpdateProfileData(WORD node,long profile_position, DWORD profile_velocity, DWORD profile_acceleration, DWORD profile_deceleration, bool is_absolute,short profile_current)
{
	m_wNodeID = node;
	m_lProfilePos = profile_position;
	m_dwProfileVel = profile_velocity;
	m_dwProfileAcc = profile_acceleration;
	m_dwProfileDec = profile_deceleration;
	m_bIsAbsolute = is_absolute;
	m_sProfileCur = profile_current;
}

void CMotor::UpdateProfileData(CMotor* pmpd)
{
	m_lProfilePos = pmpd->m_lProfilePos;
	m_dwProfileVel = pmpd->m_dwProfileVel;
	m_dwProfileAcc = pmpd->m_dwProfileAcc;
	m_dwProfileDec = pmpd->m_dwProfileDec;
	m_bIsAbsolute = pmpd->m_bIsAbsolute;
	m_sProfileCur = pmpd->m_sProfileCur;
}

void CMotor::UpdateActualData(long actual_position, DWORD actual_velocity, short actual_current)
{
	m_lActualPos = actual_position;
	m_dwActualVel = actual_velocity;
	m_sActualCur = actual_current;
}