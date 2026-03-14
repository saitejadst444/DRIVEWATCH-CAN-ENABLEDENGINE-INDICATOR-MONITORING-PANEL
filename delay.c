//main_CANRX_TEST.C
#include <LPC21xx.h>   
//#include"can.h" 
//#include"LCD.h" 
#include"defines.h"
#include"delay.h"
#include"types.h"
#include"can_defines.h"
#define LED 0
u32 left_indicator,right_indicator;
void check_can_receive(void)
{
  struct CAN_Frame rxFrame;
  if(C1GSR & RBS_BIT_READ)
  {
	    CAN1_Rx(&rxFrame);
		if(rxFrame.ID==4) 
		{
		   right_indicator=0;
		   if(rxFrame.Data1=='L')
		   {
		       left_indicator=1;
		   }
		   else
		      {									  
			     left_indicator=0;
				 }

	    }
		else if(rxFrame.ID==5)
		{
		   left_indicator=0;
		    if(rxFrame.Data1=='R')
			{
			   right_indicator=1;
			}
			else
			{
			  right_indicator=0;
			  }
		}
		}   
}
int main()   
{   
  u8 arr[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
  S32 i;   
  Init_CAN1(); 
  WRITEBYTE(IODIR0,LED,0XFF);
  WRITEBYTE(IOPIN0,LED,0XFF);
	while(1)   
	{   
	   check_can_receive();
	   if((left_indicator==0) && (right_indicator==0))
	   {
                    WRITEBYTE(IOPIN0,LED,0XFF);
                    delayMS(100);
       }
       else if((left_indicator==1) && (right_indicator==0))
       {
                   for(i=0;i<8;i++)
                   {
                       WRITEBYTE(IOPIN0,LED,(arr[i]));
					   check_can_receive();
		               if(left_indicator==0)
			              break;
                       delayMS(50);
                   }
		}
	    else if((right_indicator==0) && (left_indicator==0))
	    {
		    WRITEBYTE(IOPIN0,LED,0XFF);
		    delayMS(100);
	    }
	    else if((left_indicator==0) && (right_indicator==1))
	    {
		       for(i=7;i>=0;i--)
		        {
		       WRITEBYTE(IOPIN0,LED,(arr[i]));
			   check_can_receive();
		       if(right_indicator==0)
                               break;
		       delayMS(50);
		       }
	    }
    }
}   
   

