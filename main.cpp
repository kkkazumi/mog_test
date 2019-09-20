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



struct timeval stTime;
pthread_mutex_t mutex;

void* servo_test(void* arg){
	int ret;
	ret = system("python /home/pi/prog/Adafruit_Python_PCA9685/examples/random7.py");
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
	}
}

void* fsr_test(void* arg){

	time_t timer;
	struct tm *t_st = localtime(&timer);

	char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
	char out_ch1[] = { 0b00000110, 0b01000000, 0b00000000 };
	char out_ch2[] = { 0b00000110, 0b10000000, 0b00000000 };
	char out_ch3[] = { 0b00000110, 0b11000000, 0b00000000 };
	char out_ch4[] = { 0b00000111, 0b00000000, 0b00000000 };
	char out_ch5[] = { 0b00000111, 0b01000000, 0b00000000 };
	char out_ch6[] = { 0b00000111, 0b10000000, 0b00000000 };
	char out_ch7[] = { 0b00000111, 0b11000000, 0b00000000 };
	char ch0_data[] = { 0x00, 0x00, 0x00 };
	char ch1_data[] = { 0x00, 0x00, 0x00 };
	char ch2_data[] = { 0x00, 0x00, 0x00 };
	char ch3_data[] = { 0x00, 0x00, 0x00 };
	char ch4_data[] = { 0x00, 0x00, 0x00 };
	char ch5_data[] = { 0x00, 0x00, 0x00 };
	char ch6_data[] = { 0x00, 0x00, 0x00 };
	char ch7_data[] = { 0x00, 0x00, 0x00 };
	float volt_photo1;
	float volt_photo2;
	float volt_photo3;
	float volt_photo4;
	float volt_photo5;
	float volt_photo6;
	float volt_photo7;
	float volt_fsr;
	mog_adc mog_photo;
	mog_photo.set_adc();

	FILE *fp_ad;
	fp_ad = fopen("ad_data_test.csv","w");
	float data[11];

	while(1){

		volt_photo1= mog_photo.get_volt(out_ch0,ch0_data);
		volt_photo2= mog_photo.get_volt(out_ch1,ch1_data);
		volt_photo3= mog_photo.get_volt(out_ch2,ch2_data);
		volt_photo4= mog_photo.get_volt(out_ch3,ch3_data);
		volt_photo5= mog_photo.get_volt(out_ch4,ch4_data);
		volt_photo6= mog_photo.get_volt(out_ch5,ch5_data);
		volt_photo7= mog_photo.get_volt(out_ch6,ch6_data);
		volt_fsr= mog_photo.get_volt(out_ch7,ch7_data);

		struct timeval nowTime;
		gettimeofday(&nowTime,NULL);
		time(&timer);
		t_st = localtime(&timer);
//		printf("fsr volt%f\n",volt_val);

		data[0] = volt_photo1;
		data[1] = volt_photo2;
		data[2] = volt_photo3;
		data[3] = volt_photo4;
		data[4] = volt_photo5;
		data[5] = volt_photo6;
		data[6] = volt_photo7;
		data[7] = volt_fsr;
		data[8] = (float)(t_st->tm_min);
		data[9] = (float)(t_st->tm_sec);
		data[10] = (float)nowTime.tv_usec;

		printf("%f,%f,%f,%f,%f,%f,%f,%f\n",
			data[0],data[1],data[2],data[3],
			data[4],data[5],data[6],data[7]);
//		data[0] = volt_photo;
//		data[1] = volt_fsr;
		fprintf(fp_ad,"%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%f,\n",
			data[0],data[1],
			data[2],data[3],
			data[4],data[5],
			data[6],data[7],
			(int)t_st->tm_min,(int)t_st->tm_sec,data[10]);
			//data[0],data[1],data[2],data[3],data[4]);
	}
	fclose(fp_ad);
}

void* imu_test(void* arg){

	time_t timer;
	struct tm *t_st = localtime(&timer);

	float ax,ay,az;
	float gx,gy,gz;
	float mx,my,mz;
	int ref;

	FILE *fp_imu;
	fp_imu = fopen("imu_data_test.csv","w");

	MPU9250 mog_mpu;
	mog_mpu.initialize();
	ref=mog_mpu.testConnection();
	float data[9];

	while(1){

		mog_mpu.getMotion9(&ax,&ay,&az,&gx,&gy,&gz,&mx,&my,&mz);

		struct timeval nowTime;
		gettimeofday(&nowTime,NULL);
		time(&timer);
		t_st = localtime(&timer);
//		printf("fsr volt%f\n",volt_val);

		data[0] = ax;
		data[1] = ay;
		data[2] = az;
		data[3] = gx;
		data[4] = gy;
		data[5] = gz;
		data[6] = (float)(t_st->tm_min);
		data[7] = (float)(t_st->tm_sec);
		data[8] = (float)nowTime.tv_usec;

		//printf("%f,%f,%f,",ax,ay,az);
		//printf("%f,%f,%f\n",gx,gy,gz);
		//printf("%f,%f,%f\n",mx,my,mz);

		fprintf(fp_imu,"%f,%f,%f,%f,%f,%f,%d,%d,%f\n",
			data[0],data[1],data[2],data[3],data[4],data[5],
			t_st->tm_min,t_st->tm_sec,data[8]);
			//ax,ay,az,gx,gy,gz,mx,my,mz);
	}
	fclose(fp_imu);

}

int main(int argc, char const* argv[]){

	pthread_t thr_sv;
	pthread_t thr_imu;
	pthread_t thr_fsr;

	gettimeofday(&stTime,NULL);
//class
//	mog_servo mogura;
/*
	

	float volt_val=0;
	float before_volt=0.1;
	int count = 0;
	int ran=0;
	*/


//	mogura.move(MOG_UP);
//	int flg=S_UP;
	//time_t st_timer,now_timer;
	//st_timer = time(NULL);
	//double dt;
	//double thre_time = 3;

	pthread_create(&thr_sv, NULL, servo_test,NULL);
	//pthread_create(&thr_ad, NULL, photo_test,&mog_photo);
	//pthread_create(&thr_ad, NULL, photo_test,NULL);
	pthread_create(&thr_imu, NULL, imu_test,NULL);
	pthread_create(&thr_fsr, NULL, fsr_test,NULL);


	pthread_join(thr_sv,NULL);
	//pthread_join(thr_ad,NULL);
	pthread_join(thr_imu,NULL);
	pthread_join(thr_fsr,NULL);

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
