#ifndef client_H
#define client_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define SERV_PORT 8001
class client{
public:
    int socketfd;
    int get_connect();
    void getsocket();
};



#endif // !client_H