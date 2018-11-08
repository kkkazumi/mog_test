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

class mog_servo
{
  public:
    void setup();
    void move(int angle);
};

void mog_servo::setup(){
	//motor setting
	pinMode(MOTORPWM,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(400);
	pwmSetRange(1024);
}

void mog_servo::move(int angle){
  pwmWrite(MOTORPWM,angle);
  sleep(1);
}
 
int main(int argc, char const* argv[]){
  mog_servo mogura;
  if(wiringPiSetupGpio()==-1){
      std::cout<<"cannot setup gpio."<<std::endl;
      return 1;
  }

	char command[256];
  mogura.setup();
 
	int num=0;
	while(num<10){
		std::cout<<num<<std::endl;
    mogura.move(24);
    mogura.move(115);
		num++;
	}
	return 0;
}
