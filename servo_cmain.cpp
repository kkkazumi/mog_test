// Ada I2C 16chサーボドライバ

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <unistd.h>

#include "servo_c.h"

int main(int argc, char **argv)
{
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
 
 printf("reset");
 Ada_ServoDriver servo(i2c);
 
 servo.reset();
 
 usleep(100000);
 
 servo.setPWMFreq(SERVO_CONTROL_FREQUENCY);
 
 sleep(1);
 printf("slept\n");
 
 while(true)
 {
 	printf("slept\n");
  
  servo.setServoPulse(9, 4096);
  
  usleep(1000000);
  
  servo.setServoPulse(9, 0);
  
  usleep(1000000);
  
 }
 
 return 0;
}
