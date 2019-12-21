#include "DistanceMeasure.h"

#define TH1_INIT        0x00 
#define TL1_INIT        0x00
typedef union{
	uint16_t d;
	char c[2];
}time;
time T;
short int distance=0;
unsigned long lastTick=0;
uint16_t t_us=0;
extern unsigned long timetick;
uint16_t TH=0,TL=0;
short int DistanceMeasure()
{
	uint8_t t;
	Trig=0;
	NOP();
	Trig=1;
	for(t=0;t<10;t++)
		NOP();	
	Trig=0;
	
	while(!P30&&(timetick>lastTick));
	if(!P30)
		return 0;
	TR1=0;       //停止定时器；
	TH1 = TH1_INIT;
  TL1 = TL1_INIT;
	TR1=1;      //启动定时器;
	while(P30);
	
	TR1=0;      //停止

	T.c[1]=TL1;//数据地位
	T.c[0]=TH1;//数据高位
	
	//T.d=(TH1<<8)|TL1;
	//printf("%u    ",T.d);
	t_us=T.d/2;
	//printf("%u ",t_us);
	distance=t_us*0.17;
	//distance++;
	//printf("%d \n",distance);
	//distance++;
	//printf("distance:%u \n",distance);
	return 1;
}