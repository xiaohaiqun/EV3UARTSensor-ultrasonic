#include "MS51_16K.H"
#ifndef I2CDEV_H
	#define I2CDEV_H
#define SYS_DIV                 1
#define I2C_CLOCK               14                  /* Setting I2C clock as 400K */

#define EEPROM_SLA              0x92    //0x49 
#define ID_ADDRESS              0x92   //默认0x10
#define EEPROM_WR               0
#define EEPROM_RD               1

#define LED                     P3

#define ERROR_CODE              0x78
#define TEST_OK                 0x00



void Init_I2C(void);
void I2C_SI_Check(void);
void I2C_Read(UINT8 REG,char* u8DAT,UINT8 len);
void I2C_Write(UINT8 REG,UINT8 u8DAT);
#endif