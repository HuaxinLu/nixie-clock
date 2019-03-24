#ifndef __DS3231_H__
#define __DS3231_H__


typedef struct 
{
	unsigned char hour;
	unsigned char min;
	unsigned char sec;			
	unsigned int w_year;
	unsigned char  w_month;
	unsigned char  w_date;
	unsigned char  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��

extern unsigned char const mon_table[12];	//�·��������ݱ�

void DS3231_Init(void);
void get_show_time(void);      
//unsigned char RTC_Get_Week(u16 year,unsigned char month,unsigned char day);
void DS3231_Set(unsigned char syear,unsigned char smon,unsigned char sday,unsigned char hour,unsigned char min,unsigned char sec);//����ʱ��	

#endif

