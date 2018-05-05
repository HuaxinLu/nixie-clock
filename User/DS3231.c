#include "DS3231.h"
#include "bsp_i2c_gpio.h" 
_calendar_obj calendar;
#define DS3231_WriteAddress 0xD0   
#define DS3231_ReadAddress  0xD1
//粗延时函数，微
void delay_us(unsigned int time)
{    
   unsigned int i=0;  
   while(time--)
   {
      i=20;  //自己定义
      while(i--) ;    
   }
}
//毫秒级的延时
void delay_ms(unsigned int time)
{    
   unsigned int i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
}
unsigned char BCD2HEX(unsigned char val)
{
    unsigned char i;
    i= val&0x0f;
    val >>= 4;
    val &= 0x0f;
    val *= 10;
    i += val;
    
    return i;
}

unsigned int B_BCD(unsigned char val)
{
  unsigned char i,j,k;
  i=val/10;
  j=val%10;
  k=j+(i<<4);
  return k;
}


void I2cByteWrite(unsigned char addr,unsigned char bytedata)
{
  i2c_Start();
  delay_us(5);
  i2c_SendByte(DS3231_WriteAddress);
  i2c_WaitAck();
  delay_us(5);
  i2c_SendByte(addr);
  i2c_WaitAck();
  delay_us(5);
  i2c_SendByte(bytedata);
  i2c_WaitAck();
  delay_us(5);
  i2c_Stop();
}	


unsigned char I2cByteRead(unsigned char addr)
{
  unsigned char Dat=0;
  
  i2c_Start();
  i2c_SendByte(DS3231_WriteAddress);
  i2c_WaitAck();
  delay_us(5);
  i2c_SendByte(addr);
  i2c_WaitAck();
  delay_us(5);
  i2c_Start();
  i2c_SendByte(DS3231_ReadAddress);
  i2c_WaitAck();
  delay_us(5);
  Dat=i2c_ReadByte();
  i2c_Stop();
  return Dat;
} 



void DS3231_Init(void)
{
	i2c_CfgGpio();
	I2cByteWrite(0x0e,0);
	delay_ms(2); 
    I2cByteWrite(0x0f,0x0);
	delay_ms(2); 
}
void DS3231_Get(void)
{
  calendar.w_year=I2cByteRead(0x06);  
  calendar.w_month=I2cByteRead(0x05);  
  calendar.w_date=I2cByteRead(0x04); 
  calendar.hour=I2cByteRead(0x02);  
  calendar.min=I2cByteRead(0x01);
  calendar.sec=I2cByteRead(0x00);
}

void DS3231_Set(unsigned char yea,unsigned char mon,unsigned char da,unsigned char hou,unsigned char min,unsigned char sec)
{
  unsigned char temp=0;
  
  temp=B_BCD(yea);
  I2cByteWrite(0x06,temp);
  
  temp=B_BCD(mon);
  I2cByteWrite(0x05,temp);
   
  temp=B_BCD(da);
  I2cByteWrite(0x04,temp);
  
  temp=B_BCD(hou);
  I2cByteWrite(0x02,temp);
  
  temp=B_BCD(min);
  I2cByteWrite(0x01,temp);
  
  temp=B_BCD(sec);
  I2cByteWrite(0x00,temp);
}


void get_show_time(void)
{


calendar.w_year=I2cByteRead(0x06);  
calendar.w_year=BCD2HEX(calendar.w_year);


calendar.w_month=I2cByteRead(0x05); 
calendar.w_month=BCD2HEX(calendar.w_month);


calendar.w_date=I2cByteRead(0x04);  
calendar.w_date=BCD2HEX(calendar.w_date);
 

calendar.hour=I2cByteRead(0x02); 
calendar.hour&=0x3f;                   
calendar.hour=BCD2HEX(calendar.hour);


calendar.min=I2cByteRead(0x01);
calendar.min=BCD2HEX(calendar.min);
  
  
calendar.sec=I2cByteRead(0x00);
calendar.sec=BCD2HEX(calendar.sec);
}


