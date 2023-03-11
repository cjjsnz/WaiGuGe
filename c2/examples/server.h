#ifndef _SERVER_H
#define _SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<arpa/inet.h>
#include<pthread.h>
#include "ADS1263.h"
#include "EMGFilters.h"
#include"kalman_Filter.h"
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

#endif