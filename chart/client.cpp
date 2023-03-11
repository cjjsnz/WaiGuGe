
#include"client.h"
#include<iostream>
#include"widget.h"
#define SERV_PORT 8001
#define MAXLINE 128
#define buflen 48
int client::get_connect(){

	// 1. 创建 socket（参数：协议族ipv4, socket连接类型， 协议类型）
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd==-1)
	{
		perror("socket");
		return -1;
	}
	/*/ 2. 创建并初始化 监听端sockaddr(协议族， ip, port端口号)
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "0.0.0.0", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);*/
	// 2. 创建并初始化 客户端sockaddr(协议族， ip, port端口号)
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
	//3.连接服务器
	int ret=connect(socketfd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret==1)
	{
		perror("connect");
		return -1;
	}
        /*
        if(len>0)
        {
            printf("server say:%d\n",buff[1]);
        }
        else if(len==0)
        {
            printf("服务器已经断开了连接...\n");
			break;
        }
        else 
        {
            perror("recv");
			break;
        }*/
	
    close(socketfd);
    return 0;
}

void client::getsocket(){

    char buf[MAXLINE];
		
    	unsigned char temp[4];
		// 读取数据|参数：发送端socketfd， 数据指针，数据字节数
		int nn = read(socketfd, buf, buflen);

		if(nn == 0){
			printf("client:%d disconnect...\n",socketfd);
			//break;
		}
		else{
			//printf("%s\n", buf);
			//printf("%c",*(reinterpret_cast<unsigned char*>(&(buf[0]))));
			//printf("%c",*(reinterpret_cast<unsigned char*>(&(buf[buflen-1]))));
			if(*(reinterpret_cast<unsigned char*>(&(buf[0])))=='#'&&
			   *(reinterpret_cast<unsigned char*>(&(buf[1])))=='*'&&
			   *(reinterpret_cast<unsigned char*>(&(buf[2])))=='#'&&
			   *(reinterpret_cast<unsigned char*>(&(buf[3])))=='*'&&
			   *(reinterpret_cast<unsigned char*>(&(buf[buflen-1])))=='#'&&
			   *(reinterpret_cast<unsigned char*>(&(buf[buflen-2])))=='*'&&
			   *(reinterpret_cast<unsigned char*>(&(buf[buflen-3])))=='#'&&
			   *(reinterpret_cast<unsigned char*>(&(buf[buflen-4])))=='*')
			   {
				for(int i=0;i<10;i++){
					temp[0]=*(reinterpret_cast<unsigned char*>(&(buf[4+4*i+0])));
					temp[1]=*(reinterpret_cast<unsigned char*>(&(buf[4+4*i+1])));
					temp[2]=*(reinterpret_cast<unsigned char*>(&(buf[4+4*i+2])));
					temp[3]=*(reinterpret_cast<unsigned char*>(&(buf[4+4*i+3])));
					emgdata[i]=*(reinterpret_cast<float *>(&temp[0]));
					}
				//std::cout<<"肌电信号取值："<<emgdata[0]<<std::endl;
			}
		}

}

