	#include "server.h"
	#include<iostream>
	using namespace std;

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
		char ip[24]={0};
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
	
	struct SockInfo* pinfo=(struct SockInfo*)arg;
	char ip[32];
	printf("客户端的IP地址：%s,端口：%d\n",inet_ntop(AF_INET,&pinfo->addr.sin_addr.s_addr,ip,sizeof(ip)),ntohs(pinfo->addr.sin_port));
	while(1)
	{
		char buff[1024];
		int len=recv(pinfo->fd,buff,sizeof(buff),0);
		if(len>0)
		{
			printf("client say:%s\n",buff);
			send(pinfo->fd,buff,len,0);
		}
		else if(len==0)
		{
			printf("客户端已经断开了连接...\n");
			break;
		}
		else{
			perror("recv");
			break;
		}
	
	}
		close(pinfo->fd);
		pinfo->fd=-1;

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
	int i=1;
	while(1)
	{
		cout<<"test "<<i<<" time"<<endl;
		sleep(1);
		i++;
		if(i>=1000)
			break;
	}
	
}