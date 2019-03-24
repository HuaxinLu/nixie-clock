#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H


#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */
void i2c_CfgGpio(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(unsigned char _ucByte);
unsigned char i2c_ReadByte(void);
unsigned char i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
unsigned char i2c_CheckDevice(unsigned char _Address);

#endif
