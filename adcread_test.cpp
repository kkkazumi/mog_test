#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//to get time
#include <time.h>
#include <sys/time.h>

#include "adcread.h"

#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>


#define REF_VOLTAGE     5.0     /* 基準電圧[V] */


int main(int arc, char **argv) 
{
	mog_adc mog_photo;

  unsigned char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
  unsigned char ch0_data[] = { 0x00, 0x00, 0x00 };

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
			//mog_photo.read_val(out_ch0,ch0_data,3);
			mog_photo.get_volt(0,&val);

			std::cout << realsec << ",\t" << val << std::endl;
  }
     
 // bcm2835_spi_end();
 // bcm2835_close();
}
