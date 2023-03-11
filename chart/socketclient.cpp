#ifndef _SOCKET_SERVICE_
#define _SOCKET_SERVICE_
#include"socketclient.h"
#include<iostream>
//int socketservice_getdata(int argc, char *argv[])

/*void socketclient::getdata()
{
	char buf[MAXLINE];
	// 1. 创建 socket（参数：协议族ipv4, socket连接类型， 协议类型）
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd==-1)
	{
		perror("socket");
		exit(0);
	}
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
		exit(0);
	}
	while(1){
	//for(int j=0;j<10;j++){
		// 读取数据|参数：发送端socketfd， 数据指针，数据字节数
		int nn = read(socketfd, buf, buflen);
		if(nn == 0){
			printf("client:%d disconnect...\n", socketfd);
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
					emgdata[i]=*(reinterpret_cast<float *>(temp));
					printf("OP%d is %lf \r\n", i, emgdata[i]);
				}
				int z=1;
				series->append(QPointF(z, emgdata[0]));
				z++;
				if(z>100)
				{
					QChart* chart = ui->chartView->chart();     // 获取QchartView内置的chart
					chart->addSeries(series);                   // 将创建好的折线图series添加进chart中
					chart->createDefaultAxes();                 // 基于已添加到图表中的series为图表创建轴。以前添加到图表中的任何轴都将被删除。
					ui->chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
					ui->chartView->setRubberBand(QChartView::NoRubberBand);  //矩形缩放
					z=1;
					series->clear();
				}
				printf("\33[10A");
			}
			// 发送数据|参数：发送端socketfd， 数据指针，数据字节数
			//write(clientfd, sendbuf, strlen(sendbuf));
		}
		//data_Channel_1[j]=emgdata[0];
	//}
	}
	// 8. 关闭连接
	close(socketfd);
}*/
#endif// _SOCKET_SERVICE_
