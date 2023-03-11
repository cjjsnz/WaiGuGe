#pragma once
#include "Motor.h"
#include <cmath>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<deque>

enum MotionMode { SINGLE_MOTOR_MODE, TRACE_TRACKING_MODE };
enum CurrentState { INITIAL, ONCE, TWICE, FINAL };
typedef void* HANDLE;
class CMotionControl
{
public:
	CMotor m_motor[4];
	std::ofstream outPosition;
	static CMotionControl* m_pSelf;//回调函数参数只能是静态变量，所以设置一个静态CMotionControl指针作为回调函数的参数
	WORD m_wNodeID;//回调函数用
	unsigned short m_usTimeCnt;//获取点的个数，10ms/次
	bool m_bInitFinished[4];//初始化完成标志
	DWORD m_dwErrorCode;//储存错误代码
	int position;
	std::fstream m_fTraceFile;
	std::ofstream m_fRecordFile;
	static HANDLE m_hKeyHandle;
	

    //INITIAL, Velocity Mode, 初始化
	const long INITIAL_VELOCITY[4] = { 600,-200,-1500,-1500 }; 
	const short INITIAL_CURRENT[4] = { 100,100,200,200 };//初始化复位时判断到达极限位置的电流阈值

	//INITIAL OFFSTET, Profile Position Mode， 复位，ResetDevice,重新设置此位置为0位
	//const long INITIAL_OFFSET_POSITION[4] = { -100000,10000,130000,160000 };

	////vet offset，zhang 20200812         -80000,10000,130000,160000
	///ktest offset, jiang 20200813        -20000,10000,130000,160000

	const long INITIAL_OFFSET_POSITION[4] = { -60000,30000,80000,100000 };
	const DWORD INITIAL_OFFSET_VELOCITY[4] = { 2000,500,6000,6000 };
	const DWORD INITIAL_OFFSET_ACCELERATION[4] = { 6000,6000,6000,6000 };
	const DWORD INITIAL_OFFSET_DECELERATION[4] = { 6000,6000,6000,6000 };

	//const long FORWARD_POSITION[4] = { -65162,10000,  359814,559814 };//forward极限位置

	//vet       -65162,000,  329814,559814
	//ktest
	// 1 2： -65162,80000,  329814,559814 
	//3：-65162,60000,  309814,559814
	//4：-125162,60000,  309814,559814
	//5：-185162,68000,  309814,559814
	//6： -255162,75000,  309814,55984514
	//7： -205162,110000,  459814,559814
	//8： -285162,120000,  459814,659814
	const long FORWARD_POSITION[4] = { -74000 ,364047,  509814,609814 };
	const long GRASP_POSITION[4] = { -24000 ,350053,  509814,609814 };
	const long SCREW_POSITION[4] = { -56000 ,230053,  509814,609814 };
	//速度 20200812 zhang VET 5000,2000,7000,7000
	//速度 20200812 zhang KTest 2000,2000,7000,7000
	DWORD FORWARD_VELOCITY[4] = { 2000,2000,5000,5000 };//forward速度 5000,2000,7000,7000
	const DWORD FORWARD_ACCELERATION[4] = { 5000,5000,50000,30000};//forward加速度
	const DWORD FORWARD_DECELERATION[4] = { 5000,5000,50000,30000 };//forward减速度
	const short FORWARD_CURRENT_CRITICAL[4] = { -50,50,50,50 };//位置模式与电流模式的阈值
	short FORWARD_CURRENT[4] = { -20,20,20,20 };//进入电流模式之后的电流值
	const float kMaxForwardVelocity = 1;
	const float kMinForwardVelocity = 0.2;
	const float kBackVelocity = 0.6; 
	
	//BACK, Profile Position Mode
	const long BACK_POSITION[4] = { 0,0,0,0 };
	//const DWORD BACK_VELOCITY[4] = { 1000,1800,4500,4500 };//back速度
	const DWORD BACK_VELOCITY[4] = { 2500,2000,7000,7000 };
	const DWORD BACK_ACCELERATION[4] = { 10000,5000,300000,30000 };//forward加速度
	const DWORD BACK_DECELERATION[4] = { 10000,5000,300000,30000 };//forward减速度
	short BACK_CURRENT[4] = { 10,-30,-50,-50 };//进入电流模式之后的电流值

	//HOMING,Profile Position Mode
	const long HOMING_POSITION[4] = { 0,0,-30000,-35000 };//Home按钮回初始位置 
	const long HOMING_VELOCITY[4] = { 1000,1000,2500,2500 };//Home按钮回初始位置速度
	const DWORD HOMING_ACCELERATION[4] = { 5000,5000,5000,5000 };//Home按钮回初始加速度
	const DWORD HOMING_DECELERATION[4] = { 5000,5000,5000,5000 };//Home按钮回初始减速度

	//CURRENT
	short CURRENT[4] = { -10,20,20,20 };//进入电流模式之后的电流值



	/*位置信息 单位：个（脉冲数）*/
	const long REDUCTION[4] = { 256 * 4096 / 360 ,64 * 4096 / 360 ,10 * 4096 ,10 * 4096 };//减速比
	//const double POS_CUR_COEF[4] = { 0.00583, 0.00146, 0.00121, 0.000996 };//将位置差转化为电流的系数//TODO:将这个方案作为改进措施


public:
	void DisableOneDevice();
	bool IsZeroPosition();
	bool OpenDevice(char *my_DeviceName,char *my_ProtocolStackName,char *my_InterfaceName,char *my_PortName);//程序开机检测
	void InitialReset();//上电复位
	bool GetMotorInfo(int nodeID); //获取位置实际值、速度实际平均值和电流实际平均值
	bool SinMotPntToPnt(WORD nodeID);//单电机点对点位置运动模式
	void SinMotWithCur(WORD nodeID);//单电机电流模式 先是位置模式，当电流值大于阈值30ms后，进入电流模式
	void SinMotOnCur(WORD nodeID);//单电机电流模式 一直是电流模式运行
	bool SingleMotorHalt(WORD nodeID);//单电机急停
	bool SingleMotorHoming(WORD nodeID);//先回home位置，再回0
	bool DisableAllDevices();//四个电机全部停止（使不能）
	void AllMotorsHalt();//设置四个电机进入急停状态，然后四个电机全部Disable
	bool AllMotorsHoming();//3、4homing,再1、2homing
	bool IsHomed();//判断四个电机是否都回0
	long DisplayToPulse(WORD node, long display);//将输入数据转换成脉冲，1,2为输出轴转动角度，3,4为圈数
	double PulseToDisplay(WORD node, double pulse);//将电机脉冲数转换为电机1，2的转动角度或者电机3，4的转动圈数（未用）
	bool CurrentJudge(WORD node);//用于判断电流平均值是否超过模式切换阈值，是否切换为电流模式
	long GetPositon(WORD nodeID);

	
};