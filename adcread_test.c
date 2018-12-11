#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>

//to get time
#include <time.h>
#include <sys/time.h>

#include "mcp3208.h"


#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

#include "mcp3208.h"

#define REF_VOLTAGE     5.0     /* 基準電圧[V] */


int main(int argc, char *argv[])
{
        struct mcp3208 *adc;
        const char *device;
        int channel = 0;
        double voltage;
        uint16_t digital_code;
        int ret;

//        exitfail_init();

//        parse_arg(argc, argv, &device, &channel);

        adc = mcp3208_open(device);
        if (adc == NULL)
//                exitfail_errno("mcp3208_open");

		while(1){
        ret = mcp3208_read(adc, channel, &digital_code);
        if (ret != 0)
//                exitfail_errno("mcp3208_read");

        voltage = digital_code * REF_VOLTAGE /
                ((1 << MCP3208_RESOLUTION_BITS) - 1);

        printf("%1.3fV\n", voltage);
		}

        ret = mcp3208_close(adc);
        if (ret != 0)
//                exitfail_errno("mcp3208_close");

        return 0;
}

/*
int main(int arc, char **argv) 
{
  unsigned char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
  //unsigned char ch0_data[] = { 0x00, 0x00, 0x00 };
  unsigned char ch0_data[] = { 0x00, 0x00, 0x00 };

	mog_adc mog_photo;
//	mog_photo.set_adc();
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
			val = mog_photo.get_volt(out_ch0,ch0_data,3);

//			std::cout << realsec << ",\t" << val << std::endl;
  }
     
 // bcm2835_spi_end();
 // bcm2835_close();
}
*/
