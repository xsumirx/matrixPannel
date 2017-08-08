/*
 * Display_code.c
 *
 * Created: 8/3/2017 7:56:41 PM
 * Author : Saurav
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "BSP.h"
#include "common.h"

extern const uint8_t testBitmaps[64];

int main(void)
{
	int i;
	DDRB=0x17;
	DDRG=0x18;
	DDRE=0x84;
	PORTSET(PORTG,BIT4);
	SPI_MasterInit();

	for(i=0;i<64;i++)
		bitMapBuffer[i]=testBitmaps[i];

	timer0Init();

    /* Replace with your application code */
    while (1) 
    {
	}
}

