#include "MS51_16K.H"
void get_bytes(unsigned long val, uint8_t* bb); 
void sendInfoMessage(uint8_t cmd, uint8_t type, uint8_t* Data, uint8_t len);
void sendInfoLLLLHHHHMessage(uint8_t cmd, uint8_t type, uint8_t* DataL,uint8_t* DataH, uint8_t len);
void sendInfoWithPaddedMessage(uint8_t cmd, uint8_t type, uint8_t* Data, uint8_t len, uint8_t padedLenth);
void sendCMDMessage(uint8_t cmd);
void sendDataMessageWithPaded(uint8_t cmd, uint8_t* Data, uint8_t len, uint8_t padLen);
void sendDataMessage(uint8_t cmd, uint8_t* Data, uint8_t len);
void sendTypeMessage(uint8_t cmd, uint8_t Data);