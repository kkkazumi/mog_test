#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "adcread.h"

void mog_adc::set_adc(){
  if(!bcm2835_init()) printf("end");
         
  bcm2835_spi_begin();
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);         
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128); 
  bcm2835_spi_chipSelect(BCM2835_SPI_CS1);                 
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW); 
}

void mog_adc::read_val(char* out_ch0, char* ch0_data, int val){
	bcm2835_spi_transfernb(out_ch0, ch0_data, val);
}

float mog_adc::get_volt(char* out_ch0, char* ch0_data){
	unsigned int val;
	float volt_value;
	bcm2835_spi_transfernb(out_ch0, ch0_data, 3);
	val	=	(ch0_data[1]	&	0b00001111)<<8;
	val	+=	ch0_data[2];
	volt_value	=	3.3*(float)val/4095.0;
	//printf("voltage = %f\n",(float)volt_value);
	return volt_value;
}
