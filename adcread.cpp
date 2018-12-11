#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "SPIdev.h"
#include "adcread.h"

#define START_BIT (1<<2)
#define SINGLE_ENDED (1<<1)
#define CH_SHIFT (6)

float mog_adc::get_volt(unsigned char* out_ch0, unsigned char* ch0_data, int len){
	uint8_t tx[len][3];
	uint8_t rx[len][3];

	unsigned int val;
	float volt_value;

	for(int i=0;i<len;i++){
		tx[i][0] = START_BIT | SINGLE_ENDED;
		tx[i][1] = out_ch0[i] << CH_SHIFT;
	}

	//bcm2835_spi_transfernb(out_ch0, ch0_data, 3);
	SPIdev::transfer("/dev/spidev0.1", out_ch0, ch0_data, len);
	printf("char = %s\n",ch0_data[1]);
	val	=	(ch0_data[1]	&	0b00001111)<<8;
	val	+=	ch0_data[2];
	volt_value	=	5.0*(float)val/4095.0;
	return volt_value;
}
