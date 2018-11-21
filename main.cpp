#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "servo.h"
#include "adcread.h"

#define MOG_UP 120
#define MOG_DOWN 80

#define PHOTO_THRE 0.25

#define S_UP 1
#define S_DOWN 0

void return_ud(int flg){
	if(flg==S_UP) printf("UP\n");
	if(flg==S_DOWN) printf("DOWN\n");
}

int main(int argc, char const* argv[]){
	char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
	char ch0_data[] = { 0x00, 0x00, 0x00 };

	//class
	mog_servo mogura;
	mog_adc mog_photo;

	mogura.setup();
	mog_photo.set_adc();

	float volt_val=0;
	float before_volt[100]={0};
	int count = 0;
	int ran=0;
	mogura.move(MOG_DOWN);
	sleep(1);
	mogura.move(MOG_UP);
	sleep(1);
	int flg=S_UP;
	time_t st_timer,now_timer;
	st_timer = time(NULL);
	double dt;
	double thre_time = 3;

	while(1){
		//photo
		mog_photo.read_val(out_ch0,ch0_data,3);
		volt_val = mog_photo.get_volt(out_ch0,ch0_data);

		//time
		now_timer = time(NULL);
		dt = difftime(now_timer,st_timer);
		std::cout<<dt<<"\t"<<volt_val<<std::endl;
		return_ud(flg);

		//
		if(flg == S_DOWN){
			if(dt > thre_time){
				mogura.move(MOG_UP);
				st_timer = time(NULL);
				flg = S_UP;
			}
		}else if(flg == S_UP){
			if(dt > thre_time){
				mogura.move(MOG_DOWN);
				st_timer = time(NULL);
				flg = S_DOWN;
			}
			/*
			else if(volt_val>PHOTO_THRE){
				mogura.move(MOG_DOWN);
				st_timer = time(NULL);
				flg = S_DOWN;
			}
			*/
		}
		before_volt=volt_val;
//		sleep(1);
	}
}
