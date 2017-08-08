#include "avr/io.h"
#include "BSP.h"
#include "common.h"
#include "util/delay.h"
#include <avr/interrupt.h>

const uint8_t testBitmaps[64] =
{
	0x00, 0x00, 0x00, 0x00, //                                 
	0x00, 0x00, 0x00, 0x00, //                                 
	0x00, 0x00, 0x00, 0x00, //                                 
	0x7E, 0xD7, 0xD9, 0x80, //  ###### ## # ##### ##  ##       
	0x7E, 0xD6, 0x5B, 0x00, //  ###### ## # ##  # ## ##        
	0x60, 0xD6, 0x1E, 0x00, //  ##     ## # ##    ####         
	0x60, 0xD6, 0x1C, 0x00, //  ##     ## # ##    ###          
	0x60, 0xD6, 0x18, 0x00, //  ##     ## # ##    ##           
	0x78, 0xD6, 0x1C, 0x00, //  ####   ## # ##    ###          
	0x60, 0xD6, 0x1E, 0x00, //  ##     ## # ##    ####         
	0x60, 0xD6, 0xDF, 0x00, //  ##     ## # ## ## #####        
	0x60, 0xF7, 0xDB, 0x80, //  ##     #### ##### ## ###       
	0x60, 0xF7, 0xD9, 0xC0, //  ##     #### ##### ##  ###      
	0x00, 0x00, 0x00, 0x00, //                                 
	0x00, 0x00, 0x00, 0x00, //                                 
	0x00, 0x00, 0x00, 0x00, //                                 
};

int8_t isrMode=0,muxLine=0;

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = 0x51; //(1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

void mux_select(char a)
{
	switch(a){
		case 0:
			PORTCLR(PORTE,BIT7);
			PORTCLR(PORTB,BIT4);
			break;
		case 1:
			PORTSET(PORTE,BIT7);
			PORTCLR(PORTB,BIT4);
			break;
		case 2:
			PORTCLR(PORTE,BIT7);
			PORTSET(PORTB,BIT4);
			break;
		case 3:
			PORTSET(PORTE,BIT7);
			PORTSET(PORTB,BIT4);
			break;
		default:
			PORTCLR(PORTE,BIT7);
			PORTCLR(PORTB,BIT4);
			break;	
	}
}


void timer0Init(void)
{
    // enable timer overflow interrupt for both Timer0
    TIMSK|=(1<<TOIE0);

   	// set timer0 counter initial value to 0
    TCNT0=0xb7;

	// start timer0 with /128 prescaler
    TCCR0 = (1<<CS02) | (1<<CS00);   

    // enable interrupts

    sei();
}

ISR(TIMER0_OVF_vect) 
{
	int8_t j,k;
	// stop timer0
	TCCR0 &= ~((1<<CS02) | (1<<CS00));
	if(isrMode)
	{
		isrMode=0;
		for(j=0;j<4;j++)
			for(k=3;k>=0;k--)
				SPI_MasterTransmit(~bitMapBuffer[((4*k+muxLine)*4)+j]);		
		mux_select(muxLine);
		muxLine++;
		if(muxLine>3)
			muxLine=0;
		PORTSET(PORTG,BIT3);
  		TCNT0=0xfd;
	}
	else
	{
		isrMode=1;
		PORTCLR(PORTG,BIT3);
		TCNT0=0xb7;
	}

	// start timer0 with /128 prescaler
    TCCR0 = (1<<CS02) | (1<<CS00); 
}
