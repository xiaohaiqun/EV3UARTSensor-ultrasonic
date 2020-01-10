#include "MS51_16K.H"

//#define TH0_INIT        0xA0 
//#define TL0_INIT        0x00   //0xFFFF-0xA0000,一秒计数一次； 

#define   BYTE_ACK                      0x04
#define   BYTE_NACK                     0x02
#define   CMD_TYPE                      0x40
#define   CMD_MODES                     0x49
#define   CMD_SELECT                    0x43
#define   CMD_SPEED                     0x52
#define   CMD_MASK                      0xC0
#define   CMD_INFO                      0x80
#define   CMD_LLL_MASK                  0x38
#define   CMD_LLL_SHIFT                 3
#define   CMD_MMM_MASK                  0x07
#define   CMD_DATA                      0xc0

extern uint8_t UART1Receive;
extern bit riflag;
extern unsigned long  timetick;  //系统时钟，每1ms +1;

void reset();
void loop();