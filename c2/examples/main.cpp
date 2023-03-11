#include<stdio.h>
#include <stdlib.h> //exit()
#include <signal.h> //signal()
#include <time.h>
#include <stdio.h>
#include <string.h>
#include<sys/select.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdbool.h>
#include<pthread.h>
#include<iostream>
#include"EMGData.h"
#include"initial.h"
#include"Motor.h"
#include"MotorControl.h"
#include"Definitions.h"
#include <unistd.h>
#include<thread>
#include"keyboard.h"
#include"server.h"

#include<mutex>
#include<chrono>
#include<cstdlib>
#include<cstring>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<semaphore.h>
#include<random>
//共享内存大小
#define SHM_SIZE 10
#define MAXLINE 128
#define SERV_PORT 8001
#define frameToSendLen (4+4*10+4)
#define REF 5.08 // Modify according to actual voltage
				 // external AVDD and AVSS(Default), or internal 2.5V

//共享内存结构体
struct shared_data{
    std::mutex mutex;
    float data[SHM_SIZE];
};
//信息结构体
struct SockInfo
{
	struct sockaddr_in addr;
	int fd;
};
struct SockInfo infos[512];


//信号量
sem_t sem;

//共享内存指针
void* shm_ptr=nullptr;

// ADC1 test part
#define TEST_ADC1 1
// ADC1 rate test par
#define TEST_ADC1_RATE 0
// ADC2 test part
#define TEST_ADC2 0
// RTD test part
#define TEST_RTD 0
#define ARR_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define _DEBUG      0
/********************************************************************/
typedef void* HANDLE;
typedef int BOOL;
using namespace std;
initialdefination I1;
initialdefination I2;
unsigned short MyNodeId=1;
void* g_pKeyHandle = 0;
unsigned short g_usNodeId = 1;
string g_deviceName;
string g_protocolStackName;
string g_interfaceName;
string g_portName;
int g_baudrate = 0;
CMotionControl m_motionControl;
enum EAppMode
{
    AM_UNKNOWN,
    AM_DEMO,
    AM_INTERFACE_LIST,
    AM_PROTOCOL_LIST,
    AM_VERSION_INFO
};
enum state{getdata,init,dataprocess,motioncontrol};
state m_state=getdata;
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
void Handler(int signo)
{
	// System Exit
	printf("\r\n END \r\n");
	DEV_Module_Exit();
	exit(0);
}
void mainThread();
void subThread();
void thread_function();
void server_init();
void* working(void* arg);
int main()
{
	
	std::thread threadObj(thread_function);
	threadObj.join();
	//初始化信号量
    sem_init(&sem,0,0);
    //创建子线程
    std::thread t(subThread);
	t.detach();
    //运行主线程
    mainThread();
    //等待子线程结束
    //销毁信号量和共享内存
    sem_destroy(&sem);
    munmap(shm_ptr,sizeof(shared_data));
    shm_unlink("/shm");
    return 0;
}
void thread_function()
{
	for(int i=0;i<2;i++)
	std::cout<<"thread function excuting"<<std::endl;
	read_keyboard();
}
void mainThread()
{
    //创建共享内存
    int shm_fd=shm_open("/shm",O_RDWR|O_CREAT,0666);
    ftruncate(shm_fd,sizeof(shared_data));
    shm_ptr=mmap(NULL,sizeof(shared_data),PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
    close(shm_fd);

    shared_data*pSharedData=static_cast<shared_data*>(shm_ptr);
    int count[10];

	CEMGData m_seEMGData;
	//肌电信号采集
	UWORD i;
	double RES, TEMP;
	int var=10;
	int *p;
	p= &var;
	// Exception handling:ctrl + c
	signal(SIGINT, Handler);
	printf("ADS1263 Demo \r\n");
	DEV_Module_Init();
	ADS1263_SetMode(0);
	if (ADS1263_init_ADC1(ADS1263_38400SPS) == 1)
	{
		printf("\r\n END \r\n");
		DEV_Module_Exit();
		exit(0);
	}
	/* Test DAC */
	// ADS1263_DAC(ADS1263_DAC_VLOT_3, Positive_A6, Open);
	// ADS1263_DAC(ADS1263_DAC_VLOT_2, Negative_A7, Open);
	/***********************肌电信号滤波初始化*/
	
	typedef struct
		{
		uint8_t index;
		uint16_t buf[64]; /* Buffer for rectified AC value */
		uint32_t sum;     /* Sum for fast caculation of mean value */
		} CycleBuf_t;
	// Append to cycle buffer
	#define CYCLE_BUF_ADD(cb, val)                    \
	{                                               \
		cb.sum -= cb.buf[cb.index];                   \
		cb.sum += (val);                              \
		cb.buf[cb.index] = (val);                     \
		cb.index = (cb.index + 1) % ARR_SIZE(cb.buf); \
	}
	/* Get mean value of cycle buffer */
	#define CYCLE_BUF_MEAN(cb) (cb.sum / ARR_SIZE(cb.buf))
	CycleBuf_t rectifiedAcBuf[10];//设定10通道数据
	SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;
	unsigned long long interval = 1000000ul / sampleRate;
	NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;

	//initialization
	EMGFilters myFilter[10];
 	for (int i = 0; i < 10; i++) {
    myFilter[i].init(sampleRate, humFreq, true, true, true);
    rectifiedAcBuf[i].sum = 0;
    rectifiedAcBuf[i].index = 0;
    for (int j = 0; j < ARR_SIZE(rectifiedAcBuf[i].buf); j++)
    {
      rectifiedAcBuf[i].buf[j] = 0;
    }
	}
	/*****************************************************************************************///eposcontrol
	/**********************卡尔曼滤波初始化*/
	const double W0[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	/*******************************************/
	float envelope[10]={0,0,0,0,0,0,0,0,0,0};
	/*********************************/
	/*****************************************************************************************///eposcontrol
	int lResult = MMC_FAILED;
	unsigned int ulErrorCode = 0;
	g_deviceName = "EPOS2"; 
	g_protocolStackName = "MAXON SERIAL V2"; 
	g_interfaceName = "USB"; 
	g_portName = "USB0"; 
	g_baudrate = 1000000; 
	char* pDeviceName = new char[255];
	char* pProtocolStackName = new char[255];
	char* pInterfaceName = new char[255];
	char* pPortName = new char[255];
	strcpy(pDeviceName, g_deviceName.c_str());
	strcpy(pProtocolStackName, g_protocolStackName.c_str());
	strcpy(pInterfaceName, g_interfaceName.c_str());
	strcpy(pPortName, g_portName.c_str());
	//I1.LogInfo("Open device...");
	m_motionControl.OpenDevice(pDeviceName, pProtocolStackName, pInterfaceName, pPortName);
	m_motionControl.OpenDevice(pDeviceName, pProtocolStackName, pInterfaceName, pPortName);
	int CURRENT[2]={300,300};
	/**********************************************************************************************打开电机1,2*/
	//int j=0;
	double independent_raw_data[4]={0,0,0,0};

	/*循环初始化*/
	UDOUBLE ADC[10];
	float data = 0, dataAfterFilter = 0;
	float* addr_emg;
	unsigned char* temp;
	//m_seEMGData.emgdata[10];
	struct robot_status
	{
		unsigned char programTag;
		float emgdata_from_ADC[10];
		unsigned char frameToSend[frameToSendLen];
		unsigned int current_send_index;
		unsigned char sendTag;
	}rm_send_data;
	//大循环开始
	while(true)
	{
					//usleep(10000);
				ADS1263_GetAll(ADC); // Get `ADC1 value
				for (i = 0; i < 10; i++)
					{
						if ((ADC[i] >> 31) == 1)
						{	m_seEMGData.emgdata[i]=-REF * 2 + ADC[i] / 2147483648.0 * REF;
							//printf("IN%d is %lf \r\n", 0, emgdata[0]); // 7fffffff + 1
							}
						else
						{	m_seEMGData.emgdata[i]= ADC[i] / 2147483647.0 * REF;
							//printf("IN%d is %lf \r\n",0, emgdata[0]); // 7fffffff
							}
						/*****************************肌电信号滤波*/		
						data = m_seEMGData.emgdata[i]*100;//第一次肌电信号增幅
						dataAfterFilter = myFilter[i].update(data);
						// Rectification
						//CYCLE_BUF_ADD(rectifiedAcBuf[i], abs(dataAfterFilter));
						// Simple envelope calculation, use 2 * rectified value
						//envelope[i] = CYCLE_BUF_MEAN(rectifiedAcBuf[i]) * 2;
						//emgdata[i]=envelope[i];//第二次重新赋值
						m_seEMGData.emgdata[i]=dataAfterFilter;//第二次重新赋值
					}
				/***********************************************************************************************************/
				//printf("IN%d is %lf \r\n", 1, emgdata[1]);
					/**************************************/
					//printf("\33[10A"); // Move the curcharsor up
					//得到emgdata[]进行socket数据发送工作，emgdata此时为一个数组可以进行数组首地址发送
					/**************************进行卡尔曼滤波*/
					//输入信号转换为电机电流
					for(int i=0;i<4;i++)
					{
						independent_raw_data[i]=W0[i][0]*m_seEMGData.emgdata[0]+W0[i][1]*m_seEMGData.emgdata[1]+W0[i][2]*m_seEMGData.emgdata[2]+W0[i][3]*m_seEMGData.emgdata[3];
						independent_raw_data[i]=abs(independent_raw_data[i]);
					}
					m_seEMGData.GetRawDataFilter(independent_raw_data);
					for(int i=0;i<4;i++)
					{	m_seEMGData.m_dEMGDataAve[i]=m_seEMGData.m_dEMGDataAvePre[i];
						m_seEMGData.emgdata[i]=m_seEMGData.m_dEMGDataAve[i];
					}	
					/***********************给到电机的阈值控制代码*/
					float targetvelocity[2] = {250+10*m_seEMGData.emgdata[0],250+10*m_seEMGData.emgdata[1]};
					for(int i=0;i<2;i++)
					{
						if(m_seEMGData.emgdata[i]>15&&m_seEMGData.emgdata[i]<35)
						{
							CURRENT[i]=targetvelocity[i];
							m_motionControl.m_motor[i].UpdateProfileData(i,m_motionControl.BACK_POSITION[i],m_motionControl.BACK_VELOCITY[i],
								m_motionControl.BACK_ACCELERATION[i],m_motionControl.BACK_DECELERATION[i],true,CURRENT[i]);
							m_motionControl.SinMotOnCur(i+1);
							m_motionControl.GetMotorInfo(i+1);
							m_seEMGData.emgdata[i+4]=(float)m_motionControl.m_motor[i].m_sActualCur;		
							m_seEMGData.emgdata[i+6]=(float)m_motionControl.m_motor[i].m_lActualPos;
						}
						else if (m_seEMGData.emgdata[i]>35)
							{					
								m_motionControl.SingleMotorHalt(1+i);
								m_motionControl.GetMotorInfo(i+1);
								m_seEMGData.emgdata[i+4]=(float)m_motionControl.m_motor[i].m_sActualCur;	
								m_seEMGData.emgdata[i+6]=(float)m_motionControl.m_motor[i].m_lActualPos;
							}
						else
							{				
								m_motionControl.SingleMotorHalt(i+1);
								m_motionControl.GetMotorInfo(i+1);
								m_seEMGData.emgdata[i+4]=(float)m_motionControl.m_motor[i].m_sActualCur;	
								m_seEMGData.emgdata[i+6]=(float)m_motionControl.m_motor[i].m_lActualPos;
							}
					}
				DWORD FORWARD_VELOCITY[4]={1800,1800,1800,1800};
				const long GRASP_POSITION[4]={0,0,0,0};
				/**************行程限位模块代码******************/
				for(int i=0;i<2;i++)
				{
					if(m_seEMGData.emgdata[i+6]>=1000000)
					{
						m_motionControl.m_motor[i].UpdateProfileData(i,GRASP_POSITION[i],FORWARD_VELOCITY[i],m_motionControl.FORWARD_ACCELERATION[i]
							,m_motionControl.FORWARD_DECELERATION[i],true,m_motionControl.FORWARD_CURRENT[i]);
						while(m_seEMGData.emgdata[i+6]!=0)
						{
							m_motionControl.SinMotPntToPnt(i+1);
							m_motionControl.GetMotorInfo(i+1);
							m_seEMGData.emgdata[i+6]=(float)m_motionControl.m_motor[i].m_lActualPos;
							//cout<<m_seEMGData.emgdata[i+6]<<endl;
							if(m_seEMGData.emgdata[i+6]==0)
								break;
							addr_emg = &m_seEMGData.emgdata[0];
						}
						/*if(m_motionControl.SinMotPntToPnt(i+1))
							//printf("111");
						//sleep(5000);
						{
							m_motionControl.GetMotorInfo(i+1);
							emgdata[i+4]=(float)m_motionControl.m_motor[i].m_sActualCur;	
							emgdata[i+6]=(float)m_motionControl.m_motor[i].m_lActualPos;
						}*/
					}
					else
					{
						m_motionControl.GetMotorInfo(i+1);
						m_seEMGData.emgdata[i+4]=(float)m_motionControl.m_motor[i].m_sActualCur;	
						m_seEMGData.emgdata[i+6]=(float)m_motionControl.m_motor[i].m_lActualPos;
					}
				}

				std::this_thread::sleep_for(std::chrono::seconds(1));			
				std::lock_guard<std::mutex>lock(pSharedData->mutex);
				for(int i=0;i<10;i++)
				{
					pSharedData->data[i]=m_seEMGData.emgdata[i];
					cout<< pSharedData->data[i]<<endl;
				}
				//snprintf(pSharedData->data, SHM_SIZE,"Data from sub thread:%d",count);
				sem_post(&sem);//发送信号量
	}
}
							
void subThread()
{
   /* int shm_fd=shm_open("/shm",O_RDWR,0666);
    shm_ptr=mmap(NULL,sizeof(shared_data),PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
    close(shm_fd);
    shared_data*pSharedData=static_cast<shared_data*>(shm_ptr);
	/*开始设置服务器*/
	/*
    while(true)
	{
        sem_wait(&sem);
        std::lock_guard<std::mutex>lock(pSharedData->mutex);
        std::cout<<"通道1:"<<pSharedData->data[0]<<"通道2"<<pSharedData->data[1]<<std::endl;
	}*/
	server_init();
    
}
void server_init(){
	
    /*开始设置服务器*************************************************************************************/
	// struct SOCKADDR_IN servaddr;
	char buf[MAXLINE];
	// 1. 创建 socket（参数：协议族ipv4, socket连接类型， 协议类型）
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd==-1)
	{
		perror("socket");
		exit(0);
	}
	// 2. 创建并初始化 监听端 sockaddr(协议族， ip, port端口号)
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;  //协议族 = ipv4
	inet_pton(AF_INET, "0.0.0.0", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
	// 3. 绑定监听端sockaddr(参数：监听端 socketfd, 监听端socketaddr指针, 监听端socketaddr字节数))
	int ret=bind(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(ret==-1)
	{
		perror("bind");
		exit(0);
	}
	// 4. 设置监听个数
	int lis=listen(socketfd, 5);
	if(lis==-1)
	{
		perror("listen");
		exit(0);
	}

	cout<<"server init success"<<endl;
//	初始化数组
	int max=sizeof(infos)/sizeof(infos[0]);
	for(int i=0;i<max;i++)
	{
		bzero(&infos[i],sizeof(infos[i]));
		infos[i].fd=-1;
	}

	//struct sockaddr_in clientaddr;
	//int n = sizeof(clientaddr);
	int n=sizeof(struct sockaddr_in);
	while(1)
	{
		struct SockInfo* pinfo;
		for(int i=0;i<max;++i)
		{
			if(infos[i].fd==-1)
			{
				pinfo=&infos[i];
				break;
			}
		}
		int clientfd = accept(socketfd, (struct sockaddr *)&pinfo->addr, (socklen_t *)&n);
		pinfo->fd=clientfd;
		if(clientfd==-1)
		{ 
			perror("accept");
			break;
			//continue;
		}
		pthread_t tid;
		pthread_create(&tid,NULL,working,pinfo);
		pthread_detach(tid);
		cout<<clientfd<<endl;
        cout<<socketfd<<endl;
		//char ip[24]={0};
		//printf("客户端的IP地址：%s,端口：%d\n",inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip)),ntohs(clientaddr.sin_port));
			/*if(flag_client()!=-1)
				{
					
				}*/
		/********************************************/
		/*	struct robot_status
		{
		unsigned char programTag;
		float emgdata_from_ADC[10];
		unsigned char frameToSend[frameToSendLen];
		unsigned int current_send_index;
		unsigned char sendTag;
		}rm_send_data;
		unsigned char* temp;
		rm_send_data.frameToSend[0]='#';
		rm_send_data.frameToSend[1]='*';
		rm_send_data.frameToSend[2]='#';
		rm_send_data.frameToSend[3]='*';
		rm_send_data.frameToSend[frameToSendLen-1]='#';
		rm_send_data.frameToSend[frameToSendLen-2]='*';
		rm_send_data.frameToSend[frameToSendLen-3]='#';
		rm_send_data.frameToSend[frameToSendLen-4]='*';
		for(int i = 0; i < 10; i++)
			{			
				//printf("数组emgdata[%d]:%lf \n",i,*(addr_emg+i));
				temp=(unsigned char*)(&(m_seEMGData.emgdata[i]));
				rm_send_data.frameToSend[4+i*4+0]=*temp;
				rm_send_data.frameToSend[4+i*4+1]=*(temp+1);
				rm_send_data.frameToSend[4+i*4+2]=*(temp+2);
				rm_send_data.frameToSend[4+i*4+3]=*(temp+3);
					}
		write(socketfd, &rm_send_data.frameToSend, frameToSendLen);
		*/
	}
	close(socketfd);
}


void* working(void* arg)
{
    int shm_fd=shm_open("/shm",O_RDWR,0666);
    shm_ptr=mmap(NULL,sizeof(shared_data),PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
    close(shm_fd);
    shared_data*pSharedData=static_cast<shared_data*>(shm_ptr);
     //等待信号量
    
	struct SockInfo* pinfo=(struct SockInfo*)arg;
	char ip[32];
	printf("客户端的IP地址：%s,端口：%d\n",inet_ntop(AF_INET,&pinfo->addr.sin_addr.s_addr,ip,sizeof(ip)),ntohs(pinfo->addr.sin_port));
    cout<<"对接的端口号"<<pinfo->fd<<endl;
    float* addr_emg;
	unsigned char* temp;
	float emgdata[10];
	struct robot_status
	{
		unsigned char programTag;
		float emgdata_from_ADC[10];
		unsigned char frameToSend[frameToSendLen];
		unsigned int current_send_index;
		unsigned char sendTag;
	}rm_send_data;
    
    std::cout<<"进入循环"<<std::endl;
	while(true)
	{
        sem_wait(&sem);
        std::lock_guard<std::mutex>lock(pSharedData->mutex);
        std::cout<<"send data:"<<pSharedData->data[1]<<std::endl;
			rm_send_data.frameToSend[0]='#';
			rm_send_data.frameToSend[1]='*';
			rm_send_data.frameToSend[2]='#';
			rm_send_data.frameToSend[3]='*';
			rm_send_data.frameToSend[frameToSendLen-1]='#';
			rm_send_data.frameToSend[frameToSendLen-2]='*';
			rm_send_data.frameToSend[frameToSendLen-3]='#';
			rm_send_data.frameToSend[frameToSendLen-4]='*';
			for(int i = 0; i < 10; i++)
				{			
					//printf("数组emgdata[%d]:%lf \n",i,*(addr_emg+i));
					temp=(unsigned char*)(&(pSharedData->data[i]));
					rm_send_data.frameToSend[4+i*4+0]=*temp;
					rm_send_data.frameToSend[4+i*4+1]=*(temp+1);
					rm_send_data.frameToSend[4+i*4+2]=*(temp+2);
					rm_send_data.frameToSend[4+i*4+3]=*(temp+3);
				}
	    write(pinfo->fd, &rm_send_data.frameToSend, frameToSendLen);
	}
    /*
		close(pinfo->fd);
		pinfo->fd=-1;
    */
		return NULL;
}