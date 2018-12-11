#include <stdio.h>
#include "SPIdev.h"

class mog_adc
{
	public:
//		void set_adc();
//		float ad2dist(int ad);
//		void read_val(char* out_ch0, char* ch0_data, int val);
		float get_volt(unsigned char* out_ch0, unsigned char* ch0_data, int len);
};
