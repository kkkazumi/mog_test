#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <pthread.h>

#include "adcread.h"
#include "MPU9250.h"

//#define MOG_UP 80
//#define MOG_DOWN 120

#define PHOTO_THRE 0.4

#define S_UP 1
#define S_DOWN 0

float data[2] = {0};


pthread_mutex_t mutex;

void* servo_test(void* arg){
	int ret;
	ret = system("python /home/pi/prog/Adafruit_Python_PCA9685/examples/simpletest.py");
}

void* photo_test(void* arg){
	char out_ch0[] = { 0b00000110, 0b01000000, 0b00000000 };
	char ch0_data[] = { 0x00, 0x00, 0x00 };
	float volt_val;
	mog_adc mog_photo;

	while(1){
		mog_photo.read_val(out_ch0,ch0_data,3);
		volt_val = mog_photo.get_volt(out_ch0,ch0_data);
//		printf("photo volt%f\n",volt_val);
		data[0] = volt_val;
	}
}

void* fsr_test(void* arg){
	char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
	char out_ch1[] = { 0b00000110, 0b01000000, 0b00000000 };
	char ch0_data[] = { 0x00, 0x00, 0x00 };
	char ch1_data[] = { 0x00, 0x00, 0x00 };
	float volt_photo;
	float volt_fsr;
	mog_adc mog_photo;

	while(1){
		mog_photo.read_val(out_ch0,ch0_data,3);
		volt_photo= mog_photo.get_volt(out_ch0,ch0_data);

		mog_photo.read_val(out_ch1,ch1_data,3);
		volt_fsr= mog_photo.get_volt(out_ch1,ch1_data);
//		printf("fsr volt%f\n",volt_val);

		data[0] = volt_photo;
		data[1] = volt_fsr;
	}
}

void* print_all(void *arg){
	FILE *fp;
	fp = fopen("data_test.csv","w");
	while(1){
		fprintf(fp,"%f,%f\n",data[0],data[1]);
		printf("%f,%f\n",data[0],data[1]);
	}
	fclose(fp);
}


void* imu_test(void* arg){
	float ax,ay,az;
	float gx,gy,gz;
	int ref;

	MPU9250 mog_mpu;
	mog_mpu.initialize();
	ref=mog_mpu.testConnection();

	while(1){
		mog_mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
		//printf("%f,%f,%f,",ax,ay,az);
		//printf("%f,%f,%f\n",gx,gy,gz);

	}

}

int main(int argc, char const* argv[]){

	pthread_t thr_sv;
	pthread_t thr_ad;
	pthread_t thr_imu;
	pthread_t thr_fsr;
	pthread_t thr_prt;
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
	//pthread_create(&thr_ad, NULL, photo_test,NULL);
//	pthread_create(&thr_imu, NULL, imu_test,NULL);
	pthread_create(&thr_fsr, NULL, fsr_test,NULL);
	pthread_create(&thr_prt, NULL, print_all,NULL);


	pthread_join(thr_sv,NULL);
	//pthread_join(thr_ad,NULL);
//	pthread_join(thr_imu,NULL);
	pthread_join(thr_fsr,NULL);
	pthread_join(thr_prt,NULL);

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
