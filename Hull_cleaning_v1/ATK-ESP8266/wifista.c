#include "common.h"
#include "stdlib.h"
#include "usart3.h"
#include "dac.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� WIFI STA��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//ATK-ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8  netpro=0;	//����ģʽ
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
	//u8 netpro=0;	//����ģʽ

	u8 timex=0; 
	u8 ipbuf[16]; 	//IP����
	u8 *p;
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//����״̬
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
//	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
//	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//����Ŀ��·����,���һ��IP
//	atk_8266_send_cmd("AT+CIFSR","OK",20);
//	netpro|=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[0]);	//ѡ������ģʽ

		//TCP Server
		
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ��������",16,240); 
				//Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
				atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
				atk_8266_send_cmd("AT+CIPSTA=\"192.168.4.122\"","OK",20);
				atk_8266_send_cmd("AT+AUTOCONN","OK",20);
				sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //����Serverģʽ(0���رգ�1����)���˿ں�Ϊportnum
				//sprintf((char*)p,"AT+CIPSERVER=1,8086"); 
				if((atk_8266_send_cmd(p,"OK",50))==0)
				{
					Show_Str(30,50,200,16,"��������ATK-ESPģ��,server ok...",12,0);
				}
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
			Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240);
			Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);			
			LCD_Fill(30,50,239,50+12,WHITE);			//���֮ǰ����ʾ
			Show_Str(30,50,200,16,"WK_UP:�˳�����  KEY0:��������",12,0);
			LCD_Fill(30,80,239,80+12,WHITE);
			atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
			sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
			Show_Str(30,65,200,12,p,12,0);				//��ʾIP��ַ�Ͷ˿�	
			Show_Str(30,80,200,12,"״̬:",12,0); 		//����״̬
			Show_Str(120,80,200,12,"ģʽ:",12,0); 		//����״̬
			Show_Str(30,100,200,12,"��������:",12,0); 	//��������
			Show_Str(30,115,200,12,"��������:",12,0);	//��������
			atk_8266_wificonf_show(30,180,"������·�������߲���Ϊ:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
			POINT_COLOR=BLUE;
			Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//����״̬
			DAC_SetChannel1Data(DAC_Align_12b_R,0);
			DAC_SetChannel2Data(DAC_Align_12b_R,0);
			while(1)
			{
				if(timex)timex--;
				if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
				t++;
				delay_ms(10);
				if(USART3_RX_STA&0X8000)		//���յ�һ��������
				{ 
					rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
					USART3_RX_BUF[26]=0;		//��ӽ����� 
					
					strncpy(USART3_SEND_BUF_DATEALL,(char *)USART3_RX_BUF,27);
					USART3_SEND_BUF_DATEALL[26]=0;
					strncpy(USART3_SEND_BUF_DATE,&USART3_SEND_BUF_DATEALL[12],6);
					date= atoi(USART3_SEND_BUF_DATE);
					
					strncpy(USART3_SEND_BUF_LEFT,&USART3_SEND_BUF_DATEALL[18],4);
					left = atoi(USART3_SEND_BUF_LEFT);			
//					strncpy(USART3_SEND_BUF_RIGHT,&USART3_SEND_BUF_DATEALL[22],4);
					right = atoi(&USART3_RX_BUF[22]);
//					right = (right/100000)/10;
					if((date/100000)==1)	//�������
					{
							GPIO_SetBits(GPIOG,GPIO_Pin_12);   //����ʹ��
						if(((date/10000)%10)==1)//������ת
						{
							GPIO_SetBits(GPIOG,GPIO_Pin_11);
							if(date/1000%10==1)
							{
								GPIO_SetBits(GPIOG,GPIO_Pin_2);//�ҵ����ת
							}
							else //if(date/100%10==0)
							{
								GPIO_ResetBits(GPIOG,GPIO_Pin_2);//�ҵ����ת
							}
						}
						else //������ת
						{
							GPIO_ResetBits(GPIOG,GPIO_Pin_11);
							if(date/1000%10==1)
							{
								GPIO_SetBits(GPIOG,GPIO_Pin_2);//�ҵ����ת
							}
							else 
							{
								GPIO_ResetBits(GPIOG,GPIO_Pin_2);//�ҵ����ת
							}
						}	
//							Dac2_Set_Vol(1200);
//							Dac2_Set_Vol(right);						
							Dac1_Set_Vol(left);		
//							delay_ms(1000);							
							Dac2_Set_Vol(right);								
													
					}
					if(((date/100)%10)==1)
						GPIO_SetBits(GPIOG,GPIO_Pin_14);//����ջ���
					else //if(((date/10)%10)==0)	//����ջ���
						GPIO_ResetBits(GPIOG,GPIO_Pin_14);					
					if((date/10%10 ==1))
							GPIO_SetBits(GPIOG,GPIO_Pin_15);//��ˮ��
					else 
							GPIO_ResetBits(GPIOG,GPIO_Pin_15);//��ˮ��
					if((date%10 ==1))
							GPIO_SetBits(GPIOG,GPIO_Pin_4);//�������
					else 
							GPIO_ResetBits(GPIOG,GPIO_Pin_4);//�ؾ����
					printf("%d\n",&USART3_RX_BUF[12]);	//���͵����� 
					
					sprintf((char*)p,"�յ�%d�ֽ�,��������",right);//���յ����ֽ��� 
					LCD_Fill(30+54,115,239,130,WHITE);
					POINT_COLOR=BRED;
					Show_Str(30+54,115,156,12,p,12,0); 			//��ʾ���յ������ݳ���
					POINT_COLOR=BLUE;
					LCD_Fill(30,130,239,319,WHITE);
					Show_Str(30,130,180,190,&USART3_RX_BUF[12],12,0);//��ʾ���յ�������

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
					if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
					else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��		)	
				
				} 
				
				if(t==100)//����10����û���յ��κ�����,��������ǲ��ǻ�����.2
				{
					LCD_Fill(30+54,125,239,130,WHITE);
					LCD_Fill(60,80,120,92,WHITE);
					constate=atk_8266_consta_check();//�õ�����״̬
					if(constate=='+')Show_Str(30+30,80,200,12,"���ӳɹ�",12,0);  //����״̬
					else 
					{
						Show_Str(30+30,80,200,12,"����ʧ��",12,0);					
					}
					t=0;
				}
				if((t%20)==0)LED1=!LED1;
				atk_8266_at_response(1);
		}
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	return res;		
} 



