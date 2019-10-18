#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//to get time
#include <time.h>
#include <sys/time.h>

#include "adcread.h"

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
	float val;

	//to get time
	struct timeval stTime, nowTime;
	time_t diffsec;
	suseconds_t diffsub;
	double realsec;

	gettimeofday(&stTime,NULL);

  while(1){
			//get time calc dt
			gettimeofday(&nowTime,NULL);
			diffsec = difftime(nowTime.tv_sec, stTime.tv_sec);
			diffsub = nowTime.tv_usec - stTime.tv_usec;
			realsec = diffsec + diffsub*1e-6;

			//get ad val
			mog_photo.read_val(out_ch0,ch0_data,3);
			val = mog_photo.get_volt(out_ch1,ch1_data);

			std::cout << realsec << ",\t" << val << std::endl;
  }
     
  bcm2835_spi_end();
  bcm2835_close();
}
