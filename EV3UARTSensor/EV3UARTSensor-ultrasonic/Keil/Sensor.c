#include "Sensor.h"
#include "MessageSend.h"
#include "DistanceMeasure.h"
#define MY_TYPE 30
#define MY_NAME "US-DIST-CM"
#define SPEED 57600

#define NOP() {_nop_();_nop_();_nop_();_nop_();}

unsigned long last_nack = 0; //上一次收到NACK或ACK的时间；
unsigned long last_message=0;  //上次发送消息的时间

void reset(){
	while(1)
	{
    uint8_t mode=0;
		uint8_t bb[4];
    uint8_t speed[4];
		float L=0,H=0;
		long m=0;
		uint8_t cmd=0;
		UART_Open(24000000,UART1_Timer3,2400);	
    sendTypeMessage(CMD_TYPE, MY_TYPE);
    bb[0] = 6; // modes = 1
    bb[1] = 2; // views = 1
    sendDataMessage(CMD_MODES, bb, 2);
    get_bytes(SPEED, speed);
    sendDataMessage(CMD_SPEED, speed, 4);
		NOP();
		///////////////////////////M0DE0 Message//////////////////////////////////
		mode=0x00;
    sendInfoWithPaddedMessage(CMD_INFO | (4 << CMD_LLL_SHIFT | mode), 0, (uint8_t*) MY_NAME, 10, 6);
		L=0,H=2550;
		sendInfoLLLLHHHHMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode), 0x01,(uint8_t*)&L ,(uint8_t*)&H , 4);//rawLLLL,rawHHHH
		
		L=0,H=255;
		sendInfoLLLLHHHHMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode ), 0x03,(uint8_t*)&L ,(uint8_t*)&H , 4);//SILLLL,SIHHHH
		
		sendInfoWithPaddedMessage(CMD_INFO | (3 << CMD_LLL_SHIFT | mode), 0x04, "cm", 2, 6);//symbol
		
		bb[0] = 1; // sets
    bb[1] = 1; // format Data16
    bb[2] = 5; // figures
    bb[3] = 1; // decimals
    sendInfoMessage(CMD_INFO | (2 << CMD_LLL_SHIFT | mode), 0x80, bb, 4);
    //////////////////////////////////////////////////////////////////////////////////////////////////////
		//More MODE....
		UART_Send_Data(UART1,BYTE_ACK);
    m = timetick;//发送ACK后传感器开始计时，超过1s未回复则传感器复位。
		
    while(!riflag && timetick - m <1000);
    if(riflag) {
		  riflag=0;
      cmd =UART1Receive; //uart_receive_data;
			UART_Send_Data(UART0, cmd);
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
extern short int distance;

typedef union{
	uint16_t d;
	char c[2];
}Distance;
Distance D;
void loop(){
	while(1)
	{
		if(timetick - last_nack >999) 
			reset();
		if(riflag){
			riflag=0;
			cmd = UART1Receive;
			last_nack = timetick;
			switch(cmd)
			{
				case BYTE_NACK:
					last_nack = timetick;
					sendDataMessage(CMD_DATA | (1 << CMD_LLL_SHIFT),D.c, 2);
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
			DistanceMeasure();
			//distance++;
			D.d=distance;
			n=D.c[1];
			D.c[1]=D.c[0];
			D.c[0]=n;
			sendDataMessage(CMD_DATA | (1 << CMD_LLL_SHIFT),D.c, 2); // mode 0, length 2
			last_message = timetick;
		}
	}
}