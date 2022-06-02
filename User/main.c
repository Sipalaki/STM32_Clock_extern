/*********************************************************************************************
程序名：	电子时钟扩展项目
编写人：	Sipalaki	
编写时间：	2022年5月30日
硬件支持：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  

修改日志：　　
1-202205301910 开始编程	
	
							
说明：
 # 本模板加载了STM32F103内部的RCC时钟设置，并加入了利用滴答定时器的延时函数。
 # 可根据自己的需要增加或删减。

*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "rtc.h"
#include "TM1640.h"
#include "touch_key.h"
#include "lm75a.h"
#include "buzzer.h"
#include "flash.h"
#include "relay.h"


#define FLASH_START_ADDR 0x0801f000
#define TEMP_ADDR 0x0801E000
#define KEYA_SPEED1	100	
#define Disp0 2000
#define Disp1 3000
#define Disp2 1000

int main (void){//主程序
	u8 MENU = 0;//菜单
	u8 MENU_2 = 0;
	u16 i = 0;
	u16 temp = 0;
	u8 rupdate = 0;
	u8 buffer[3];
	u8 a = 0,c=0;
	u32 b = 0;
	u8 al = 0;
	u8 al_flag = 0;
	u16 al_delay = 0;
	u8 alhour[10] = {0};
	u8 almin[10] = {0};
	u16 temps[3] = {100};
	RCC_Configuration(); //系统时钟初始化 
	RELAY_Init();
	delay_ms(200);
	RTC_Config();  //RTC初始化
	TM1640_Init(); //TM1640初始化
	TOUCH_KEY_Init();
	BUZZER_Init(); 
	BUZZER_BEEP2();
	I2C_Configuration();
	LM75A_GetTemp(buffer);
	RELAY_1(0);
	RELAY_2(0);
	
	for(b = 0;b<11;b++){
		temp = FLASH_R(FLASH_START_ADDR+b*2 );
		alhour[b] = temp / 0x100;
		almin[b] = temp % 0x100;
	}
	
	for(b=0;b<3;b++){
	temp = FLASH_R(TEMP_ADDR+b*2);
	temps[b] = temp;
	}
	
	if(alhour[0]>24){
		for(b=0;b<10;b++){
			alhour[b] = 24;
			almin[b] = 0;
	}
		ALFLASH_W(alhour,almin,FLASH_START_ADDR,11);
}
	if(temps[2]!=100){
		for(b=0;b<3;b++){
			temps[b] = 100;
	}
		TEMPFLASH_W(temps,TEMP_ADDR,3);
}
	
	while(1){
		if(rupdate){
			rupdate = 0;
			RTC_Set(ryear,rmon,rday,rhour,rmin,rsec);
		}
		if(MENU<3)
		{
			RTC_Get();
		}
		LM75A_GetTemp(buffer);
		
		switch(MENU){
			case 0:
					i++;
					TM1640_display(0,ryear/1000);	//年
					TM1640_display(1,(ryear/100)%10);
					TM1640_display(2,(ryear/10)%10); 
					TM1640_display(3,ryear%10+10); 
					TM1640_display(4,rmon/10);//月
					TM1640_display(5,rmon%10+10);
					TM1640_display(6,rday/10);	//日
					TM1640_display(7,rday%10+10);
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //???????????j??
						MENU = 0;
						i = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //???????????j??
						MENU = 1;
						i = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = 2;
						i = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						MENU = 3;
						i = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					if(i>Disp0){i=0;MENU = 1;}
				break;
			
			case 1:
				i++;
				TM1640_display(0,rhour/10); //时
				TM1640_display(1,rhour%10);
				TM1640_display(2,21);
				TM1640_display(3,rmin/10);	//分
				TM1640_display(4,rmin%10);
				TM1640_display(5,21);
				TM1640_display(6,rsec/10); //秒
				TM1640_display(7,rsec%10);
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //???????????j??
					MENU = 0;
					i = 0;
					BUZZER_BEEP1();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //???????????j??
					MENU = 1;
					i = 0;
					BUZZER_BEEP1();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
					MENU = 2;
					i = 0;
					BUZZER_BEEP1();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
					MENU = 3;
					i = 0;
					BUZZER_BEEP1();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}
				if(i>Disp1){i=0;MENU = 2;}
				break;
			
			case 2:
					i++;
					TM1640_display(0,20);	//
					TM1640_display(1,20);
					TM1640_display(2,20); //
					TM1640_display(3,20+buffer[0]); //正负
					TM1640_display(4,buffer[1]%100/10);	//温度
					TM1640_display(5,buffer[1]%10+10);
					TM1640_display(6,buffer[2]%100/10); //
					TM1640_display(7,22);//c
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //???????????j??
						MENU = 0;
						i = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //???????????j??
						MENU = 1;
						i = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = 2;
						i = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						MENU = 3;
						i = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					if(i>Disp2){i=0;MENU = 0;}
				break;
				
		case 3:
					TM1640_display(0,5);	//
					TM1640_display(1,23);
					TM1640_display(2,24); //
					TM1640_display(3,20); //
					TM1640_display(4,20);	//
					TM1640_display(5,20);
					TM1640_display(6,20); //
					TM1640_display(7,20);//
					MENU = 100;
					
				break;
			
			
			
			case 4:
					i++;
					if(i<600){
					TM1640_display(0,ryear/1000);	//年
					TM1640_display(1,(ryear/100)%10);
					TM1640_display(2,(ryear/10)%10); 
					TM1640_display(3,ryear%10+10); 
					
					
					}else{
					TM1640_display(0,20);	//
					TM1640_display(1,20);
					TM1640_display(2,20);	//
					TM1640_display(3,20);
					
					}
					if(i>1200) i=0;
					TM1640_display(4,rmon/10);//月
					TM1640_display(5,rmon%10+10);
					
					TM1640_display(6,rday/10);	//日
					TM1640_display(7,rday%10+10);
					
				
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
								i=0;
								ryear++;
								if(ryear>2099) ryear=2000;
								TM1640_display(0,ryear/1000);	//年
								TM1640_display(1,(ryear/100)%10);
								TM1640_display(2,(ryear/10)%10); 
								TM1640_display(3,ryear%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								BUZZER_BEEP1();
								i=0;
								ryear++;
								if(ryear>2099) ryear=2000;
							}
						}
						a=0;c=0; //???0
					}
					
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
								i=0;
								ryear--;
								if(ryear<2000) ryear=2099;

								TM1640_display(0,ryear/1000);	//年
								TM1640_display(1,(ryear/100)%10);
								TM1640_display(2,(ryear/10)%10); 
								TM1640_display(3,ryear%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								BUZZER_BEEP1();
								i=0;
								ryear--;
								if(ryear<2000) ryear=2099;

							}
						}
						a=0;c=0; //???0
					}
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = 5;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						MENU = 0;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					
				break;
			
			case 5:
					i++;
					if(i<600){
						TM1640_display(4,rmon/10);//月
						TM1640_display(5,rmon%10+10);
					
					
					}else{
					TM1640_display(4,20);	//
					TM1640_display(5,20);
					
					}
					if(i>1200) i=0;
					TM1640_display(0,ryear/1000);	//年
					TM1640_display(1,(ryear/100)%10);
					TM1640_display(2,(ryear/10)%10); 
					TM1640_display(3,ryear%10+10); 
					
					
					TM1640_display(6,rday/10);	//日
					TM1640_display(7,rday%10+10);
					
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
								i=0;
								rmon++;
								if(rmon>12) rmon=1;
								TM1640_display(4,(rmon/10)%10); 
								TM1640_display(5,rmon%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								BUZZER_BEEP1();
								i=0;
								rmon++;
								if(rmon>12) ryear=1;
							}
						}
						a=0;c=0; //???0
					}
					
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
								i=0;
								rmon--;
								if(rmon<1) rmon=12;


								TM1640_display(4,(rmon/10)%10); 
								TM1640_display(5,rmon%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								BUZZER_BEEP1();
								i=0;
								rmon--;
								if(rmon<1) rmon=12;

							}
						}
						a=0;c=0; //???0
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = 6;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						MENU = 0;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					
				break;
			
			case 6:
					i++;
					if(i<600){
						TM1640_display(6,rday/10);	//日
						TM1640_display(7,rday%10+10);
					
					
					}else{
						TM1640_display(6,20);	//
						TM1640_display(7,20);
					
					}
					if(i>1200) i=0;
					TM1640_display(0,ryear/1000);	//年
					TM1640_display(1,(ryear/100)%10);
					TM1640_display(2,(ryear/10)%10); 
					TM1640_display(3,ryear%10+10); 
					TM1640_display(4,rmon/10);//月
					TM1640_display(5,rmon%10+10);
					
					
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
								i=0;
								rday++;
								if(rday>31) rday=1;
								TM1640_display(6,(rday/10)%10); 
								TM1640_display(7,rday%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								BUZZER_BEEP1();
								i=0;
								rday++;
								if(rday>31) rday=1;
							}
						}
						a=0;c=0; //???0
					}
					
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
								i=0;
								rday--;
								if(rday<1) rday=31;


								TM1640_display(6,(rday/10)%10); 
								TM1640_display(7,rday%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								BUZZER_BEEP1();
								i=0;
								rday--;
								if(rday<1) rday=31;

							}
						}
						a=0;c=0; //???0
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = 7;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						MENU = 0;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					
				break;

			case 7:
					i++;
					if(i<600){
					TM1640_display(0,rhour/10); //时
					TM1640_display(1,rhour%10);
					
					
					}else{
						TM1640_display(0,20);	//
						TM1640_display(1,20);
					
					}
					if(i>1200) i=0;

					TM1640_display(2,21);
					TM1640_display(3,rmin/10);	//分
					TM1640_display(4,rmin%10);
					TM1640_display(5,21);
					TM1640_display(6,rsec/10); //秒
					TM1640_display(7,rsec%10);
					
					
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
								i=0;
								rhour++;
								if(rhour>23) rhour=0;
								TM1640_display(0,(rhour/10)%10); 
								TM1640_display(1,rhour%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								BUZZER_BEEP1();
								i=0;
								rhour++;
								if(rhour>23) rhour=0;
							}
						}
						a=0;c=0; //???0
					}
					
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
								i=0;
								
								if(rhour==0) rhour=24;
								rhour--;


								TM1640_display(0,(rhour/10)%10); 
								TM1640_display(1,rhour%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								BUZZER_BEEP1();
								i=0;
								if(rhour==0) rhour=24;
								rhour--;

							}
						}
						a=0;c=0; //???0
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = 8;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						MENU = 1;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					
				break;
				
			case 8:
					i++;
					if(i<600){
						TM1640_display(3,rmin/10);	//分
						TM1640_display(4,rmin%10);
					
					
					}else{
						TM1640_display(3,20);	//
						TM1640_display(4,20);
					
					}
					if(i>1200) i=0;
					TM1640_display(0,rhour/10); //时
					TM1640_display(1,rhour%10);
					TM1640_display(2,21);

					TM1640_display(5,21);
					TM1640_display(6,rsec/10); //秒
					TM1640_display(7,rsec%10);
					
					
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
								i=0;
								rmin++;
								if(rmin>59) rmin=0;
								TM1640_display(3,(rmin/10)%10); 
								TM1640_display(4,rmin%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								i=0;
								rmin++;
								if(rmin>59) rmin=0;
							}
						}
						rsec = 0;
						a=0;c=0; //???0
					}
					
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//?????
						while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEYA_SPEED1){ 
							c++;delay_ms(10); 
						}
						if(c>=KEYA_SPEED1){ 
							
							while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
								i=0;
								
								if(rmin==0) rmin=60;
								rmin--;


								TM1640_display(3,(rmin/10)%10); 
								TM1640_display(4,rmin%10+10); 
								delay_ms(50);
								BUZZER_BEEP1();
							};
						}else{ 
							if(a==0){ 
								i=0;
								if(rmin==0) rmin=60;
								rmin--;

							}
						}
						rsec = 0;
						a=0;c=0; //???0
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = 4;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						MENU = 1;
						rupdate = 1;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					
				break;
			

			case 100:
				switch(MENU_2){
					case 0:
						TM1640_display(0,5);	//
						TM1640_display(1,23);
						TM1640_display(2,24); //
						TM1640_display(3,20);
						TM1640_display(4,20);
						break;
					case 10:
						TM1640_display(0,5);	//
						TM1640_display(1,23);
						TM1640_display(2,24); //
						TM1640_display(3,22);
						TM1640_display(4,11);

						break;
					case 11:
						TM1640_display(0,5);	//
						TM1640_display(1,23);
						TM1640_display(2,24); //
						TM1640_display(3,22);
						TM1640_display(4,12);
						
						break;
					default:
						TM1640_display(0,25);	//
						TM1640_display(1,26);
						TM1640_display(2,MENU_2+10); //
						TM1640_display(3,20);
						TM1640_display(4,20);
						break;	
				}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //???????????j??
						MENU_2++;
						if(MENU_2>11) MENU_2 = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //???????????j??
						if(MENU_2==0) MENU_2 = 12;
						MENU_2--;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = MENU_2 * 10 + 4;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						MENU = 0;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					
				break;
				
			case 14:case 24:case 34:case 44:case 54:case 64:case 74:case 84:case 94:
				 i++;
				if(i<600){
					if(alhour[MENU_2]==24){
						TM1640_display(4,27); //时
						TM1640_display(5,28);
						TM1640_display(6,28);
						TM1640_display(7,20);
				
				}else{
				TM1640_display(4,alhour[MENU_2]/10); //时
				TM1640_display(5,alhour[MENU_2]%10+10);
				}
				
				}else{
					TM1640_display(4,20);	//
					TM1640_display(5,20);
					if(alhour[MENU_2]==24){
						TM1640_display(6,20);
				}
				
				}
				if(i>1200) i=0;
				TM1640_display(0,25);	//
				TM1640_display(1,26);
				TM1640_display(2,MENU_2+10); //
				if(alhour[MENU_2]!=24){
				TM1640_display(6,almin[MENU_2]/10);	//分
				TM1640_display(7,almin[MENU_2]%10+10);}

				
				
				
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//?????
					while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEYA_SPEED1){ 
						c++;delay_ms(10); 
					}
					if(c>=KEYA_SPEED1){ 
						
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
							i=0;
							alhour[MENU_2]++;
							if(alhour[MENU_2]>24) alhour[MENU_2]=0;
							TM1640_display(4,(alhour[MENU_2]/10)%10); 
							TM1640_display(5,alhour[MENU_2]%10+10); 
							delay_ms(50);
							BUZZER_BEEP1();
						};
					}else{ 
						if(a==0){ 
							BUZZER_BEEP1();
							i=0;
							alhour[MENU_2]++;
							if(alhour[MENU_2]>24) alhour[MENU_2]=0;
						}
					}
					a=0;c=0; //???0
				}
				
				
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//?????
					while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEYA_SPEED1){ 
						c++;delay_ms(10); 
					}
					if(c>=KEYA_SPEED1){ 
						
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
							i=0;
							
							if(alhour[MENU_2]==0) alhour[MENU_2]=25;
							alhour[MENU_2]--;


							TM1640_display(4,(alhour[MENU_2]/10)%10); 
							TM1640_display(5,alhour[MENU_2]%10+10); 
							delay_ms(50);
							BUZZER_BEEP1();
						};
					}else{ 
						if(a==0){ 
							BUZZER_BEEP1();
							i=0;
							if(alhour[MENU_2]==0) alhour[MENU_2]=25;
							alhour[MENU_2]--;

						}
					}
					a=0;c=0; //???0
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						if(alhour[MENU_2]!=24){
						MENU = MENU_2 * 10 + 5;
						BUZZER_BEEP1();}
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						MENU = 3;
						ALFLASH_W(alhour,almin,FLASH_START_ADDR,11);
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
			
				break;
	case 15:case 25:case 35:case 45:case 55:case 65:case 75:case 85:case 95:
		i++;
				if(i<600){
				TM1640_display(6,almin[MENU_2]/10); //min
				TM1640_display(7,almin[MENU_2]%10);
				
				
				}else{
					TM1640_display(6,20);	//
					TM1640_display(7,20);
				
				}
				if(i>1200) i=0;
				TM1640_display(0,25);	//
				TM1640_display(1,26);
				TM1640_display(2,MENU_2+10); //
				TM1640_display(4,alhour[MENU_2]/10);	//hour
				TM1640_display(5,alhour[MENU_2]%10+10);

				
				
				
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//?????
					while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEYA_SPEED1){ 
						c++;delay_ms(10); 
					}
					if(c>=KEYA_SPEED1){ 
						
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
							i=0;
							almin[MENU_2]++;
							if(almin[MENU_2]>59) almin[MENU_2]=0;
							TM1640_display(6,(almin[MENU_2]/10)%10); 
							TM1640_display(7,almin[MENU_2]%10+10); 
							delay_ms(50);
							BUZZER_BEEP1();
						};
					}else{ 
						if(a==0){ 
							BUZZER_BEEP1();
							i=0;
							almin[MENU_2]++;
							if(almin[MENU_2]>59) almin[MENU_2]=0;
						}
					}
					a=0;c=0; //???0
				}
				
				
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//?????
					while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEYA_SPEED1){ 
						c++;delay_ms(10); 
					}
					if(c>=KEYA_SPEED1){ 
						
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
							i=0;
							
							if(almin[MENU_2]==0) almin[MENU_2]=60;
							almin[MENU_2]--;


							TM1640_display(6,(almin[MENU_2]/10)%10); 
							TM1640_display(7,almin[MENU_2]%10+10); 
							delay_ms(50);
							BUZZER_BEEP1();
						};
					}else{ 
						if(a==0){ 
							BUZZER_BEEP1();
							i=0;
							if(almin[MENU_2]==0) almin[MENU_2]=60;
							almin[MENU_2]--;

						}
					}
					a=0;c=0; //???0
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = MENU_2 * 10 + 4;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						ALFLASH_W(alhour,almin,FLASH_START_ADDR,11);
						MENU = 3;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
		break;
		
	case 200:
		i++;
		if(i<600){
				TM1640_display(6,almin[al]/10); //min
				TM1640_display(7,almin[al]%10);	
				TM1640_display(0,25);	//
				TM1640_display(1,26);
				TM1640_display(2,al+10); //
				TM1640_display(4,alhour[al]/10);	//hour
				TM1640_display(5,alhour[al]%10+10);
		}else{
			TM1640_display(0,20);	//
			TM1640_display(1,20);	//
			TM1640_display(2,20);
			TM1640_display(3,20);	//
			TM1640_display(4,20);	//
			TM1640_display(5,20);
			TM1640_display(6,20);	//
			TM1640_display(7,20);
		}
		if(i>1200) {i=0;BUZZER_BEEP1();}
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
				MENU = 1;
				BUZZER_BEEP1();
				i = 0;
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			}
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
				MENU = 1;
				BUZZER_BEEP1();
				i = 0;
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
			}
		break;
		
	case 104: case 114:
				i++;
				if(temps[MENU_2%10]==100){
					TM1640_display(0,5);	//
					TM1640_display(1,23);
					TM1640_display(2,24); //
					TM1640_display(3,22);
					TM1640_display(4,MENU_2+1);
					TM1640_display(5,27); //时
					TM1640_display(6,28);
					TM1640_display(7,28);				
		
		}else{
				if(i<600){
					if(temps[MENU_2%10]<100){
						TM1640_display(5,21); 
						TM1640_display(6,(100-temps[MENU_2%10])/10%10); 
						TM1640_display(7,(100-temps[MENU_2%10])%10);
					}else{
						TM1640_display(5,20); 
						TM1640_display(6,(temps[MENU_2%10]-101)/10%10); 
						TM1640_display(7,(temps[MENU_2%10]-101)%10);
					}		
				}else{
					TM1640_display(5,20); 
					TM1640_display(6,20);	//
					TM1640_display(7,20);
				
				}
				if(i>1200) i=0;
				TM1640_display(0,5);	//
				TM1640_display(1,23);
				TM1640_display(2,24); //
				TM1640_display(3,22);
				TM1640_display(4,MENU_2+1);
			}
				
				
				
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//?????
					while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEYA_SPEED1){ 
						c++;delay_ms(10); 
					}
					if(c>=KEYA_SPEED1){ 
						
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
							i=0;
							temps[MENU_2%10]++;
							if(temps[MENU_2%10]>200) temps[MENU_2%10]=1;
							if(temps[MENU_2%10]<100){
								TM1640_display(5,21); 
								TM1640_display(6,(100-temps[MENU_2%10])/10%10); 
								TM1640_display(7,(100-temps[MENU_2%10])%10);
							}else{
								TM1640_display(5,20); 
								TM1640_display(6,(temps[MENU_2%10]-101)/10%10); 
								TM1640_display(7,(temps[MENU_2%10]-101)%10);
							}	
							delay_ms(50);
							BUZZER_BEEP1();
						};
					}else{ 
						if(a==0){ 
							BUZZER_BEEP1();
							i=0;
							temps[MENU_2%10]++;
							if(temps[MENU_2%10]>200) temps[MENU_2%10]=1;
						}
					}
					a=0;c=0; //???0
				}
				
				
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//?????
					while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEYA_SPEED1){ 
						c++;delay_ms(10); 
					}
					if(c>=KEYA_SPEED1){ 
						
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
							i=0;
							
							if(temps[MENU_2%10]<1) temps[MENU_2%10]=201;
							temps[MENU_2%10]--;
							if(temps[MENU_2%10]<100){
								TM1640_display(5,21); 
								TM1640_display(6,(100-temps[MENU_2%10])/10%10); 
								TM1640_display(7,(100-temps[MENU_2%10])%10);
							}else{
								TM1640_display(5,20); 
								TM1640_display(6,(temps[MENU_2%10]-101)/10%10); 
								TM1640_display(7,(temps[MENU_2%10]-101)%10);
							}	
							delay_ms(50);
							BUZZER_BEEP1();
						};
					}else{ 
						if(a==0){ 
							BUZZER_BEEP1();
							i=0;
							if(temps[MENU_2%10]<1) temps[MENU_2%10]=201;
							temps[MENU_2%10]--;

						}
					}
					a=0;c=0; //???0
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //???????????j??
						MENU = MENU_2 * 10 + 4;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
					}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //???????????j??
						TEMPFLASH_W(temps,TEMP_ADDR,3);
						if(MENU_2%10 == 0 && temps[MENU_2%10]==100){
								RELAY_1(0);
						}else if(MENU_2%10 == 1 && temps[MENU_2%10]==100){
								RELAY_2(0);
						}
						MENU = 3;
						BUZZER_BEEP1();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
		
		break;
		
	}	
		
		if(MENU<3 && al_flag==0 && rsec==0){
			for(b=0;b<10;b++){
				if(alhour[b]==rhour && almin[b]==rmin){
					al = b;
					al_flag = 1;
					if(al>5&&al<10){
					if(al==6){
						RELAY_1(1);
					}
					if(al==7){
						RELAY_1(0);
					}
					if(al==8){
						RELAY_2(1);
					}
					if(al==9){
						RELAY_2(0);
					}
					MENU = 1;
					}else{
					MENU = 200;
				}
			}
		}
	}
	if(MENU<3){
			if(temps[0]!=100){
			
			if(temps[0]<100){
				if(buffer[0]==1 && buffer[1]>=(100-temps[0])){
					RELAY_1(1);
			}else{
					RELAY_1(0);
			}
		}else{
			if(buffer[0]==0 && buffer[1] <= (temps[0]-101)){
					RELAY_1(1);
			}else{
					RELAY_1(0);
			}
	}}
			
			
			if(temps[1]!=100){
			
			if(temps[1]<100){
				if(buffer[0]==1 && buffer[1]<=(100-temps[1])){
					RELAY_2(1);
			}else{
					RELAY_2(0);
			}
		}else{
			if(buffer[0]==0 && buffer[1] >= (temps[1]-101)){
					RELAY_2(1);
			}else{
					RELAY_2(0);
			}
	}}	
	} 
		
		if(al_flag==1){
				al_delay++;
				if(al_delay>50000){
						MENU = 1;
						al_flag = 0;
						al_delay = 0;
				}
		}

	}
}

/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/
/*

【变量定义】
u32     a; //定义32位无符号变量a
u16     a; //定义16位无符号变量a
u8     a; //定义8位无符号变量a
vu32     a; //定义易变的32位无符号变量a
vu16     a; //定义易变的 16位无符号变量a
vu8     a; //定义易变的 8位无符号变量a
uc32     a; //定义只读的32位无符号变量a
uc16     a; //定义只读 的16位无符号变量a
uc8     a; //定义只读 的8位无符号变量a

#define ONE  1   //宏定义

delay_us(1); //延时1微秒
delay_ms(1); //延时1毫秒
delay_s(1); //延时1秒

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED控制

*/



