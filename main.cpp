#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <pthread.h>
#include <mutex>
#include <string.h>

#include "adcread.h"
#include "MPU9250.h"
#define ABUF_SIZE (5)
#define IBUF_SIZE (9) 
#define SET (100) 
#define ALOGBUFSIZE (ABUF_SIZE * SET)
#define ILOGBUFSIZE (IBUF_SIZE * SET)

struct timeval nowTime;
time_t now;
struct tm *pnow = localtime(&now);

float adBuf[ABUF_SIZE];
float imBuf[IBUF_SIZE];

float *rp_a,*rp_i, *wp_a,*wp_i;
int ra_count, ri_count, wa_count, wi_count;

std::mutex mtx;

void* servo_test(void* arg){
	int ret;
	ret = system("python /home/pi/prog/Adafruit_Python_PCA9685/examples/simpletest.py");
}

void* fsr_test(void* arg){
	printf("fsr in");
	char out_ch0[] = { 0b00000110, 0b00000000, 0b00000000 };
	char out_ch1[] = { 0b00000110, 0b01000000, 0b00000000 };
	char ch0_data[] = { 0x00, 0x00, 0x00 };
	char ch1_data[] = { 0x00, 0x00, 0x00 };
	float volt_photo;
	float volt_fsr;
	mog_adc mog_photo;
	float data[5];

	while(1){
	/*
		if(wa_count == SET){
			wa_count = 0;
			//pthread_mutex_lock(&lock);
			std::lock_guard<std::mutex> lock(mtx);
			wp_a = adBuf;
			//pthread_mutex_unlock(&lock);
		}
	*/

		//mog_photo.read_val(out_ch0,ch0_data,3);
		volt_photo= mog_photo.get_volt(out_ch0,ch0_data);

		//mog_photo.read_val(out_ch1,ch1_data,3);
		volt_fsr= mog_photo.get_volt(out_ch1,ch1_data);

		gettimeofday(&nowTime,NULL);
		struct tm *pnow = localtime(&now);

		data[0] = volt_photo;
		data[1] = volt_fsr;
		data[2] = (float)(pnow->tm_min);
		data[3] = (float)nowTime.tv_sec;
		data[4] = (float)nowTime.tv_usec;
		for(int i = 0;i<5;i++){
			printf("%f\n",data[i]);
		}

/*
		//pthread_mutex_lock(&lock);
		std::lock_guard<std::mutex> lock(mtx);
		//memcpy(wp_a,data,sizeof(float)*ABUF_SIZE);
		wp_a+=ABUF_SIZE;//?!
		wa_count++;
		//pthread_mutex_unlock(&lock);
		*/
	}
}

	//FILE *fp_ad;
	//fp_ad = fopen("ad_data_test.csv","w");
//		printf("fsr volt%f\n",volt_val);
	//fclose(fp_ad);
	//	fprintf(fp_ad,"%f,%f,%f\n",realsec,volt_photo,volt_fsr);

void *save_log(void* arg)
{
	printf("in save");
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

	while(1){
		if(ra_count == 0){
			//pthread_mutex_lock(&lock);
			std::lock_guard<std::mutex> lock(mtx);
			rp_a = adBuf;
			//pthread_mutex_unlock(&lock);
		}
		if(ri_count == 0){
			std::lock_guard<std::mutex> lock(mtx);
			//pthread_mutex_lock(&lock);
			rp_i = imBuf;
			//pthread_mutex_unlock(&lock);
		}

		//pthread_mutex_lock(&lock);
		std::lock_guard<std::mutex> lock(mtx);
		write(fdf_a,rp_a,sizeof(float)*ABUF_SIZE);
		write(fdf_i,rp_i,sizeof(float)*IBUF_SIZE);

		rp_a+=ABUF_SIZE;
		rp_i+=IBUF_SIZE;

		ra_count++;
		ri_count++;

		if(ra_count == SET) ra_count = 0;
		if(ri_count == SET) ri_count = 0;
	}
	//pthread_mutex_unlock(&lock);

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
			//pthread_mutex_lock(&lock);
			std::lock_guard<std::mutex> lock(mtx);
			wi_count = 0;
			wp_i = adBuf;
			//pthread_mutex_unlock(&lock);
		}

		mog_mpu.getMotion9(&ax,&ay,&az,&gx,&gy,&gz,&mx,&my,&mz);
		gettimeofday(&nowTime,NULL);
		struct tm *pnow = localtime(&now);

		data[0] = ax;
		data[1] = ay;
		data[2] = az;
		data[3] = gx;
		data[4] = gy;
		data[5] = gz;
		data[6] = (float)(pnow->tm_min);
		data[7] = (float)nowTime.tv_sec;
		data[8] = (float)nowTime.tv_usec;

		std::lock_guard<std::mutex> lock(mtx);
		//pthread_mutex_lock(&lock);
		memcpy(wp_i,data,sizeof(float)*IBUF_SIZE);
		wp_i+=IBUF_SIZE;//?!
		//pthread_mutex_unlock(&lock);
	}
}

//	FILE *fp_imu;
//	fp_imu = fopen("imu_data_test.csv","w");

//		fprintf(fp_imu,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
//			realsec,ax,ay,az,gx,gy,gz,mx,my,mz);
//	}

//	fclose(fp_imu);

void *test(void *argv){
	int count=0;
	while(1){
		if(count%10==0){
			printf("dummy func\n");
		}
		count++;
	}
}

int main(int argc, char const* argv[]){

	pthread_t thr_sv;
	//pthread_t thr_imu;
	pthread_t thr_fsr;
	pthread_t thr_log;


	pthread_create(&thr_sv, NULL, test,NULL);
//	pthread_create(&thr_sv, NULL, test,NULL);
	//pthread_create(&thr_imu, NULL, imu_test,NULL);
	pthread_create(&thr_fsr, NULL, fsr_test,NULL);
	pthread_create(&thr_log, NULL, save_log,NULL);


	pthread_join(thr_sv,NULL);
	//pthread_join(thr_imu,NULL);
	pthread_join(thr_fsr,NULL);
	pthread_join(thr_log,NULL);
}
