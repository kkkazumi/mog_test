//g++ -o tes adcread_test.cpp adcread.cpp -lbcm2835
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//to get time
#include <time.h>
#include <sys/time.h>

#include "adcread.h"

#include <iomanip>

int main(int arc, char **argv) 
{
	//char out_ch7[] = { 0b00000111, 0b11000000, 0b00000000 };

	char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
	char out_ch1[] = { 0b00000110, 0b01000000, 0b00000000 };
	char out_ch2[] = { 0b00000110, 0b10000000, 0b00000000 };
	char out_ch3[] = { 0b00000110, 0b11000000, 0b00000000 };
	char out_ch4[] = { 0b00000111, 0b00000000, 0b00000000 };
	char out_ch5[] = { 0b00000111, 0b01000000, 0b00000000 };
	char out_ch6[] = { 0b00000111, 0b10000000, 0b00000000 };
	char out_ch7[] = { 0b00000111, 0b11000000, 0b00000000 };

  char ch7_data[] = { 0x00, 0x00, 0x00 };

	mog_adc mog_photo;
	mog_photo.set_adc();
	float val0,val1,val2,val3,val4,val5,val6,val7;

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
	//		mog_photo.read_val(out_ch0,ch0_data,8);
			val0 = mog_photo.get_volt(out_ch0,ch7_data);
			val1 = mog_photo.get_volt(out_ch1,ch7_data);
			val2 = mog_photo.get_volt(out_ch2,ch7_data);
			val3 = mog_photo.get_volt(out_ch3,ch7_data);
			val4 = mog_photo.get_volt(out_ch4,ch7_data);
			val5 = mog_photo.get_volt(out_ch5,ch7_data);
			val6 = mog_photo.get_volt(out_ch6,ch7_data);
			val7 = mog_photo.get_volt(out_ch7,ch7_data);
			std::cout<< std::setprecision(3);
			std::cout << realsec << ",\t" << val0<<",\t" << val1<<",\t" << val2<<",\t";
			std::cout<<val3<<",\t" << val4<<",\t" << val5<<",\t" << val6<<",\t"<<val7 << std::endl;
  }
     
  bcm2835_spi_end();
  bcm2835_close();
}
