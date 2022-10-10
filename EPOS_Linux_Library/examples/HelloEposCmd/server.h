#pragma once
#ifndef SERVER_H
#define SERVER_H
#include<sys/select.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
class emgdata{
 public:
    
    char emgdata[256];
    int counter = 0;
    float emgData4Chn[4];
    float emg1Chn=emgData4Chn[0];
    float emg2Chn=emgData4Chn[1];
    float emg3Chn=emgData4Chn[2];
    float emg4Chn=emgData4Chn[3];

 public:
    int getemgdata();

};
#endif