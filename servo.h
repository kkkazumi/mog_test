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
