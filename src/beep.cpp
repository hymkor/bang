#pragma inline
#include "music.h"

static int level = 0;

const unsigned short   beep_table[]={
	36488,34440,32507,30682,28960,27334,25801,24353,22986,21696,20478,19561
};

void beep_off( void )
{
	asm in al,0x61
	asm and al,0xfc
	asm out 0x61,al
	asm or al,2
	asm out 0x61,al

	level = 0;
}

void beep_on(unsigned n)
{
/* dncount”‚ğƒZƒbƒg */
	asm mov al,0xb6
	asm out 0x43,al
	asm mov ax,n
	asm out 0x42,al
	asm mov al,ah
	asm out 0x42,al
	
/* beep_on */
	asm in al,0x61
	asm or al,0x03
	asm out 0x61,al
}

void beep( int sw )
{
	if( sw ){
		beep_on(400);
		level++;
	}else{
		if( --level <= 0 )
			beep_off();
	}
}
