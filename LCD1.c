//mainTxcode.c
#include <lpc21xx.h>    
//#include "types.h"    
//#include "can.h"
#include"can_defines.h"    
#include "lcd.h"
#include "delay.h"
#include "ds18b20.h"
#include"Interrupts.h"
#include"defines.h"
#include"lcd_defines.h"
volatile u32 eint0_flag,eint1_flag;
u8 previous_value = 255;
#define LED 4
//u32 f_t_cnt=0;
//u8 f=0;
int main(void)   
{   
      S32 temp=15;
	unsigned char tp,tpd;
	struct CAN_Frame txFrame,rxFrame; //i=sizeof(txFrame);    
       	txFrame.vbf.RTR=0; //data frame   
	txFrame.vbf.DLC=1;
	Init_CAN1();
	Enable_EINT0();
	Enable_EINT1(); 
	LCD_Init();//LCD initialization
	Write_CMD_LCD(0x80);//selecting starting line and staring pos
    Write_CMD_LCD(GOTO_LINE1_POS0+5);
	Write_str_LCD("CAR INFO");
    Write_str_LCD("");
	//Write_str_LCD("**vector**");
    delayMS(1000);
    Write_CMD_LCD(0xC0);
	IODIR0|=(0xf<<LED);  
    IOPIN0|=(0xf<<LED); 
	eint1_flag=0;
	eint0_flag=0;

	while(1)   
        {  
    		temp=ReadTemp();	//READING TEMPERATURE FROM DS18B20 USING 1-WIRE PROTOCAL
	   /*  CAN1_Rx(&rxFrame);
		  if(rxFrame.ID==1)
	  	{
			//print
			Write_CMD_LCD(0xC0+4);
			Write_CMD_LCD(GOTO_LINE2_POS0+15);
			Write_int_LCD(rxFrame.Data1);
		    //Write_CMD_LCD(GOTO_LINE2_POS0+15);
			Write_str_LCD("% ");
		}*/ 
		if(temp==-1)
		{
		   Write_CMD_LCD(0xC0);
		   Write_str_LCD("                ");
		   Write_CMD_LCD(0xC0);
		   Write_str_LCD("Sensor fails");
		} 
		else
		{	
		tp  = temp>>4;	  //GETTING INTEGER PART
		tpd=(temp&0x08)?0x35:0x30;//GETTING FRACTIONAL PART
		Write_CMD_LCD(0xC0);
		Write_CMD_LCD(0xC0);
 		Write_str_LCD("Tem=");
		Write_int_LCD(tp);
		Write_DAT_LCD('.');
		Write_DAT_LCD(tpd);
		Write_CMD_LCD(GOTO_LINE2_POS0+8);
		Write_DAT_LCD(0xDF);
		Write_str_LCD("C");	  
		  //else
		  //{
	     if((eint0_flag==1) && (eint1_flag==0))
	      {  
			txFrame.ID=4;
            txFrame.Data1='L';
		   Write_CMD_LCD(GOTO_LINE1_POS0+1);
			Write_str_LCD("L");
			//Write_str_LCD("   ");
			CAN1_Tx(txFrame);
			//Write_str_LCD(" ");	
	  	}
		else if(eint0_flag>1)
		{
		        eint0_flag=0;
				 Write_CMD_LCD(GOTO_LINE1_POS0+1);
				Write_str_LCD(" ");
			     txFrame.ID=4;
                        txFrame.Data1=0;
                        CAN1_Tx(txFrame);
		}
        else if((eint1_flag==1) && (eint0_flag==0))
	    { 
                 txFrame.Data1='R';
			     txFrame.ID=5;
				 Write_CMD_LCD(GOTO_LINE1_POS0+17);
		     	Write_str_LCD("R");
				Write_str_LCD("   ");
                 CAN1_Tx(txFrame);
				// Write_str_LCD(" ");
		  }
	      else if(eint1_flag>1)
		  {
			      eint1_flag=0;
				   Write_CMD_LCD(GOTO_LINE1_POS0+17);
			       Write_str_LCD(" ");
			      txFrame.Data1=0;
                             txFrame.ID=5;
                             CAN1_Tx(txFrame);
		  }	
	 }
		/*if(C1GSR & RBS_BIT_READ)
		  {	  
		    CAN1_Rx(&rxFrame);
		     if(rxFrame.ID==1)
		     {
	 		   //CharLCD('B');
		       f_t_cnt=0;
		        f=0;
		        //CmdLCD(0x01);
                Write_CMD_LCD(GOTO_LINE2_POS0);
                Write_str_LCD("FUEL:");
		        Write_CMD_LCD(GOTO_LINE2_POS0+15);
		        Write_int_LCD(rxFrame.Data1);
		        Write_str_LCD("%");
		        delayMS(100);
		       }
		       // can message not received 
		      else
		      { 
		       //no can data-increase timeout
		       f_t_cnt++;
		       if(f_t_cnt>10 && f==0)
		       {
		       //show N/A
		       Write_CMD_LCD(GOTO_LINE2_POS0);
               Write_str_LCD("FUEL:N/A");
		        //clear the percent area
		         Write_CMD_LCD(GOTO_LINE2_POS0+15);
		         Write_CMD_LCD(0x01);
		        f=1;
		       }
             } 
			}*/		 
       if(C1GSR & RBS_BIT_READ)
		  {	  
		    CAN1_Rx(&rxFrame);
		    if(rxFrame.ID==1)
	  	   {
		   if(previous_value != rxFrame.Data1)
		   {
		   previous_value = rxFrame.Data1;
			//print
			Write_CMD_LCD(0xC0+4);
			Write_CMD_LCD(GOTO_LINE2_POS0+11);
			Write_str_LCD("FUEL:");
		 	Write_CMD_LCD(GOTO_LINE2_POS0+16);
				Write_str_LCD("   ");
			Write_CMD_LCD(GOTO_LINE2_POS0+16);
			Write_int_LCD(rxFrame.Data1);
		    Write_CMD_LCD(GOTO_LINE2_POS0+19);
			Write_str_LCD("%");
	   	   }
		   }
		}   
 }
}
