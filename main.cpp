//for test
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

#include <assert.h>
#include "MovingAverage.h"

//#define MOG_UP 80
//#define MOG_DOWN 120
#include <linux/i2c-dev.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "servo_c.h"



#define PHOTO_THRE 0.4

#define S_UP 2300
#define S_DOWN 1500

struct timeval stTime;
pthread_mutex_t mutex;

class mogura
{
private:
	float ad_data[8][2000];
	float imu_data[6][500];
	static int hitflg[7];

public:
	static void* servo_test(void* arg);
	static void* fsr_test(void* arg);
	static void* imu_test(void* arg);
	static void* led(void* arg);
	static void change_flg(int i_ad);
};
	
//////////
int mogura::hitflg[7]={0};

void mogura::change_flg(int i_ad){
	if(hitflg[i_ad]==1){
		hitflg[i_ad]=0;
	}else if(hitflg[i_ad]==0){
		hitflg[i_ad]=1;
	}else if(hitflg[i_ad]==2){
	}
}

//void* servo_test(void* arg){
void* mogura::servo_test(void* arg){
	int i2c;    // ファイルディスクリプタ
	char *i2cFileName = "/dev/i2c-1"; // I2Cデバイスのパス（古いものはi2c-0）

	int driverAddress = 0x40;

	if ((i2c = open(i2cFileName, O_RDWR)) < 0)
	{
		printf("Faild to open i2c port\n");
		exit(1);
	}

	if (ioctl(i2c, I2C_SLAVE, driverAddress) < 0)
	{
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}

	Ada_ServoDriver servo(i2c);
	servo.reset();
	servo.setPWMFreq(SERVO_CONTROL_FREQUENCY);
 	int ran_array[7]={0};
 	int slp=0;
	int servo_id=0;


	struct timeval nowTime;
	time_t timer;
	struct tm *t_st = localtime(&timer);

		int t;
		int test_t;


	while(1){
//		pthread_mutex_lock(&mutex);

	gettimeofday(&nowTime,NULL);
	time(&timer);
	t_st = localtime(&timer);
	//printf("time,%d,%d,%d\n",
	//(int)t_st->tm_min,(int)t_st->tm_sec,
	//(int)nowTime.tv_usec);
//		printf("fsr volt%f\n",volt_val);


		std::srand(time(NULL));
		slp=rand()%3;

		test_t++;
		t=test_t%2;

		for(int servo_id=0;servo_id<7;servo_id++){
			ran_array[servo_id]=rand()%2;
			//ran_array[servo_id]=t;
			if(ran_array[servo_id]==1){
				servo.setServoPulse(servo_id,S_UP);
			}else{
				hitflg[servo_id]=2;
				servo.setServoPulse(servo_id,S_DOWN);
				//sleep(1);
			}
		}

		//sleep(1);
		usleep((slp+1)*100000);
		//usleep((slp+1)*500000);
//		pthread_mutex_unlock(&mutex);
		for(int id=0;id<7;id++){
			hitflg[id]=0;
		}
		//usleep((slp+1)*30000);
	}
}
//void* fsr_test(void* arg){
void* mogura::fsr_test(void* arg){

	time_t timer;
	struct tm *t_st = localtime(&timer);
	char filename[100];

	struct timeval nowTime;

	gettimeofday(&nowTime,NULL);
	time(&timer);
	t_st = localtime(&timer);
	sprintf(filename,"/mnt/mogura/ad_data_%d%d%d-%d%d%d.csv",
	1990+(int)t_st->tm_year,(int)t_st->tm_mon,(int)t_st->tm_mday,(int)t_st->tm_hour,(int)t_st->tm_min,(int)t_st->tm_sec);
//		printf("fsr volt%f\n",volt_val);

	FILE *fp_ad;
	fp_ad = fopen(filename,"w");
	float data[11];

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

	float volt_photo[7];
	float volt_fsr;

	mog_adc mog_photo;

	mog_photo.set_adc();

	//MovingAverage<float> intAverager(1000);
	MovingAverage<float> intAverager[7];
	MovingAverage<float> intAve2[7];
	MovingAverage<float> fsAve;
	fsAve.average_count = 7;
	for(int c_ave=0;c_ave<7;c_ave++){
		intAverager[c_ave].average_count=1000;
		intAve2[c_ave].average_count=1000;
	}

	float average[8]={0};
	float before[8]={0};

	float ave2[8]={0};
	float bef2[8]={0};

	while(1){

		volt_photo[0]= mog_photo.get_volt(out_ch0,ch0_data);
		volt_photo[1]= mog_photo.get_volt(out_ch1,ch1_data);
		volt_photo[2]= mog_photo.get_volt(out_ch2,ch2_data);
		volt_photo[3]= mog_photo.get_volt(out_ch3,ch3_data);
		volt_photo[4]= mog_photo.get_volt(out_ch4,ch4_data);
		volt_photo[5]= mog_photo.get_volt(out_ch5,ch5_data);
		volt_photo[6]= mog_photo.get_volt(out_ch6,ch6_data);
		volt_fsr= mog_photo.get_volt(out_ch7,ch7_data);

		struct timeval nowTime;
		gettimeofday(&nowTime,NULL);
		time(&timer);
		t_st = localtime(&timer);
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

	for(int i_ad=0;i_ad<7;i_ad++){
		data[i_ad] = volt_photo[i_ad];
//		int	i_ad=0;
		before[i_ad] = average[i_ad];
		bef2[i_ad] = ave2[i_ad];
		average[i_ad] = intAverager[i_ad].update(data[i_ad]);
		ave2[i_ad] = intAve2[i_ad].update(average[i_ad]);
		if(ave2[i_ad]-bef2[i_ad]>0.00002){
		//if(average[i_ad]-before[i_ad]>0.00022){
			//printf("%f,%fHIT\n",average,before);
			change_flg(i_ad);
		}
	}

	before[7] = data[7];
	data[7] = volt_fsr;
	bef2[7] = ave2[7];
	average[7] = fsAve.update(data[7]);
	ave2[7] = fsAve.update(average[7]);
//		printf("fsr, %f\n",ave2[7]-bef2[7]);
	if(ave2[7]-bef2[7]>0.5){
		
		printf("===========================hit\n");
		/*
		gettimeofday(&nowTime,NULL);
		time(&timer);
		t_st = localtime(&timer);
		sprintf(filename,"/mnt/mogura/ad_data_%d%d%d-%d%d%d.csv",
		1990+(int)t_st->tm_year,(int)t_st->tm_mon,(int)t_st->tm_mday,(int)t_st->tm_hour,(int)t_st->tm_min,(int)t_st->tm_sec);
		*/

	}

	data[8] = (float)(t_st->tm_min);
	data[9] = (float)(t_st->tm_sec);
	data[10] = (float)nowTime.tv_usec;

	fprintf(fp_ad,"%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%f,\n",
		data[0],data[1],
		data[2],data[3],
		data[4],data[5],
		data[6],data[7],
		(int)t_st->tm_min,(int)t_st->tm_sec,data[10]);
		//data[0],data[1],data[2],data[3],data[4]);
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

void* mogura::led(void* arg){
	int i2c;    // ファイルディスクリプタ
	char *i2cFileName = "/dev/i2c-1"; // I2Cデバイスのパス（古いものはi2c-0）
	int driverAddress = 0x40;

	if ((i2c = open(i2cFileName, O_RDWR)) < 0)
	{
		printf("Faild to open i2c port\n");
		exit(1);
	}

	if (ioctl(i2c, I2C_SLAVE, driverAddress) < 0)
	{
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}

	Ada_ServoDriver servo(i2c);
	servo.reset();
	servo.setPWMFreq(SERVO_CONTROL_FREQUENCY);
 
	int ret;
	while(1){
		for(int i_ad=0;i_ad<7;i_ad++){
			if(hitflg[i_ad]==1){
				servo.setServoPulse(i_ad+8,4096);
				//ret = system("python /home/pi/prog/Adafruit_Python_PCA9685/examples/led_on.py");
				//usleep(50000);
				change_flg(i_ad);
				ret = system("aplay strike1.wav");
				servo.setServoPulse(i_ad+8,0);
				//usleep(50000);
				//ret = system("python /home/pi/prog/Adafruit_Python_PCA9685/examples/led_off.py");
			}
		}
	}
}

//void* imu_test(void* arg){
void* mogura::imu_test(void* arg){

	time_t timer;
	struct tm *t_st = localtime(&timer);
	char filename[100];

	struct timeval nowTime;
	gettimeofday(&nowTime,NULL);
	time(&timer);
	t_st = localtime(&timer);
	sprintf(filename,"/mnt/mogura/imu_data_%d%d%d-%d%d%d.csv",
	1990+(int)t_st->tm_year,(int)t_st->tm_mon,(int)t_st->tm_mday,(int)t_st->tm_hour,(int)t_st->tm_min,(int)t_st->tm_sec);
//		printf("fsr volt%f\n",volt_val);

	FILE *fp_imu;
	fp_imu = fopen(filename,"w");

	float ax,ay,az;
	float gx,gy,gz;
	float mx,my,mz;
	int ref;

	MPU9250 mog_mpu;
	mog_mpu.initialize();
	ref=mog_mpu.testConnection();

	float im_data[9];

	while(1){

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

		struct timeval nowTime;
		gettimeofday(&nowTime,NULL);
		time(&timer);
		t_st = localtime(&timer);
//		printf("fsr volt%f\n",volt_val);

		im_data[0] = ax;
		im_data[1] = ay;
		im_data[2] = az;
		im_data[3] = gx;
		im_data[4] = gy;
		im_data[5] = gz;
		im_data[6] = (float)(t_st->tm_min);
		im_data[7] = (float)(t_st->tm_sec);
		im_data[8] = (float)nowTime.tv_usec;

		//printf("%f,%f,%f,",ax,ay,az);
		//printf("%f,%f,%f\n",gx,gy,gz);
		//printf("%f,%f,%f\n",mx,my,mz);

		fprintf(fp_imu,"%f,%f,%f,%f,%f,%f,%d,%d,%f\n",
			im_data[0],im_data[1],im_data[2],im_data[3],im_data[4],im_data[5],
			t_st->tm_min,t_st->tm_sec,im_data[8]);
			//ax,ay,az,gx,gy,gz,mx,my,mz);
	}
	fclose(fp_imu);

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
	mogura mogu;

	gettimeofday(&stTime,NULL);

	pthread_t thr_sv;
	pthread_t thr_led;
	pthread_t thr_imu;
	pthread_t thr_fsr;

	pthread_create(&thr_sv, NULL, mogu.servo_test,NULL);
	pthread_create(&thr_led, NULL, mogu.led,NULL);
	pthread_create(&thr_imu, NULL, mogu.imu_test,NULL);
	pthread_create(&thr_fsr, NULL, mogu.fsr_test,NULL);

	pthread_join(thr_sv,NULL);

	pthread_join(thr_led,NULL);
	pthread_join(thr_imu,NULL);
	pthread_join(thr_fsr,NULL);

	pthread_mutex_destroy(&mutex);

}
