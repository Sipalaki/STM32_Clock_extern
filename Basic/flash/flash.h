#ifndef __FLASH_H
#define __FLASH_H 			   
#include "sys.h"
 

void FLASH_W(u32 add,u16 dat);
u16 FLASH_R(u32 add);
void ALFLASH_W (u8* alhour,u8* almin,u32 add,u8 size);
void TEMPFLASH_W(u16* temps,u32 add,u8 size);
#endif





























