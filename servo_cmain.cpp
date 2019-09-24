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
 
 //
 if ((i2c = open(i2cFileName, O_RDWR)) < 0)
 {
  printf("Faild to open i2c port\n");
  exit(1);
 }
 
 //
 if (ioctl(i2c, I2C_SLAVE, driverAddress) < 0)
 {
  printf("Unable to get bus access to talk to slave\n");
  exit(1);
 }
 
 Ada_ServoDriver servo(i2c);
 
 servo.reset();
 
 usleep(100000);
 
 // サーボ制御パルス周波数の設定。
 servo.setPWMFreq(SERVO_CONTROL_FREQUENCY);
 
 // サーボをセンタ位置へ。
 servo.setServoPulse(0, SERVO_CENTER_PULSE_WIDTH_US);
 servo.setServoPulse(1, SERVO_CENTER_PULSE_WIDTH_US);
 servo.setServoPulse(2, SERVO_CENTER_PULSE_WIDTH_US);
 servo.setServoPulse(3, SERVO_CENTER_PULSE_WIDTH_US);
 
 
 sleep(1);
 
 // とりあえず、適当に動かしてみる。
 servo.setServoPulse(1, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 4));
 
 while(true)
 {
  
  servo.setServoPulse(0, (SERVO_CENTER_PULSE_WIDTH_US + SERVO_RANGE_PULSE_WIDTH_US / 4));
  servo.setServoPulse(3, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 4));
  
  usleep(500000);
  
  servo.setServoPulse(1, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 4));
  servo.setServoPulse(2, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 4));
  
  usleep(500000);
  
  servo.setServoPulse(0, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 4));
  servo.setServoPulse(3, SERVO_CENTER_PULSE_WIDTH_US);
  
  usleep(500000);
  
  servo.setServoPulse(1, SERVO_CENTER_PULSE_WIDTH_US);
  servo.setServoPulse(2, (SERVO_CENTER_PULSE_WIDTH_US + SERVO_RANGE_PULSE_WIDTH_US / 4));
  
  usleep(500000);
 }
 
 return 0;
}
