#include "common.h"
#include "stdlib.h"
#include "usart3.h"
#include "dac.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 WIFI STA驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2015/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//ATK-ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
u8  netpro=0;	//网络模式
char USART3_SEND_BUF_LEFT[4];
char USART3_SEND_BUF_RIGHT[4];
char USART3_SEND_BUF_DATE[5];
char USART3_SEND_BUF_DATEALL[27];
int date=0,right=0,left=0;
u8 atk_8266_wifista_test(void)
{

//	char USART3_SEND_BUF_LEFT[4];
//	char USART3_SEND_BUF_RIGHT[4];
//	char USART3_SEND_BUF_DATE[5];
	//u8 netpro=0;	//网络模式

	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	u8 *p;
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
//	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
//	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP
//	atk_8266_send_cmd("AT+CIFSR","OK",20);
//	netpro|=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[0]);	//选择网络模式

		//TCP Server
		
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 服务器端",16,240); 
				//Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
				atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
				atk_8266_send_cmd("AT+CIPSTA=\"192.168.4.122\"","OK",20);
				atk_8266_send_cmd("AT+AUTOCONN","OK",20);
				sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //开启Server模式(0，关闭；1，打开)，端口号为portnum
				//sprintf((char*)p,"AT+CIPSERVER=1,8086"); 
				if((atk_8266_send_cmd(p,"OK",50))==0)
				{
					Show_Str(30,50,200,16,"正在配置ATK-ESP模块,server ok...",12,0);
				}
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
			Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240);
			Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);			
			LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
			Show_Str(30,50,200,16,"WK_UP:退出测试  KEY0:发送数据",12,0);
			LCD_Fill(30,80,239,80+12,WHITE);
			atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
			sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
			Show_Str(30,65,200,12,p,12,0);				//显示IP地址和端口	
			Show_Str(30,80,200,12,"状态:",12,0); 		//连接状态
			Show_Str(120,80,200,12,"模式:",12,0); 		//连接状态
			Show_Str(30,100,200,12,"发送数据:",12,0); 	//发送数据
			Show_Str(30,115,200,12,"接收数据:",12,0);	//接收数据
			atk_8266_wificonf_show(30,180,"请设置路由器无线参数为:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
			POINT_COLOR=BLUE;
			Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//连接状态
			DAC_SetChannel1Data(DAC_Align_12b_R,0);
			DAC_SetChannel2Data(DAC_Align_12b_R,0);
			while(1)
			{
				if(timex)timex--;
				if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
				t++;
				delay_ms(10);
				if(USART3_RX_STA&0X8000)		//接收到一次数据了
				{ 
					rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
					USART3_RX_BUF[26]=0;		//添加结束符 
					
					strncpy(USART3_SEND_BUF_DATEALL,(char *)USART3_RX_BUF,27);
					USART3_SEND_BUF_DATEALL[26]=0;
					strncpy(USART3_SEND_BUF_DATE,&USART3_SEND_BUF_DATEALL[12],6);
					date= atoi(USART3_SEND_BUF_DATE);
					
					strncpy(USART3_SEND_BUF_LEFT,&USART3_SEND_BUF_DATEALL[18],4);
					left = atoi(USART3_SEND_BUF_LEFT);			
//					strncpy(USART3_SEND_BUF_RIGHT,&USART3_SEND_BUF_DATEALL[22],4);
					right = atoi(&USART3_RX_BUF[22]);
//					right = (right/100000)/10;
					if((date/100000)==1)	//电机启动
					{
							GPIO_SetBits(GPIOG,GPIO_Pin_12);   //左电机使能
						if(((date/10000)%10)==1)//左电机正转
						{
							GPIO_SetBits(GPIOG,GPIO_Pin_11);
							if(date/1000%10==1)
							{
								GPIO_SetBits(GPIOG,GPIO_Pin_2);//右电机正转
							}
							else //if(date/100%10==0)
							{
								GPIO_ResetBits(GPIOG,GPIO_Pin_2);//右电机反转
							}
						}
						else //左电机反转
						{
							GPIO_ResetBits(GPIOG,GPIO_Pin_11);
							if(date/1000%10==1)
							{
								GPIO_SetBits(GPIOG,GPIO_Pin_2);//右电机正转
							}
							else 
							{
								GPIO_ResetBits(GPIOG,GPIO_Pin_2);//右电机反转
							}
						}	
//							Dac2_Set_Vol(1200);
//							Dac2_Set_Vol(right);						
							Dac1_Set_Vol(left);		
//							delay_ms(1000);							
							Dac2_Set_Vol(right);								
													
					}
					if(((date/100)%10)==1)
						GPIO_SetBits(GPIOG,GPIO_Pin_14);//开真空回收
					else //if(((date/10)%10)==0)	//关真空回收
						GPIO_ResetBits(GPIOG,GPIO_Pin_14);					
					if((date/10%10 ==1))
							GPIO_SetBits(GPIOG,GPIO_Pin_15);//开水泵
					else 
							GPIO_ResetBits(GPIOG,GPIO_Pin_15);//关水泵
					if((date%10 ==1))
							GPIO_SetBits(GPIOG,GPIO_Pin_4);//开卷扬机
					else 
							GPIO_ResetBits(GPIOG,GPIO_Pin_4);//关卷扬机
					printf("%d\n",&USART3_RX_BUF[12]);	//发送到串口 
					
					sprintf((char*)p,"收到%d字节,内容如下",right);//接收到的字节数 
					LCD_Fill(30+54,115,239,130,WHITE);
					POINT_COLOR=BRED;
					Show_Str(30+54,115,156,12,p,12,0); 			//显示接收到的数据长度
					POINT_COLOR=BLUE;
					LCD_Fill(30,130,239,319,WHITE);
					Show_Str(30,130,180,190,&USART3_RX_BUF[12],12,0);//显示接收到的数据

					USART3_RX_BUF[12]=0;
					USART3_SEND_BUF_LEFT[0]=0;
					USART3_SEND_BUF_RIGHT[0]=0;
					USART3_SEND_BUF_DATE[0]=0;	
					USART3_SEND_BUF_DATEALL[0]=0;
					USART3_RX_STA&=0;
					if((rlen-12) == -2)
					{
						DAC_SetChannel1Data(DAC_Align_12b_R,0);
						DAC_SetChannel2Data(DAC_Align_12b_R,0);
					}
					if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
					else t=0;                   //状态为已经连接了,10秒后再检查		)	
				
				} 
				
				if(t==100)//连续10秒钟没有收到任何数据,检查连接是不是还存在.2
				{
					LCD_Fill(30+54,125,239,130,WHITE);
					LCD_Fill(60,80,120,92,WHITE);
					constate=atk_8266_consta_check();//得到连接状态
					if(constate=='+')Show_Str(30+30,80,200,12,"连接成功",12,0);  //连接状态
					else 
					{
						Show_Str(30+30,80,200,12,"连接失败",12,0);					
					}
					t=0;
				}
				if((t%20)==0)LED1=!LED1;
				atk_8266_at_response(1);
		}
	myfree(SRAMIN,p);		//释放内存 
	return res;		
} 



