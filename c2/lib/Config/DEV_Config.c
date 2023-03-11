/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-21
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of theex Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "DEV_Config.h"
#include <fcntl.h>

/**
 * GPIO
**/
int DEV_RST_PIN;
int DEV_CS_PIN;
int DEV_DRDY_PIN;

/**
 * GPIO read and write
**/
void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
#ifdef RPI
#ifdef USE_BCM2835_LIB
	bcm2835_gpio_write(Pin, Value);
#endif
#endif

}

UBYTE DEV_Digital_Read(UWORD Pin)
{
	UBYTE Read_value = 0;
#ifdef RPI
#ifdef USE_BCM2835_LIB
	Read_value = bcm2835_gpio_lev(Pin);
#endif
#endif
	return Read_value;
}

/**
 * SPI
**/
UBYTE DEV_SPI_WriteByte(uint8_t Value)
{
	UBYTE temp = 0;
	// printf("write %x \r\n", Value);
#ifdef RPI
#ifdef USE_BCM2835_LIB
	temp = bcm2835_spi_transfer(Value);
#endif
#endif

	// printf("Read %x \r\n", temp);
	return temp;
}

UBYTE DEV_SPI_ReadByte(void)
{
	return DEV_SPI_WriteByte(0x00);
}

/**
 * GPIO Mode
**/
void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{
#ifdef RPI
#ifdef USE_BCM2835_LIB
	if(Mode == 0 || Mode == BCM2835_GPIO_FSEL_INPT) {
		bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_INPT);
	} else {
		bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_OUTP);
	}

#endif
#endif

}

/**
 * delay x ms
**/
void DEV_Delay_ms(UDOUBLE xms)
{
#ifdef RPI
#ifdef USE_BCM2835_LIB
	bcm2835_delay(xms);
#endif
#endif


}

static int DEV_Equipment_Testing(void)
{
	int i;
	int fd;
	char value_str[20];
	fd = open("/etc/issue", O_RDONLY);
    printf("Current environment: ");
	while(1) {
		if (fd < 0) {
			Debug( "Read failed Pin\n");
			return -1;
		}
		for(i=0;; i++) {
			if (read(fd, &value_str[i], 1) < 0) {
				Debug( "failed to read value!\n");
				return -1;
			}
			if(value_str[i] ==32) {
				printf("\r\n");
				break;
			}
			printf("%c",value_str[i]);
		}
		break;
	}
#ifdef RPI
	if(i<5) {
		printf("Unrecognizable\r\n");
	} else {
		char RPI_System[10]   = {"Raspbian"};
		for(i=0; i<6; i++) {
			if(RPI_System[i]!= value_str[i]) {
				printf("Please make JETSON !!!!!!!!!!\r\n");
				return -1;
			}
		}
	}
#endif

	return 0;
}

void DEV_GPIO_Init(void)
{
#ifdef RPI
	DEV_RST_PIN     = 18;
	DEV_CS_PIN      = 22;
	DEV_DRDY_PIN    = 17;

	DEV_GPIO_Mode(DEV_RST_PIN, 1);
	DEV_GPIO_Mode(DEV_CS_PIN, 1);
	
	DEV_GPIO_Mode(DEV_DRDY_PIN, 0);
	
	DEV_Digital_Write(DEV_CS_PIN, 1);
#endif
}

/******************************************************************************
function:	Module Initialize, the library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV_Module_Init(void)
{
    printf("/***********************************/ \r\n");
	if(DEV_Equipment_Testing() < 0) {
		return 1;
	}
#ifdef RPI
#ifdef USE_BCM2835_LIB
	if(!bcm2835_init()) {
		printf("bcm2835 init failed  !!! \r\n");
		return 1;
	} else {
		printf("bcm2835 init success !!! \r\n");
	}

	// GPIO Config
	DEV_GPIO_Init();

	bcm2835_spi_begin();                                         //Start spi interface, set spi pin for the reuse function
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //High first transmission
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                  //spi mode 1, '0, 1'
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);  //Frequency

#endif




#endif
    printf("/***********************************/ \r\n");
	return 0;
}

/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV_Module_Exit(void)
{
#ifdef RPI
#ifdef USE_BCM2835_LIB
	DEV_Digital_Write(DEV_RST_PIN, LOW);
	DEV_Digital_Write(DEV_CS_PIN, LOW);

	bcm2835_spi_end();
	bcm2835_close();



#endif
#endif
}
