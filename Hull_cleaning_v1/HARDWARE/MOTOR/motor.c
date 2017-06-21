#include "sys.h"
#include "common.h"
#include "adc.h"
#include "delay.h"
#include "motor.h"
u8 Motor_control(u16 speed_l,u16 speed_r)
{		
	u16 adcx;
	float temp; 
	u16 pwmval=0;
	POINT_COLOR=RED; 
	LCD_ShowString(30,50+50+70,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70+50+70,200,16,16,"PWM DAC TEST");	
	LCD_ShowString(30,90+50+70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110+50+70,200,16,16,"2014/5/6");	 
	LCD_ShowString(30,130+50+70,200,16,16,"WK_UP:+  KEY1:-");	 
	POINT_COLOR=BLUE;//设置字体为蓝色      	 
	LCD_ShowString(30,150+50+70,200,16,16,"DAC VAL:");	      
	LCD_ShowString(30,170+50+70,200,16,16,"DAC VOL:0.000V");	      
	LCD_ShowString(30,190+50+70,200,16,16,"ADC VOL:0.000V");
	TIM_SetCompare2(TIM9,pwmval);	//初始值 
		if((speed_l > 0)&&(speed_l < 500))
		{
			while(pwmval<speed_l)
			{
				pwmval+=5;
				TIM_SetCompare2(TIM9,pwmval);	//输出	 
				adcx=TIM_GetCapture2(TIM9);;
				LCD_ShowxNum(94,150+120,adcx,3,16,0);     	//显示DAC寄存器值
				temp=(float)adcx*(3.3/256);;			      //得到DAC电压值
				adcx=temp;
				LCD_ShowxNum(94,170+120,temp,1,16,0);     	//显示电压值整数部分
				temp-=adcx;
				temp*=1000;
				LCD_ShowxNum(110,170+120,temp,3,16,0x80); 	//显示电压值的小数部分
				adcx=Get_Adc_Average(ADC_Channel_5,20); //得到ADC转换值	  
				temp=(float)adcx*(3.3/4096);			      //得到ADC电压值
				adcx=temp;
				LCD_ShowxNum(94,190+120,temp,1,16,0);     	//显示电压值整数部分
				temp-=adcx;
				temp*=1000;
				LCD_ShowxNum(110,190+120,temp,3,16,0x80); 	//显示电压值的小数部分		   
				delay_ms(10); 
			}
		}
//		else if((speed_l < 0)&&(speed_l > -256))
//		{
//			while(pwmval>speed_l)
//			{
//				pwmval-=5;
//				TIM_SetCompare2(TIM9,pwmval);	//输出	 
//				adcx=TIM_GetCapture2(TIM9);;
//				LCD_ShowxNum(94,150+120,adcx,3,16,0);     	//显示DAC寄存器值
//				temp=(float)adcx*(3.3/256);;			      //得到DAC电压值
//				adcx=temp;
//				LCD_ShowxNum(94,170+120,temp,1,16,0);     	//显示电压值整数部分
//				temp-=adcx;
//				temp*=1000;
//				LCD_ShowxNum(110,170+120,temp,3,16,0x80); 	//显示电压值的小数部分
//				adcx=Get_Adc_Average(ADC_Channel_5,20); //得到ADC转换值	  
//				temp=(float)adcx*(3.3/4096);			      //得到ADC电压值
//				adcx=temp;
//				LCD_ShowxNum(94,190+120,temp,1,16,0);     	//显示电压值整数部分
//				temp-=adcx;
//				temp*=1000;
//				LCD_ShowxNum(110,190+120,temp,3,16,0x80); 	//显示电压值的小数部分		   
//				delay_ms(10); 
//			}
//		}
	return 0;
}
