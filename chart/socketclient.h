#ifndef _SOCKET_SERVICE_H
#define _SOCKET_SERVICE_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <QLineSeries>
#include <QWidget>
#include "widget.h"
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"
#include <stdlib.h> //exit()
#include <signal.h> //signal()
#include <time.h>
#include <string.h>
#define MAXLINE 128
#define SERV_PORT 8001
#define buflen 48

class socketclient
{
private:
    /* data */

public:
    float data_Channel_1[10];
    float emgdata[10];
    unsigned char temp[4];
public:    
    //void getdata();
    
};

#endif// _SOCKET_SERVICE_