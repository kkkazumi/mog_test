#include <stdio.h>
#include <string>
#include <iostream>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
 
#include <wiringPi.h>
 
#define MOTORPWM 18
#define POW 1024
 
int main(int argc, char const* argv[])
{
  if(wiringPiSetupGpio()==-1){
      std::cout<<"cannot setup gpio."<<std::endl;
      return 1;
  }
	//motor setting
	pinMode(MOTORPWM,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(400);
	pwmSetRange(1024);
 
	char command[256];
 
	int num=0;
	while(num<10){
		std::cout<<num<<std::endl;
		pwmWrite(MOTORPWM,24);
    sleep(1);
		pwmWrite(MOTORPWM,115);
    sleep(1);
		num++;
	}
	return 0;
}
