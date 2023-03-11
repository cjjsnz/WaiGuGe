#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
#include<cstdlib>
#include<cstring>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<semaphore.h>
#include<random>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<arpa/inet.h>
#include<pthread.h>
#include "ADS1263.h"
#include "EMGFilters.h"
#include"kalman_Filter.h"
#include<iostream>
using namespace std;

#define MAXLINE 128
#define SERV_PORT 8001
#define frameToSendLen (4+4*10+4)
#define REF 5.08 // Modify according to actual voltage
				 // external AVDD and AVSS(Default), or internal 2.5V


void server_test();
void* readPthread(void *arg);
void server_init();
int flag_client();
void* working(void* arg);
//共享内存大小
#define SHM_SIZE 10

//共享内存结构体
struct shared_data{
    std::mutex mutex;
    float data[SHM_SIZE];
};

//信号量
sem_t sem;

//共享内存指针
void* shm_ptr=nullptr;

//主线程函数
void mainThread(){
    //创建共享内存
    int shm_fd=shm_open("/shm",O_RDWR|O_CREAT,0666);
    ftruncate(shm_fd,sizeof(shared_data));
    shm_ptr=mmap(NULL,sizeof(shared_data),PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
    close(shm_fd);

    shared_data*pSharedData=static_cast<shared_data*>(shm_ptr);
    float count[10];

    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex>lock(pSharedData->mutex);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<>dis(1,100);
        for(int i=0;i<10;i++)
        {
            count[i]=(float)dis(gen);

        }
           for(int i=0;i<10;i++)
         {
            pSharedData->data[i]=count[i];
			cout<< pSharedData->data[i]<<endl;
          }
        //snprintf(pSharedData->data, SHM_SIZE,"Data from sub thread:%d",count);
        sem_post(&sem);//发送信号量
    }
   
}

void subThread()
{
	/*
    int shm_fd=shm_open("/shm",O_RDWR,0666);
    shm_ptr=mmap(NULL,sizeof(shared_data),PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
    close(shm_fd);
    shared_data*pSharedData=static_cast<shared_data*>(shm_ptr);
     //等待信号量
    
    while(true){
        sem_wait(&sem);
        std::lock_guard<std::mutex>lock(pSharedData->mutex);
        std::cout<<"Received data:"<<pSharedData->data[1]<<std::endl;
    }
   
    */
    server_test();
}

int main()
{
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


//信息结构体
struct SockInfo
{
	struct sockaddr_in addr;
	int fd;
};
struct SockInfo infos[512];

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
    void* readPthread(void *arg)
{
	// 设置自动回收
	pthread_detach(pthread_self());
	int socketfd = (int)arg;
	char buf[MAXLINE];
	while(1){
		// 读取监听端发送；过来的数据
		//参数：监听端socketfd， 数据指针，数据字节数
		int n = read(socketfd, buf, MAXLINE);

		
		if(n == 0){
			printf("disconnect...thread\n");
			break;
		}else{
			printf("%s\n", buf);
		}
	}
	close(socketfd);
	return (void *)0;
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
		
	/*
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
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
	UDOUBLE ADC[10];
	float data = 0, dataAfterFilter = 0;
	EMGFilters myFilter[10];
	while(1)
	{
		addr_emg = &emgdata[0];
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
					temp=(unsigned char*)(&(emgdata[i]));
					rm_send_data.frameToSend[4+i*4+0]=*temp;
					rm_send_data.frameToSend[4+i*4+1]=*(temp+1);
					rm_send_data.frameToSend[4+i*4+2]=*(temp+2);
					rm_send_data.frameToSend[4+i*4+3]=*(temp+3);
				}

	write(socketfd, &rm_send_data.frameToSend, frameToSendLen);
	}*/
}

void server_test()
{
	server_init();
}
