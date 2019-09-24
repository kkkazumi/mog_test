#ifndef SERVO_CHPP
#define SERVO_CHPP
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
// Ada 16ch PWMサーボドライバクラス

/*
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD
*/
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#define SERVO_CONTROL_FREQUENCY 60
#define SERVO_CENTER_PULSE_WIDTH_US 1520
#define SERVO_CENTER_PULSE_WIDTH_US_FUTABA_OLD  1310
#define SERVO_RANGE_PULSE_WIDTH_US 1600


class Ada_ServoDriver
{
public:
 Ada_ServoDriver(int i2c);
 void reset(void);
 void setPWMFreq(float frea);
 void setPWM(uint8_t srvNo, uint16_t onTime, uint16_t offTime);
 void setServoPulse(uint8_t ch, double pulseWidth_us);
 uint8_t read8(uint8_t addr);
 void write8(uint8_t addr, uint8_t d);
 
private:
 uint8_t _i2cAddr;
 int _i2c;
};

#endif// SERVO_CHPP
