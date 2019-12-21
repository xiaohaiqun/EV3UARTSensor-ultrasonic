#include "MessageSend.h"
void get_bytes(unsigned long val, uint8_t* bb) 
{
  uint8_t i=0;
	for(i=0;i<4;i++) {
    bb[i] = (val >> (i *8)) & 0xff;
  }
}

void sendInfoMessage(uint8_t cmd, uint8_t type, 
uint8_t* Data, uint8_t len) {
  uint8_t checksum = 0xff ^ cmd;
	uint8_t i=0;
  UART_Send_Data(UART1,cmd);
  checksum ^= type;
  UART_Send_Data(UART1,type);
  for(i=0;i<len;i++) {
    checksum ^= Data[i];
    UART_Send_Data(UART1,Data[i]);
  }
  UART_Send_Data(UART1,checksum);
}

void sendInfoLLLLHHHHMessage(uint8_t cmd, uint8_t type, 
uint8_t* DataL,uint8_t* DataH, uint8_t len) {
  uint8_t checksum = 0xff ^ cmd;
	uint8_t i=0;
  UART_Send_Data(UART1,cmd);
  checksum ^= type;
  UART_Send_Data(UART1,type);
  for(i=0;i<len;i++) {
    checksum ^= DataL[i];
    UART_Send_Data(UART1,DataL[i]);
  }
	for(i=0;i<len;i++) {
    checksum ^= DataH[i];
    UART_Send_Data(UART1,DataH[i]);
  }
  UART_Send_Data(UART1,checksum);
}

void sendInfoWithPaddedMessage(uint8_t cmd, uint8_t type, 
uint8_t* Data, uint8_t len, uint8_t padedLenth) {
  uint8_t checksum = 0xff ^ cmd;
	uint8_t i=0;
  UART_Send_Data(UART1,cmd);
  checksum ^= type;
  UART_Send_Data(UART1,type);
  for(i=0;i<len;i++) {
    checksum ^= Data[i];
    UART_Send_Data(UART1,Data[i]);
  }
	for(i=0;i<padedLenth;i++)
		UART_Send_Data(UART1,0x00);
  UART_Send_Data(UART1,checksum);
}

void sendCMDMessage(uint8_t cmd) {
  uint8_t checksum = 0xff ^ cmd;
  UART_Send_Data(UART1,cmd);
  UART_Send_Data(UART1,checksum);
}
 
void sendDataMessage(uint8_t cmd, uint8_t* Data, uint8_t len) {
  uint8_t checksum = 0xff ^ cmd;
	uint8_t i=0;
  UART_Send_Data(UART1,cmd);
  for(i=0;i<len;i++) {
    checksum ^= Data[i];
    UART_Send_Data(UART1,Data[i]);
  }
  UART_Send_Data(UART1,checksum);
}

void sendTypeMessage(uint8_t cmd, uint8_t Data) {
  uint8_t bb[1];
  bb[0] = Data;
  sendDataMessage(cmd,bb,1);
}


