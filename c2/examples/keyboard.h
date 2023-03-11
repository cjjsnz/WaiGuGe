#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<linux/input.h>
#include<termios.h>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<signal.h>


#include"Motor.h"
#include"MotorControl.h"
#include"Definitions.h"
#include"initial.h"

static int tty_fd=-1;
static struct termios save_tty,save_stdin,nt;

 
int read_keyboard();