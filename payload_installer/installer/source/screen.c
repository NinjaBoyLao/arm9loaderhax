#include "common.h"
#include "screen.h"

#include <string.h>

void screenInit(void){
	void ARM11Init(void){
		__asm(".word 0xF10C01C0");				//CPSID AIF @ Disable Interrupts
		*((vu32*)0x10141200) = 0x1007F;			// PDN_GPU_CNT
		*((vu32*)0x10202014) = 0x00000001;
		*((vu32*)0x1020200C) &= 0xFFFEFFFE;		// wtf register
		*((vu32*)0x10202244) = 0x1023E; 		// Top
		*((vu32*)0x10202A44) = 0x1023E;			// Bottom
	
		// Top screen
		*((vu32*)0x10400400) = 0x000001c2;
		*((vu32*)0x10400404) = 0x000000d1;
		*((vu32*)0x10400408) = 0x000001c1;
		*((vu32*)0x1040040c) = 0x000001c1;
		*((vu32*)0x10400410) = 0x00000000;
		*((vu32*)0x10400414) = 0x000000cf;
		*((vu32*)0x10400418) = 0x000000d1;
		*((vu32*)0x1040041c) = 0x01c501c1;
		*((vu32*)0x10400420) = 0x00010000;		// Comment for stripey screen
		*((vu32*)0x10400424) = 0x0000019d;		// Commenting this makes framebuffer noticeably slower (but works otherwise)
		*((vu32*)0x10400428) = 0x00000002;
		*((vu32*)0x1040042c) = 0x00000192;
		*((vu32*)0x10400430) = 0x00000192;
		*((vu32*)0x10400434) = 0x00000192;
		*((vu32*)0x10400438) = 0x00000001;
		*((vu32*)0x1040043c) = 0x00000002;
		*((vu32*)0x10400440) = 0x01960192;
		*((vu32*)0x10400444) = 0x00000000;
		*((vu32*)0x10400448) = 0x00000000;
		*((vu32*)0x1040045C) = 0x00f00190; 		// Framebuffer size
		*((vu32*)0x10400460) = 0x01c100d1;
		*((vu32*)0x10400464) = 0x01920002;
		*((vu32*)0x10400468) = 0x18300000;		// Left framebuffer 0
		*((vu32*)0x10400470) = 0x80341;			// Framebuffer format
		*((vu32*)0x10400474) = 0x00010501;
		*((vu32*)0x10400478) = 0;				// Framebuffer select
		*((vu32*)0x10400490) = 0x000002D0;		// Framebuffer stride
		*((vu32*)0x1040049C) = 0x00000000;
	
		// Disco register
		for(int i = 0; i < 256; i++)
		{
			*((vu32*)0x10400484) = 0x10101 * i;
		}
	
		// Bottom screen
		*((vu32*)0x10400500) = 0x000001c2;
		*((vu32*)0x10400504) = 0x000000d1;
		*((vu32*)0x10400508) = 0x000001c1;
		*((vu32*)0x1040050c) = 0x000001c1;
		*((vu32*)0x10400510) = 0x000000cd;
		*((vu32*)0x10400514) = 0x000000cf;
		*((vu32*)0x10400518) = 0x000000d1;
		*((vu32*)0x1040051c) = 0x01c501c1;
		*((vu32*)0x10400520) = 0x00010000;		// Comment for stripey screen
		*((vu32*)0x10400524) = 0x0000019d;		// Commenting this makes framebuffer noticeably slower (but works otherwise)
		*((vu32*)0x10400528) = 0x00000052;		// Commenting this moves the framebuffer to the right (for some reason)
		*((vu32*)0x1040052c) = 0x00000192;
		*((vu32*)0x10400530) = 0x00000192;
		*((vu32*)0x10400534) = 0x0000004f;
		*((vu32*)0x10400538) = 0x00000050;
		*((vu32*)0x1040053c) = 0x00000052;
		*((vu32*)0x10400540) = 0x01980194;
		*((vu32*)0x10400544) = 0x00000000;
		*((vu32*)0x10400548) = 0x00000011;
		*((vu32*)0x1040055C) = 0x00f00140;		// Framebuffer size
		*((vu32*)0x10400560) = 0x01c100d1;
		*((vu32*)0x10400564) = 0x01920052;
		*((vu32*)0x10400568) = 0x18300000 + 0x46500;  // Framebuffer 0
		*((vu32*)0x10400570) = 0x80301;			// Framebuffer format
		*((vu32*)0x10400574) = 0x00010501;
		*((vu32*)0x10400578) = 0;				// Framebuffer select
		*((vu32*)0x10400590) = 0x000002D0;  	// Framebuffer stride
		*((vu32*)0x1040059C) = 0x00000000;
	
		// Disco register
		for(int i = 0; i < 256; i++)
			*((volatile uint32_t*)0x10400584) = 0x10101 * i;
	
		// Enable backlight
		i2cWriteRegister(3, 0x22, 0b101010);
		
		memset((void*)0x18300000, 0, 0x46500*2);		
		*((vu32*)0x10400468) = 0x18300000;
		*((vu32*)0x1040046c) = 0x18300000;
		*((vu32*)0x10400494) = 0x18300000;
		*((vu32*)0x10400498) = 0x18300000;
		*((vu32*)0x10400568) = 0x18346500;
		*((vu32*)0x1040056c) = 0x18346500;
		
		// Save in memory
		*((vu32*)0x23FFFE00) = 0x18300000;
		*((vu32*)0x23FFFE04) = 0x18300000;
		*((vu32*)0x23FFFE08) = 0x18346500;
		
		*((vu32*)0x1FFFFFF8) = 0;
		while(!*((vu32*)0x1FFFFFF8));
		((void (*)())*((vu32*)0x1FFFFFF8))();
	}
	
	void ARM11(void){
		__asm(".word 0xF10C01C0");				//CPSID AIF @ Disable Interrupts
		
		*((vu32*)0x1FFFFFF8) = 0;
		while(!*((vu32*)0x1FFFFFF8));
		((void (*)())*((vu32*)0x1FFFFFF8))();
	}
	
	// In short terms, we initalize the screen just if it still haven't been.
	// We check if the framebuffer is set.
	if(!(*(u32*)0x23FFFE00 >= 0x18000000 && *(u32*)0x23FFFE00 < 0x18600000) && !(*(u32*)0x23FFFE00 >= 0x20000000 && *(u32*)0x23FFFE00 < 0x28000000))
	{
		*((vu32*)0x1FFFFFF8) = (u32)(isColdBoot ? ARM11Init : ARM11);
		*((vu32*)0x1FFFFFFC) = (u32)(isColdBoot ? ARM11Init : ARM11);
		for(volatile unsigned int i = 0; i < 0xF; ++i);
		while(*(volatile uint32_t *)0x1FFFFFF8 != 0);
	}
}

void screenExit(void){
	void ARM11(void){
		__asm(".word 0xF10C01C0");		//CPSID AIF @ Disable Interrupts
		
		*((vu32*)0x10202A44) = 0;
		*((vu32*)0x10202244) = 0;
		*((vu32*)0x1020200C) = 0;
		*((vu32*)0x10202014) = 0;
		
		*((vu32*)0x1FFFFFF8) = 0;
		while(!*((vu32*)0x1FFFFFF8));
		((void (*)())*((vu32*)0x1FFFFFF8))();
	}
	
	*((vu32*)0x1FFFFFF8) = (u32)ARM11;
	for(volatile unsigned int i = 0; i < 0xF; ++i);
	while(*(volatile uint32_t *)0x1FFFFFF8 != 0);
}