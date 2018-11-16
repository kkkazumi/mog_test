#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
 

#include "servo.h"
#include "adcread.h"

#define MOG_UP 80
#define MOG_DOWN 120

#define PHOTO_THRE 0.005


int main(int argc, char const* argv[]){
	char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
  char ch0_data[] = { 0x00, 0x00, 0x00 };

	//class
  mog_servo mogura;
	mog_adc mog_photo;

	mogura.setup();
	mog_photo.set_adc();

	float volt_val;

	while(1){

		mog_photo.read_val(out_ch0,ch0_data,3);
		volt_val = mog_photo.get_volt(out_ch0,ch0_data);
		std::cout<<volt_val<<std::endl;

		if(volt_val<PHOTO_THRE){
			mogura.move(MOG_DOWN);
		}else if(volt_val>=PHOTO_THRE){
			mogura.move(MOG_UP);
		}

	}
}
