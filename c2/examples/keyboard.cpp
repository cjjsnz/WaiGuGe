#include "keyboard.h"



int read_keyboard(){

    int g_baudrate = 0;
    CMotionControl m_motionControl;
	int lResult = MMC_FAILED;
	unsigned int ulErrorCode = 0;
	string g_deviceName = "EPOS2"; 
	string g_protocolStackName = "MAXON SERIAL V2"; 
	string g_interfaceName = "USB"; 
	string g_portName = "USB0"; 
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
    /**/
   int tty_fd=open("/dev/tty",O_RDONLY);
   char temp;
   tcgetattr(0,&save_stdin);
   nt=save_stdin;
   nt.c_lflag&=~(ECHO|ICANON);
   tcsetattr(0,TCSANOW,&nt);



   int flags=fcntl(tty_fd,F_GETFL);
   flags|=O_NONBLOCK;
   if(fcntl(tty_fd,F_SETFL,flags)==-1){exit(1);}
    flags=fcntl(0,F_GETFL);
    if(fcntl(0,F_SETFL,flags)==-1){exit(1);}
    while(1)
    {
        sleep(1);
        read(tty_fd,&temp,1);
        switch(temp)
        {
            case 'w':
            {
                printf("you press w \n");
               
				for(int i=0;i<2;i++)
                {
                CURRENT[i]=400;
				m_motionControl.m_motor[i].UpdateProfileData(i,m_motionControl.BACK_POSITION[i],m_motionControl.BACK_VELOCITY[i],
				m_motionControl.BACK_ACCELERATION[i],m_motionControl.BACK_DECELERATION[i],true,CURRENT[i]);
			    m_motionControl.SinMotOnCur(i+1);
				m_motionControl.GetMotorInfo(i+1);
                }	
					
                break;
            }
            case 'd':
            {
                for(int i=0;i<2;i++){
                        m_motionControl.SingleMotorHalt(1+i);
                }
                break;
            }
            case 'a':
            {
				for(int i=0;i<2;i++)
                {
                    CURRENT[i]=-400;
                    m_motionControl.m_motor[i].UpdateProfileData(i,m_motionControl.BACK_POSITION[i],m_motionControl.BACK_VELOCITY[i],
                    m_motionControl.BACK_ACCELERATION[i],m_motionControl.BACK_DECELERATION[i],true,CURRENT[i]);
                    m_motionControl.SinMotOnCur(i+1);
                    m_motionControl.GetMotorInfo(i+1);
                }	
                break;
            }

            case 's':
            {
                tcsetattr(0,TCSANOW,&save_stdin);
                close(tty_fd);
                return 0;
            }
            
            default:;
        }
    }
    return 0;
}