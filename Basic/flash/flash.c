
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


*/



#include "flash.h"

//FLASHд������
void FLASH_W(u32 add,u16 dat){ //����1��32λFLASH��ַ������2��16λ����
//	 RCC_HSICmd(ENABLE); //��HSIʱ��
	 FLASH_Unlock();  //����FLASH��̲���������
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
     FLASH_ErasePage(add);     //����ָ����ַҳ
     FLASH_ProgramHalfWord(add,dat); //��ָ��ҳ��addr��ַ��ʼд
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
     FLASH_Lock();    //����FLASH��̲���������
}

//FLASH��������
u16 FLASH_R(u32 add){ //����1��32λ����FLASH��ַ������ֵ��16λ����
	u16 a;
    a = *(u16*)(add);//��ָ��ҳ��addr��ַ��ʼ��
return a;

}


void ALFLASH_W (u8* alhour,u8* almin,u32 add,u8 size){
	u16 t;
	u8 i = 0;
	
	FLASH_Unlock();  //����FLASH��̲���������
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
	FLASH_ErasePage(add);     //����ָ����ַҳ
	for(;i<size;i++){
		t = alhour[i]*0x100 + almin[i];
		FLASH_ProgramHalfWord(add+i*2,t);
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
}
	
	FLASH_Lock();   
}

void TEMPFLASH_W(u16* temps,u32 add,u8 size){
	u8 i = 0;
	FLASH_Unlock();  //����FLASH��̲���������
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
	FLASH_ErasePage(add);     //����ָ����ַҳ
	for(;i<size;i++){
		FLASH_ProgramHalfWord(add+i*2,temps[i]);
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
}
	
	FLASH_Lock();   

}

/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/































