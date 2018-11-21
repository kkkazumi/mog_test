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

#define PHOTO_THRE 0.4

#define S_UP 1
#define S_DOWN 0



int main(int argc, char const* argv[]){
	char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
	char ch0_data[] = { 0x00, 0x00, 0x00 };

	//class
	mog_servo mogura;
	mog_adc mog_photo;

	mogura.setup();
	mog_photo.set_adc();

	float volt_val=0;
	float before_volt=0.1;
	int count = 0;
	int ran=0;
	int flg=S_DOWN;
	mogura.move(MOG_UP);
	time_t st_timer,now_timer;
	st_timer = time(NULL);
	double dt;
	double thre_time = 3;

	while(1){
		now_timer = time(NULL);
		dt = difftime(st_timer,now_timer);
		std::cout<<dt<<std::endl;

		if(dt > thre_time && flg == S_DOWN){
			mogra.move(MOG_UP);
			st_timer = time(NULL);
			flg = S_UP;
		}
		if(dt > thre_time && flg == S_UP){
			mogura.move(MOG_DOWN);
			st_timer = time(NULL);
			flg = S_DOWN;
		}

		before_volt=volt_val;

	}
}
