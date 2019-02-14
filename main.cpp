#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <pthread.h>

#include "adcread.h"

//#define MOG_UP 80
//#define MOG_DOWN 120

#define PHOTO_THRE 0.4

#define S_UP 1
#define S_DOWN 0

pthread_mutex_t mutex;

void* servo_test(void* arg){
	int ret;
	ret = system("python /home/pi/prog/Adafruit_Python_PCA9685/examples/simpletest.py");
}

void* photo_test(void* arg){
	char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
	char ch0_data[] = { 0x00, 0x00, 0x00 };
	float volt_val;
	mog_adc mog_photo;

	while(1){
		mog_photo.read_val(out_ch0,ch0_data,3);
		volt_val = mog_photo.get_volt(out_ch0,ch0_data);
		printf("volt%f\n",volt_val);
	}
}

int main(int argc, char const* argv[]){

	pthread_t thr_sv;
	pthread_t thr_ad;
	//class
//	mog_servo mogura;
	mog_adc mog_photo;
//	mogura.setup();
	mog_photo.set_adc();

	float volt_val=0;
	float before_volt=0.1;
	int count = 0;
	int ran=0;
//	mogura.move(MOG_UP);
//	int flg=S_UP;
	time_t st_timer,now_timer;
	st_timer = time(NULL);
	double dt;
	double thre_time = 3;

	pthread_create(&thr_sv, NULL, servo_test,NULL);
	//pthread_create(&thr_ad, NULL, photo_test,&mog_photo);
	pthread_create(&thr_ad, NULL, photo_test,NULL);

	pthread_join(thr_sv,NULL);
	pthread_mutex_destroy(&mutex);
}
/*
	while(1){
		//photo
	//	mog_photo.read_val(out_ch0,ch0_data,3);
	//	volt_val = mog_photo.get_volt(out_ch0,ch0_data);
		//time
		now_timer = time(NULL);
		dt = difftime(now_timer,st_timer);
//		std::cout<<volt_val<<std::endl;

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
			if(volt_val>PHOTO_THRE){
				mogura.move(MOG_DOWN);
				st_timer = time(NULL);
				flg = S_DOWN;
			}
		}

		before_volt=volt_val;
	}
		*/
