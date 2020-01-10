#include "Sensor.h"
#include "MessageSend.h"
#include "I2CDev.h"
//#include "DistanceMeasure.h"
#define MY_TYPE 29
#define MY_NAME0 "COLOR-PROXI"
#define MY_NAME1 "IR-PROX"
#define SPEED 57600

#define NOP() {_nop_();_nop_();_nop_();_nop_();}

unsigned long last_nack = 0; //上一次收到NACK或ACK的时间；
unsigned long last_message=0;  //上次发送消息的时间

void reset(){
	P03=0;
	while(1)
	{
    uint8_t mode=0;
		uint8_t bb[4];
    uint8_t speed[4];
		uint32_t L=0,H=0;
		long m=0;
		uint8_t cmd=0;
		UART_Open(24000000,UART1_Timer3,2400);	
    sendTypeMessage(CMD_TYPE, MY_TYPE);
    bb[0] = 2; // modes = 3
    bb[1] = 0; // views = 1
    sendDataMessageWithPaded(CMD_MODES, bb, 2, 0);
    get_bytes(SPEED, speed);
    sendDataMessageWithPaded(CMD_SPEED, speed, 4, 0);
		NOP();
		///////////////////////////M0DE0 Message//////////////////////////////////
		/*mode=0x00;
    sendInfoWithPaddedMessage(CMD_INFO | (4 << CMD_LLL_SHIFT | mode), 0, (uint8_t*) MY_NAME0, 11, 5);
		L=0,H=1024;
		sendInfoLLLLHHHHMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode), 0x01,(uint8_t*)&L ,(uint8_t*)&H , 4);//rawLLLL,rawHHHH
		
		L=0,H=1024;
		sendInfoLLLLHHHHMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode ), 0x03,(uint8_t*)&L ,(uint8_t*)&H , 4);//SILLLL,SIHHHH
		
		sendInfoWithPaddedMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode), 0x04, "Co", 2, 6);//symbol
		
		bb[0] = 5; // sets  有5项，RGBC+proxi
    bb[1] = 1; // format Data16  每一项的数据类型 16位整数
    bb[2] = 4; // figures      要显示的位数，4位（包括小数点）
    bb[3] = 0; // decimals      要显示的小数位数。
    sendInfoMessage(CMD_INFO | (2 << CMD_LLL_SHIFT | mode), 0x80, bb, 4);*/
    //////////////////////////////////////////////////////////////////////////////////////////////////////
		//More MODE....
		
		mode=0x00;
    sendInfoWithPaddedMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode), 0, (uint8_t*) MY_NAME1, 7, 1);
		L=0,H=51266;
		sendInfoLLLLHHHHMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode), 0x01,(uint8_t*)&L ,(uint8_t*)&H , 4);//rawLLLL,rawHHHH
		
		L=0,H=51266;
		sendInfoLLLLHHHHMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode ), 0x03,(uint8_t*)&L ,(uint8_t*)&H , 4);//SILLLL,SIHHHH
		
		sendInfoWithPaddedMessage(CMD_INFO | (2 << CMD_LLL_SHIFT | mode), 0x04, "pct", 3, 1);//symbol
		
		bb[0] = 1; // sets  有1项，IR+proxi
    bb[1] = 0; // format Data16  每一项的数据类型 8位整数
    bb[2] = 3; // figures      要显示的位数，4位（包括小数点）
    bb[3] = 0; // decimals      要显示的小数位数。
    sendInfoMessage(CMD_INFO | (2 << CMD_LLL_SHIFT | mode), 0x80, bb, 4);
		
		/*mode=0x00;
    sendInfoWithPaddedMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode), 0, (uint8_t*) MY_NAME1, 7, 1);
		L=0,H=100;
		sendInfoLLLLHHHHMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode), 0x01,(uint8_t*)&L ,(uint8_t*)&H , 4);//rawLLLL,rawHHHH
		
		L=0,H=100;
		sendInfoLLLLHHHHMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode ), 0x03,(uint8_t*)&L ,(uint8_t*)&H , 4);//SILLLL,SIHHHH
		
		sendInfoWithPaddedMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode), 0x04, "pct", 3, 5);//symbol
		
		bb[0] = 1; // sets  有1项，IR+proxi
    bb[1] = 0; // format Data16  每一项的数据类型 8位整数
    bb[2] = 3; // figures      要显示的位数，4位（包括小数点）
    bb[3] = 0; // decimals      要显示的小数位数。
    sendInfoMessage(CMD_INFO | (2 << CMD_LLL_SHIFT | mode), 0x80, bb, 4);*/

		
		UART_Send_Data(UART1,BYTE_ACK);
    m = timetick;//发送ACK后传感器开始计时，超过1s未回复则传感器复位。
		
    while(!riflag && timetick - m <1000);
    if(riflag) {
		  riflag=0;
      cmd =UART1Receive; //uart_receive_data;
      if(cmd == BYTE_ACK) {
				NOP();
				UART_Open(24000000,UART1_Timer3,SPEED);
				//UART_Send_Data(UART1, 0x00);
				//UART_Send_Data(UART1, 0xFF);				
				NOP();
        last_nack = timetick;
        break;
      }
    }
  }
}
uint8_t cmd;
uint8_t len=0;
uint8_t dataLenth=0;
uint8_t n=0;
//uint8_t Data[16]={0};
//extern 
uint16_t IR16=0;
uint8_t IR8=0;

typedef union{
	uint16_t d;
	char c[2];
}Distance;
Distance D;
uint8_t Data8[10]={33,22,3,4,5,6,7,8,9,0};
UINT8 Data[8];
UINT16 Red=0,Green=0,Blue=0,Dis=0,Clear=0;
void loop(){
	while(1)
	{
		P03=1;
		if(timetick - last_nack >999999) 
			reset();
		if(riflag){
			riflag=0;
			cmd = UART1Receive;
			last_nack = timetick;
			switch(cmd)
			{
				case BYTE_NACK:
					last_nack = timetick;
					//sendDataMessageWithPaded(CMD_DATA | (1 << CMD_LLL_SHIFT),Data, 2);
					break;
				case BYTE_ACK:
					break;
				case CMD_SELECT:
					//SelectedMode=Receive_Data(UART1);
					break;
				default:
					if((UART1Receive&0xC7)==0x44)//write Data ordle
					{
						len=(UART1Receive&0x38)>>3;
						dataLenth=0x02<<len;
						for(n=0;n<dataLenth;n++)
						{
							//Data[n]=Receive_Data(UART1);
						}						
						//To do
					}					
					break;
				}
		}
		if (timetick- last_message >50 ) {
			//I2C_Read (0x94,Data8,2);//IR
			//Data8[0]=0x01;
			//Data8[1]=0x00;
			sendDataMessageWithPaded(CMD_DATA |(1 <<CMD_LLL_SHIFT ),Data8, 1, 1); // mode 0, length 1,padding 1
			last_message = timetick;
			I2C_Read (0x94,Data8,2);//IR
			D.c[0]=Data8[1];
			D.c[1]=Data8[0];
			D.d=D.d/4;
			Data8[1]=D.c[0];
			Data8[0]=D.c[1];
			//Data8[0]=((Data8[1]<<8|Data[0])/4)<<8;
			//Data8[1]=Data8[1]>>2;
		}
	}
}