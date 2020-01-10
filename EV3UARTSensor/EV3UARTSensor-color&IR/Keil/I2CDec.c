#include "I2CDev.h"
//========================================================================================================
bit I2C_Reset_Flag;
void Init_I2C(void)
{
    P13_OPENDRAIN_MODE;          // Modify SCL pin to Open drain mode. don't forget the pull high resister in circuit
    P14_OPENDRAIN_MODE;          // Modify SDA pin to Open drain mode. don't forget the pull high resister in circuit
    /* Set I2C clock rate */
    I2CLK = I2C_CLOCK; 
    /* Enable I2C */
    set_I2CON_I2CEN;                                   
}
//========================================================================================================
void I2C_SI_Check(void)
{
	if (I2STAT == 0x00)
	{
		I2C_Reset_Flag = 1;
		set_I2CON_STO;
		SI = 0;
		if(SI)
		{
			clr_I2CON_I2CEN;
			set_I2CON_I2CEN;
		}   
	}  
}

void I2C_Read(UINT8 REG,char* u8DAT,UINT8 len)
{
    UINT8 u8Count=0;
	  /* Step1 */
    set_I2CON_STA;                                /* Send Start bit to I2C EEPROM */
    clr_I2CON_SI;
    while (!SI);
    if (I2STAT != 0x08)                     /* 0x08:  A START condition has been transmitted*/
    {
        I2C_Reset_Flag = 1;
        //printf("\nI2C 'Send STA' error");
        goto Read_Error_Stop;
    }

    /* Step2 */
    I2DAT = (EEPROM_SLA | EEPROM_WR);       /* Send (SLA+W) to EEPROM */
    clr_I2CON_STA;                                /* Clear STA and Keep SI value in I2CON */    
    clr_I2CON_SI;
    while (!SI);
    if (I2STAT != 0x18)                     /* 0x18: SLA+W has been transmitted; ACK has been received */              
    {
        I2C_Reset_Flag = 1;
        //printf("\nI2C 'Send SLA+W' error");
        goto Read_Error_Stop;
    }
    /* Step4 */
    I2DAT = REG;             /* Send I2C EEPROM's Low Byte Address */
    clr_I2CON_SI;
    while (!SI);
    if (I2STAT != 0x28)                     /* 0x28:  Data byte in S1DAT has been transmitted; ACK has been received */             
    {
        I2C_Reset_Flag = 1;
        //printf("\nI2C 'Send I2C Low Byte Address' error");
        goto Read_Error_Stop;
    }

    /* Step5 */
    set_I2CON_STA;                                /* Repeated START */
    clr_I2CON_SI; 
    while (!SI);
    if (I2STAT != 0x10)                     /* 0x10: A repeated START condition has been transmitted */
    {
        I2C_Reset_Flag = 1;
        //printf("\nI2C 'Send STA' error");
        goto Read_Error_Stop;
    }

    /* Step6 */
    clr_I2CON_STA;                                /* Clear STA and Keep SI value in I2CON */
    I2DAT = (EEPROM_SLA | EEPROM_RD);       /* Send (SLA+R) to EEPROM */
    clr_I2CON_SI;
    while (!SI);
    if (I2STAT != 0x40)                     /* 0x40:  SLA+R has been transmitted; ACK has been received */              
    {
        I2C_Reset_Flag = 1;
        //printf("\nI2C 'Send SLA+R' error");
        goto Read_Error_Stop;
    }
    /* Step7 */                             /* Verify I2C EEPROM data */
    for (u8Count = 0; u8Count <len; u8Count++)
    {
        set_I2CON_AA;                             /* Set Assert Acknowledge Control Bit */
        clr_I2CON_SI;
        while (!SI);
        if (I2STAT != 0x50)                 /* 0x50:Data byte has been received; NOT ACK has been returned */              
        {
            I2C_Reset_Flag = 1;
            //printf("\nI2C 'No Ack' error");
            goto Read_Error_Stop;
        }       
        u8DAT[u8Count] = I2DAT; 
				////printf("%X ",u8DAT[u8Count]);
				//UART_Send_Data(UART0,u8DAT[u8Count]);
				
    }

    /* Step8 */
    clr_I2CON_AA;                                 /* Send a NACK to disconnect 24xx64 */
    clr_I2CON_SI;
    while (!SI);
    if (I2STAT != 0x58)                     /* 0x58:Data byte has been received; ACK has been returned */
    {
        I2C_Reset_Flag = 1;
        //printf("\nI2C 'Ack' error");
        goto Read_Error_Stop;
    }
    
  /* Step9 */    
    clr_I2CON_SI;
    set_I2CON_STO;
    while (STO)                        /* Check STOP signal */
    {
      I2C_SI_Check();
      if (I2C_Reset_Flag)
        goto Read_Error_Stop;
    }
    
Read_Error_Stop: 
    if (I2C_Reset_Flag)
    {
        I2C_SI_Check();
        //printf("\nI2C Read error, test stop");
        I2C_Reset_Flag = 0;
    }
}
//========================================================================================================
void I2C_Write(UINT8 REG,UINT8 u8DAT)
{
    set_I2CON_STA;                                /* Send Start bit to I2C EEPROM */         
    clr_I2CON_SI;
    while (!SI);
    if (I2STAT != 0x08)                     /* 0x08:  A START condition has been transmitted*/
    {
        I2C_Reset_Flag = 1;
        //printf("\nI2C 'Send STA' error");
        goto Write_Error_Stop;
    }
    /* Step2 */
    clr_I2CON_STA;                                /* Clear STA and Keep SI value in I2CON */
    I2DAT = EEPROM_SLA | EEPROM_WR;         /* Send (SLA+W) to EEPROM */
    clr_I2CON_SI;
    while (!SI);
    if (I2STAT != 0x18)                     /* 0x18: SLA+W has been transmitted; ACK has been received */             
    {
        I2C_Reset_Flag = 1;
        //printf("\nI2C 'Send SLA+W' error");
        goto Write_Error_Stop;
    }

    /* Step4 */
    I2DAT = REG;             /* Send EEPROM's Low Byte Address */
    clr_I2CON_SI;
    while (!SI);
    if (I2STAT != 0x28)                     /* 0x28:  Data byte in S1DAT has been transmitted; ACK has been received */
    {
        I2C_Reset_Flag = 1;
        //printf("\nI2C 'Send Low byte address' error");
        goto Write_Error_Stop;
    }

    /* Step5 */
                                            /* Write data to I2C EEPROM */
		I2DAT = u8DAT;                      /* Send data to EEPROM */
		clr_I2CON_SI;
		while (!SI);
		if (I2STAT != 0x28)                 /* 0x28:  Data byte in S1DAT has been transmitted; ACK has been received */
		{
				I2C_Reset_Flag = 1;
				//printf("\nI2C 'Write Data' error");
				goto Write_Error_Stop;
		} 
		////printf("write success\n");				
                                            //After STOP condition, a inner EEPROM timed-write-cycle 
                                            //will occure and EEPROM will not response to outside command
                                            /* 0x18: SLA+W has been transmitted; ACK has been received */
    /* Step6 */
    do
    {
        set_I2CON_STO;                            /* Set I2C STOP Control Bit */
        clr_I2CON_SI;
        while (STO)                        /* Check STOP signal */
        {
          I2C_SI_Check();
          if (I2C_Reset_Flag)
            goto Write_Error_Stop;
        }
        
        set_I2CON_STA;                            /* Check if no ACK is returned by EEPROM, it is under timed-write cycle */
        clr_I2CON_SI;
        while (!SI);
        if (I2STAT != 0x08)                 /* 0x08:  A START condition has been transmitted*/
        {
            I2C_Reset_Flag = 1;
            //printf("\nI2C 'Wait Ready' error");
            goto Write_Error_Stop;
        }

        clr_I2CON_STA;                            /* Clear STA and Keep SI value in I2CON */
        I2DAT = (EEPROM_SLA | EEPROM_WR);   /* Send (SLA+W) to EEPROM */
        clr_I2CON_SI;
        while (!SI);
    }while(I2STAT != 0x18);

    /* Step7 */
    set_I2CON_STO;                                /* Set STOP Bit to I2C EEPROM */
    clr_I2CON_SI;
    while (STO)                        /* Check STOP signal */
    {
      I2C_SI_Check();
      if (I2C_Reset_Flag)
        goto Write_Error_Stop;
    }
    
Write_Error_Stop: 
    if (I2C_Reset_Flag)
    {
        I2C_SI_Check();
        I2C_Reset_Flag  = 0;
        //printf("\nI2C Write error, test stop");
    }
}
//========================================================================================================


