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

	int flg=S_UP;

	mogura.move(MOG_UP);


	while(1){
		count++;
		srand((unsigned)time(NULL));

		//mog_photo.read_val(out_ch0,ch0_data,3);
		//volt_val = mog_photo.get_volt(out_ch0,ch0_data);

		if(count==100){
				ran = rand();
				std::cout<<ran<<std::endl;
				count=0;

				if(ran%7==0){
					std::cout<<"................."<<ran%7<<std::endl;
				}else{
					std::cout<<"................."<<rand()%100<<std::endl;
				}
		}

/*
		if(volt_val<PHOTO_THRE && volt_val>0.02){
				if(flg==S_UP){
						std::cout<<"............................1"<<std::endl;
				}
		}else if(volt_val>PHOTO_THRE){
				if(flg==S_UP){
						std::cout<<"............................2"<<std::endl;
						mogura.move(MOG_DOWN);
						flg=S_DOWN;
				}else if(flg==S_DOWN){
						std::cout<<"............................3"<<std::endl;
				}
		}
*/

		before_volt=volt_val;

	}
}
