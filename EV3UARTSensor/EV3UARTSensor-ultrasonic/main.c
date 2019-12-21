#include "Sensor.h"

#define TH0_INIT        0xA0 
#define TL0_INIT        0x00


/************************************************************************************************************
*    TIMER 0 interrupt subroutine
************************************************************************************************************/
unsigned long timetick=0;
void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    TH0 = TH0_INIT;
    TL0 = TL0_INIT;    
    TF0 = 0;
		timetick++;	  
}

uint8_t UART1Receive=0;
bit riflag=0;

void SerialPort1_ISR(void) interrupt 15 
{
    if (RI_1==1) 
    {                                      
        clr_SCON_1_RI_1;                             
        UART1Receive = SBUF_1;
        riflag =1;
    }
    if(TI_1==1)
    {
        clr_SCON_1_TI_1;                            
    }
}
extern short int DistanceMeasure();
void main (void)
{
    ALL_GPIO_QUASI_MODE;
    P05_PUSHPULL_MODE;    // For I/O toggle display
/* Modify HIRC to 24MHz for UART baud rate deviation not over 1%*/
    MODIFY_HIRC_24();
    /*UART_Open(24000000,UART0_Timer3,9600);//
		ENABLE_UART0_INTERRUPT;                 //For interrupt enable
		ENABLE_UART0_PRINTF;  */
		UART_Open(24000000,UART1_Timer3,2400);//
    ENABLE_UART1_INTERRUPT;                 //For interrupt enable
	
	  ENABLE_TIMER0_MODE1;
    TIMER0_FSYS;

    TH0 = TH0_INIT;
    TL0 = TL0_INIT;
	
		ENABLE_TIMER1_MODE1;
    TIMER1_FSYS_DIV12;
	
    ENABLE_TIMER0_INTERRUPT;                       //enable Timer0 interrupt
	
		P30_INPUT_MODE;
		P30 = 1;
		P05_OPENDRAIN_MODE ;
		///P05_PUSHPULL_MODE;
		//INT0_FALLING_EDGE_TRIG;              //setting trig condition level or edge
		//ENABLE_INT0_INTERRUPT;              //INT0_Enable;
	
    ENABLE_GLOBAL_INTERRUPT;                       //enable interrupts
  
    set_TCON_TR0;                                  //Timer0 run
		reset();
		loop();
		while(1){
				DistanceMeasure();
			Timer1_Delay(24000000,5,1000);
		}
}
  
