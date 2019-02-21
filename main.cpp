#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <pthread.h>

#include "adcread.h"
#include "MPU9250.h"

#define ABUF_SIZE 5
#define IBUF_SIZE 9
#define SET 20
#define ALOGBUFSIZE (ABUF_SIZE * SET)
#define ILOGBUFSIZE (IBUF_SIZE * SET)

struct timeval stTime, nowTime;
time_t diffsec;
suseconds_t diffsub;
double realsec;

float adBuf[ABUF_SIZE];
float imBuf[IBUF_SIZE];

float *rp_a,*rp_i, *wp_a,*wp_i;
int r_count, wa_count, wi_count;

pthread_mutex_t lock;

void* servo_test(void* arg){
	int ret;
	ret = system("python /home/pi/prog/Adafruit_Python_PCA9685/examples/simpletest.py");
}


void* fsr_test(void* arg){
	char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
	char out_ch1[] = { 0b00000110, 0b01000000, 0b00000000 };
	char ch0_data[] = { 0x00, 0x00, 0x00 };
	char ch1_data[] = { 0x00, 0x00, 0x00 };
	float volt_photo;
	float volt_fsr;
	mog_adc mog_photo;
	//FILE *fp_ad;
	//fp_ad = fopen("ad_data_test.csv","w");
	float data[5];

	while(1){
		if(wa_count == SET){
			wa_count = 0;
			pthread_mutex_lock(&lock);
			wp_a = adBuf;
			pthread_mutex_unlock(&lock);
		}

		mog_photo.read_val(out_ch0,ch0_data,3);
		volt_photo= mog_photo.get_volt(out_ch0,ch0_data);

		mog_photo.read_val(out_ch1,ch1_data,3);
		volt_fsr= mog_photo.get_volt(out_ch1,ch1_data);

		gettimeofday(&stTime,NULL);

		data[0] = volt_photo;
		data[1] = volt_fsr;
		data[2] = nowTime.tv_min;
		data[3] = nowTime.tv_sec;
		data[4] = nowTime.tv_usec;

		pthread_mutex_lock(&lock);
		memcpy(data,sizeof(float)*ABUF_SIZE);
		wp_a+=LOGBUFSIZE;//?!
		wa_count++;
		pthread_mutex_unlock(&lock);
	}
}

//		printf("fsr volt%f\n",volt_val);
	//fclose(fp_ad);
	//	fprintf(fp_ad,"%f,%f,%f\n",realsec,volt_photo,volt_fsr);

void *save_log(void* arg)
{
	int i;
	int fdf_a,fdf_i;

	char afile[50],ifile[50];
	sprintf(afile,"log_data/ad_log_test.dat");
	sprintf(ifile,"log_data/imu_log_test.dat");

	if((fdf_a=open(afile,O_WRONLY|O_CREAT|O_TRUNC, 0755))==-1){
		perror("cantopen\n");
		exit(EXIT_FAILURE);
	}

	if((fdf_i=open(ifile,O_WRONLY|O_CREAT|O_TRUNC, 0755))==-1){
		perror("cantopen\n");
		exit(EXIT_FAILURE);
	}

	if(ra_count == 0){
		pthread_mutex_lock(&lock);
		rp_a = adBuf;
		pthread_mutex_unlock(&lock);
	}
	if(ri_count == 0){
		pthread_mutex_lock(&lock);
		rp_i = imBuf;
		pthread_mutex_unlock(&lock);
	}

	pthread_mutex_lock(&lock);
	write(fdf_a,rp_a,sizeof(float)*LOGBUFSIZE);
	write(fdf_i,rp_i,sizeof(float)*LOGBUFSIZE);

	rp_a+=LOGBUFSIZE;
	rp_i+=LOGBUFSIZE;

	ra_count++;
	ri_count++;

	if(ra_count == SET) ra_count = 0;
	if(ri_count == SET) ri_count = 0;
	pthread_mutex_unlock(&lock);

	close(fdf_a);
	close(fdf_i);
}

void* imu_test(void* arg){
	float ax,ay,az;
	float gx,gy,gz;
	float mx,my,mz;
	int ref;

	MPU9250 mog_mpu;
	mog_mpu.initialize();
	ref=mog_mpu.testConnection();
	float data[9];

	while(1){
		if(wi_count == SET){
			pthread_mutex_lock(&lock);
			wi_count = 0;
			wp_i = adBuf;
			pthread_mutex_unlock(&lock);
		}

		mog_mpu.getMotion9(&ax,&ay,&az,&gx,&gy,&gz,&mx,&my,&mz);
		gettimeofday(&stTime,NULL);

		data[0] = ax;
		data[1] = ay;
		data[2] = az;
		data[3] = gx;
		data[4] = gy;
		data[5] = gz;
		data[6] = nowTime.tv_min;
		data[7] = nowTime.tv_sec;
		data[8] = nowTime.tv_usec;

		pthread_mutex_lock(&lock);
		memcpy(data,sizeof(float)*IBUF_SIZE);
		wp_i+=IBUFSIZE;//?!
		pthread_mutex_unlock(&lock);
	}
}

//	FILE *fp_imu;
//	fp_imu = fopen("imu_data_test.csv","w");

//		fprintf(fp_imu,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
//			realsec,ax,ay,az,gx,gy,gz,mx,my,mz);
//	}

//	fclose(fp_imu);

int main(int argc, char const* argv[]){

	pthread_t thr_sv;
	pthread_t thr_imu;
	pthread_t thr_fsr;
	//class
//	mog_servo mogura;
	mog_adc mog_photo;
//	mogura.setup();
	mog_photo.set_adc();

	pthread_mutex_init(&lock, NULL);

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
	pthread_create(&thr_imu, NULL, imu_test,NULL);
	pthread_create(&thr_fsr, NULL, fsr_test,NULL);


	pthread_join(thr_sv,NULL);
	//pthread_join(thr_ad,NULL);
	pthread_join(thr_imu,NULL);
	pthread_join(thr_fsr,NULL);

	pthread_mutex_destroy(&mutex);
}
