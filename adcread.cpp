#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "SPIdev.h"
#include "adcread.h"

mog_adc::mog_adc()
{
}

unsigned int mog_adc::WriteReg( uint8_t WriteAddr, uint8_t WriteData )
{
    unsigned int temp_val;

    unsigned char tx[2] = {WriteAddr, WriteData};
		unsigned char rx[2] = {0};

		SPIdev::transfer("/dev/spidev0.1", tx, rx, 2);

    return rx[1];
}

//-----------------------------------------------------------------------------------------------
unsigned int mog_adc::ReadReg( uint8_t WriteAddr, uint8_t WriteData )
{
    return WriteReg(WriteAddr | READ_FLAG, WriteData);
}

//-----------------------------------------------------------------------------------------------

void mog_adc::ReadRegs( uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes )
{
    unsigned int  i = 0;

    unsigned char tx[255] = {0};
		unsigned char rx[255] = {0};

		tx[0] = ReadAddr | READ_FLAG;

		SPIdev::transfer("/dev/spidev0.1", tx, rx, Bytes + 1);

    for(i=0; i<Bytes; i++)
    	ReadBuf[i] = rx[i + 1];

    usleep(50);
}

void mog_adc::read_photo(int ch)
{
    uint8_t response[3];
    int16_t bit_data;
    float data;
    int i;

//		unsigned char out_ch0 = 0b00000110;
		uint8_t out_ch0 = 0x00;

    //ReadRegs(MPUREG_ACCEL_XOUT_H,response,6);
    ReadRegs(out_ch0, response,1);
    for(i=0; i<3; i++) {
        data=(float)bit_data;
        //accelerometer_data[i]=data/acc_divider;
        photo_data=data;
    }

}

//float mog_adc::get_volt(int ch, float *photo){
void mog_adc::get_volt(int ch, float *photo){
 	int val;
	float volt_value;
	
	read_photo(ch);
	val = photo_data;
 
	volt_value	=	5.0*(float)val/4095.0;
	//return volt_value;
	*photo = volt_value;
}

/*
float mog_adc::init(){
}
bool MPU9250::initialize(int sample_rate_div, int low_pass_filter)
{
    uint8_t i = 0;
    uint8_t MPU_Init_Data[MPU_InitRegNum][2] = {
        //{0x80, MPUREG_PWR_MGMT_1},     // Reset Device - Disabled because it seems to corrupt initialisation of AK8963
        {0x01, MPUREG_PWR_MGMT_1},     // Clock Source
        {0x00, MPUREG_PWR_MGMT_2},     // Enable Acc & Gyro
        {low_pass_filter, MPUREG_CONFIG},         // Use DLPF set Gyroscope bandwidth 184Hz, temperature bandwidth 188Hz
        {0x18, MPUREG_GYRO_CONFIG},    // +-2000dps
        {0x08, MPUREG_ACCEL_CONFIG},   // +-4G
        {0x09, MPUREG_ACCEL_CONFIG_2}, // Set Acc Data Rates, Enable Acc LPF , Bandwidth 184Hz
        {0x30, MPUREG_INT_PIN_CFG},    //
        //{0x40, MPUREG_I2C_MST_CTRL},   // I2C Speed 348 kHz
        //{0x20, MPUREG_USER_CTRL},      // Enable AUX
        {0x20, MPUREG_USER_CTRL},       // I2C Master mode
        {0x0D, MPUREG_I2C_MST_CTRL}, //  I2C configuration multi-master  IIC 400KHz

        {AK8963_I2C_ADDR, MPUREG_I2C_SLV0_ADDR},  //Set the I2C slave addres of AK8963 and set for write.
        //{0x09, MPUREG_I2C_SLV4_CTRL},
        //{0x81, MPUREG_I2C_MST_DELAY_CTRL}, //Enable I2C delay

        {AK8963_CNTL2, MPUREG_I2C_SLV0_REG}, //I2C slave 0 register address from where to begin data transfer
        {0x01, MPUREG_I2C_SLV0_DO}, // Reset AK8963
        {0x81, MPUREG_I2C_SLV0_CTRL},  //Enable I2C and set 1 byte

        {AK8963_CNTL1, MPUREG_I2C_SLV0_REG}, //I2C slave 0 register address from where to begin data transfer
        {0x12, MPUREG_I2C_SLV0_DO}, // Register value to continuous measurement in 16bit
        {0x81, MPUREG_I2C_SLV0_CTRL}  //Enable I2C and set 1 byte

    };
    //spi.format(8,0);
    //spi.frequency(1000000);

    for(i=0; i<MPU_InitRegNum; i++) {
        WriteReg(MPU_Init_Data[i][1], MPU_Init_Data[i][0]);
        usleep(100000);  //I2C must slow down the write speed, otherwise it won't work
    }

    set_acc_scale(BITS_FS_16G);
    set_gyro_scale(BITS_FS_2000DPS);

    calib_mag();
    return 0;
}
*/




/*
	uint8_t tx[len][3];
	uint8_t rx[len][3];
	for(int i=0;i<len;i++){
		tx[i][0] = START_BIT | SINGLE_ENDED;
		tx[i][1] = out_ch0[i] << CH_SHIFT;
	}
	*/

	//printf("char = %s\n",ch0_data[2]);
	//val	=	(ch0_data[1]	&	0b00001111)<<8;
	//val	+=	ch0_data[2];


//#define START_BIT (1<<2)
//#define SINGLE_ENDED (1<<1)
//#define CH_SHIFT (6)
//mog_adc::init(){

