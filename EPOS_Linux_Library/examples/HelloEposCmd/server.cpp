#include<server.h>
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
//UDP 设置端口号和IP地址
#define MYPORT 8080
const char* SERVERIP = "127.0.0.1";
typedef unsigned char uchar;
/*float bytesToFloat(uchar b0, uchar b1,uchar b2,uchar b3)
{
    uchar byte_array[]={b3,b2,b1,b0};
    float output;
    std::copy(reinterpret_cast<const char*>(&byte_array[0]),
              reinterpret_cast<const_char*>(&byte_array[4]),
              reinterpret_cast<char*>(&result));
    return output;
}*/
int emgdata::getemgdata(){
//int main(){

    //同一台电脑测试，需要两个端口
    int sockfd;
	// 创建socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1==sockfd){
        return false;
        puts("Failed to create socket");
    }

    // 设置地址与端口
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;       // Use IPV4
    addr.sin_port = htons(MYPORT);    //
    addr.sin_addr.s_addr = inet_addr(SERVERIP);
    
   // Time out
  
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 200000;  // 200 ms
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));

    // Bind 端口，用来接受之前设定的地址与端口发来的信息,作为接受一方必须bind端口，并且端口号与发送方一致
    if (bind(sockfd, (struct sockaddr*)&addr, addr_len) == -1){
        printf("Failed to bind socket on port %d\n", *SERVERIP);
        close(sockfd);
        return false;
    }
   int ASCItranslate(char c);

   unsigned char buffer[256];
    memset(buffer, 0, 256);
    char emgdata[256];
    int counter = 0;
    char floatBytesBuffer[4];

    while(1){
        struct sockaddr_in src;
        socklen_t src_len = sizeof(src);
        memset(&src, 0, sizeof(src));
	
     // 阻塞住接受消息
        int sz = recvfrom(sockfd, buffer, 256, 0, (sockaddr*)&src, &src_len);
        
        if (sz > 0){
            buffer[sz]=0;   
            printf("Get Message %d: %s\nIn int format:", counter++, buffer);

            for(int i=0;i<sz;i++)
            {
                printf("%u ",buffer[i]);
            }
            printf("\n");

          
            for(int i=0;i<4;i++)
            {
                floatBytesBuffer[3]=buffer[2+4*i];
                floatBytesBuffer[2]=buffer[2+4*i+1];
                floatBytesBuffer[1]=buffer[2+4*i+2];
                floatBytesBuffer[0]=buffer[2+4*i+3];

                float* temp;
                temp= (float*)floatBytesBuffer;
               
                emgData4Chn[i]=*temp;
            }
            float emg1Chn=emgData4Chn[0];
            float emg2Chn=emgData4Chn[1];
            float emg3Chn=emgData4Chn[2];
            float emg4Chn=emgData4Chn[3];
            printf("reinterpreted float is %f, %f, %f, %f\n",emgData4Chn[0],emgData4Chn[1],emgData4Chn[2],emgData4Chn[3]);
           
        }
        else{
            puts("timeout");
        }
    }

    close(sockfd);
    return 0;
}

