#include<lpc21xx.h>
#include "delay.h"
#include "types.h"
#include "can.h"
#include "can_defines.h"
#include "adc.h"
#include "adc_defines.h"
#include "lcd.h"
#define ADC_EMPY 0.451 //VO
#define ADC_FULL 3.390
//#define ACD_FULL_TRANK 1.841   //V0
f32 AR;
u32 adcDVal;
u32 fuel_val=0;
u32 adcDval,fg_per;
f32 eAr,fg_min=0.280,fg_max=1.735,fg_per_f;
struct CAN_Frame txFrame;
int main()
{
   Init_ADC();
   Init_CAN1();
 //Init_LCD();
   LCD_Init();
  
   while(1)
   {
  
	/* Read_ADC(0);
		//fg_per_f=((eAr-fg_min)*100)/(fg_max-fg_min);
		//fg_per_f = ((eAr - fg_min) * 100.0f) / (fg_max - fg_min);
		 Write_CMD_LCD(0x80);
		 Write_float_LCD(eAr,3);
		 Write_LCD(' ');
	     Write_LCD(' ');
		 Write_int_LCD(adcDval);
         Write_LCD(' ');
     	if (eAr< 0.280)
	     	fg_per = 0;
	    else if(eAr> 1.735) 
		   fg_per = 100;
    	else
		    fg_per =(u32)((eAr - fg_min) * 100.0f) / (fg_max - fg_min);
			 Write_CMD_LCD(0xC0);
			 Write_int_LCD(fg_per);
			 Write_LCD('%');
			 Write_LCD(' ');  	 */

	/*	Write_CMD_LCD(0xC0+9);
		Write_LCD(0);
		Write_LCD(1); */
	AR=Read_ADC(0);
    //Read_ADC(1,&adcDVal,&eAR);
    //fuel_val=((AR-ADC_EMPY)*100/(ADC_FULL-ADC_EMPY));
   // fuel_val=((AR-0.306)/(1.741-0.306))*100;
	fuel_val=((AR-0.306)/(2.270-0.306))*100;
//	fuel_val=((AR-0.045)/(0.448-0.045))*100;
    Write_CMD_LCD(0x80);
    Write_float_LCD(AR,3);
    //delayMS(500);
    Write_CMD_LCD(0xc0);
    Write_int_LCD(fuel_val);	
	//Write_CMD_LCD(0xc0);
     Write_str_LCD("   ");
     txFrame.ID=1;
     txFrame.vbf.RTR=0;
     txFrame.vbf.DLC=4;
     //txFrame.Data1=1;//fuel_val;
	 txFrame.Data1=fuel_val;//fuel_val;
     CAN1_Tx(txFrame);
	 //Write_CMD_LCD('A');	   
	}
}
 