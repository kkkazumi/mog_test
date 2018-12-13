#ifndef _ADCREAD_H
#define _ADCREAD_H

//#include <stdio.h>
#include "SPIdev.h"

class mog_adc
{
	public:
		mog_adc();

		unsigned int WriteReg( uint8_t WriteAddr, uint8_t WriteData );
		unsigned int ReadReg( uint8_t WriteAddr, uint8_t WriteData );
		void ReadRegs( uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes );
		void read_photo(int ch);
		void get_volt(int ch, float *photo);

	public:
		float photo_data; //for test
};

#endif

#define READ_FLAG   0x80

//		void set_adc();
//		float ad2dist(int ad);
//		void read_val(char* out_ch0, char* ch0_data, int val);
//		float mog_adc::init();
//		float get_volt(unsigned char* out_ch0, unsigned char* ch0_data, int len);

