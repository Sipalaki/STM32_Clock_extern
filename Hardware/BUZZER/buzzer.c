

/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT 
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��
1-201709012309 ������


*/


#include "buzzer.h"
#include "delay.h"

void BUZZER_Init(void){ //�������Ľӿڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = BUZZER; //ѡ��˿ں�                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(BUZZERPORT, &GPIO_InitStructure);	
	
	GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1		
}

void BUZZER_BEEP1(void){ //��������һ��
	u16 i;
	for(i=0;i<50;i++){
//		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
		delay_us(500); //��ʱ		
//		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
		delay_us(500); //��ʱ		
	}
}

void BUZZER_BEEP2(void){ //����
	u16 i;
	for(i=0;i<500;i++){
//		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
		delay_us(500); //��ʱ		
//		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
		delay_us(500); //��ʱ		
	}
}



uc16 music1[78]={ //����1�����ݱ���������������ż���ǳ��ȣ�
330,750,
440,375,
494,375,
523,750,
587,375,
659,375,
587,750,
494,375,
392,375,
440,1500,
330,750,
440,375,
494,375,
523,750,
587,375,
659,375,
587,750,
494,375,
392,375,
784,1500,
659,750,
698,375,
784,375,
880,750,
784,375,
698,375,
659,750,
587,750,
659,750,
523,375,
494,375,
440,750,
440,375,
494,375,
523,750,
523,750,
494,750,
392,750,
440,3000
};

void MIDI_PLAY(void){ //MIDI����
	u16 i,e;
	for(i=0;i<39;i++){
		for(e=0;e<music1[i*2]*music1[i*2+1]/1000;e++){
			GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
			delay_us(500000/music1[i*2]); //��ʱ		
			GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
			delay_us(500000/music1[i*2]); //��ʱ	
		}	
	}
}
/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/


/*
ѡ��IO�ӿڹ�����ʽ��
GPIO_Mode_AIN ģ������
GPIO_Mode_IN_FLOATING ��������
GPIO_Mode_IPD ��������
GPIO_Mode_IPU ��������
GPIO_Mode_Out_PP �������
GPIO_Mode_Out_OD ��©���
GPIO_Mode_AF_PP �����������
GPIO_Mode_AF_OD ���ÿ�©���
*/