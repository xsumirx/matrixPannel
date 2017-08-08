#ifndef _COMMON_H_
#define _COMMON_H_

#define F_CPU 18432000UL

/* Macro Definations */

#define BIT0	0x01
#define BIT1	0x02
#define BIT2	0x04
#define BIT3	0x08
#define BIT4	0x10
#define BIT5	0x20
#define BIT6	0x40
#define BIT7	0x80

#define PORTSET(x,y)	x|=y
#define PORTCLR(x,y)	x&=~y

#define DDR_SPI		PORTB
#define DD_MOSI		2
#define DD_SCK		1


#define NUMBER_OF_PANELS	1


#define VERTICAL_COLUMNS	4*NUMBER_OF_PANELS
#define BUFFER_SIZE			64*NUMBER_OF_PANELS

/* Global Variables */

uint8_t bitMapBuffer[BUFFER_SIZE];

#endif
