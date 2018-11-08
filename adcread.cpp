#include <bcm2835.h>
#include <stdio.h>

void get_dist(char* out_ch0, char* ch0_data){
	bcm2835_spi_transfernb(out_ch0, ch0_data, 3);
	printf("CH0:    %02X %02X %02X\n", ch0_data[0], ch0_data[1], ch0_data[2]);
}

void get_volt(char* out_ch0, char* ch0_data){
	unsigned int val;
	float volt_value;
	bcm2835_spi_transfernb(out_ch0, ch0_data, 3);
	val	=	(ch0_data[1]	&	0b00001111)<<8;
	val	+=	ch0_data[2];
	volt_value	=	3.3*(float)val/4095.0;
	printf("voltage = %f\n",volt_value);
}

/*
unsigned int mcp_read(int pi_channel,int adc_channel){
	unsigned	int	val	=	0;
	unsigned	char	data[3]	=	{};

	data[0]	=	0b00000110|(((pi_channel&0x04))>>2));
	data[1]	=	0b00000000|(((pi_channel&0x03))<<6));
	data[2]	=	0;
	wiringPiSPIDataRW(pi_channel,data,sizeof(data));

	val	=	(data[1]<<8)&0b111100000000;
	val+=(data[2]&0xff);

	return val;
}
*/

int main(int arc, char **argv) 
{
 
  char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
  char ch0_data[] = { 0x00, 0x00, 0x00 };
  char out_ch1[] = { 0b00000110, 0b01000000, 0b00000000 };
  char ch1_data[] = { 0x00, 0x00, 0x00 };
  char out_ch2[] = { 0b00000110, 0b10000000, 0b00000000 };
  char ch2_data[] = { 0x00, 0x00, 0x00 }; 
  
  if(!bcm2835_init()) return 1;
         
  bcm2835_spi_begin();
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);         
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128); 
  bcm2835_spi_chipSelect(BCM2835_SPI_CS1);                 
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW); 

  while(1){

      bcm2835_spi_transfernb(out_ch0, ch0_data, 3);
      //printf("CH0:    %02X %02X %02X\n", ch0_data[0], ch0_data[1], ch0_data[2]);
			//get_dist(out_ch0,ch0_data);
			get_volt(out_ch0,ch0_data);
      bcm2835_spi_transfernb(out_ch1, ch1_data, 3);
      //printf("CH1:    %02X %02X %02X\n", ch1_data[0], ch1_data[1], ch1_data[2]);
      bcm2835_spi_transfernb(out_ch2, ch2_data, 3);
      //printf("CH2:    %02X %02X %02X\n", ch2_data[0], ch2_data[1], ch2_data[2]);
  }
     
  bcm2835_spi_end();
  bcm2835_close();
}
