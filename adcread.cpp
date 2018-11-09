#include <bcm2835.h>
#include <stdio.h>

class mog_adc
{
	public:
		void set_adc();
		float ad2dist(int ad);
		void read_val(char* out_ch0, char* ch0_data, int val);
		void get_volt(char* out_ch0, char* ch0_data);
};

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

float mog_adc::ad2dist(int ad){
	float D;
	if (ad>=250){D=999;}
	else if(ad<250){D=60;
	if(ad<247){D=50;
	if(ad<245){D=40;
	if(ad<242){D=30;
	if(ad<230){D=20;
	if(ad<200){D=10;}}}}}}
	return D;
}

void mog_adc::get_volt(char* out_ch0, char* ch0_data){
	unsigned int val;
	float volt_value;
	bcm2835_spi_transfernb(out_ch0, ch0_data, 3);
	val	=	(ch0_data[1]	&	0b00001111)<<8;
	val	+=	ch0_data[2];
	volt_value	=	3.3*(float)val/4095.0;
	printf("voltage = %f\n",(float)volt_value);
}

int main(int arc, char **argv) 
{
  char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
  char ch0_data[] = { 0x00, 0x00, 0x00 };
  char out_ch1[] = { 0b00000110, 0b01000000, 0b00000000 };
  char ch1_data[] = { 0x00, 0x00, 0x00 };
  char out_ch2[] = { 0b00000110, 0b10000000, 0b00000000 };
  char ch2_data[] = { 0x00, 0x00, 0x00 }; 

	mog_adc mog_photo;
	mog_photo.set_adc();
  
  while(1){

			mog_photo.read_val(out_ch0,ch0_data,3);
      //bcm2835_spi_transfernb(out_ch0, ch0_data, 3);
      //printf("CH0:    %02X %02X %02X\n", ch0_data[0], ch0_data[1], ch0_data[2]);
			//get_dist(out_ch0,ch0_data);
			mog_photo.get_volt(out_ch0,ch0_data);
      bcm2835_spi_transfernb(out_ch1, ch1_data, 3);
      //printf("CH1:    %02X %02X %02X\n", ch1_data[0], ch1_data[1], ch1_data[2]);
      bcm2835_spi_transfernb(out_ch2, ch2_data, 3);
      //printf("CH2:    %02X %02X %02X\n", ch2_data[0], ch2_data[1], ch2_data[2]);
  }
     
  bcm2835_spi_end();
  bcm2835_close();
}
