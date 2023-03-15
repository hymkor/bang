#pragma inline

#include <stdlib.h>
#include "joy.h"

/*	ウィンドウのサイズは、８キロバイト
 *	8 * 1024 = 1 << (3+10)
 */

#define FONT_BANK_REG		0x1160	/* word */
#define WINDOW_RELOC_REG	0x1162	/* byte */
#define WINDOW_CTRL_REG 	0x1163
#define BUS_MODE_CTRL_REG	0x1164
#define STATUS_SENSE_REG	0x1165
#define WINDOW_BANK_REG		0x1166

#define JOY_CTRL_0	0xA10009L
#define JOY_DATA_0	0xA10003L

#define JOY_CTRL_1	0xA1000BL
#define JOY_DATA_1	0xA10005L

static char 	window_ctrl_save;
static short	window_bank_save;

extern int dummy_for_joy;

/** この文字列はどこからも参照されていないので、warningが出るが、決して消してはいけない。 **/
static	char	*secustr = "PRODUCED BY OR UNDER LICENSE FROM SEGA ENTERPRISES Ltd." ;

void	sega_security_unlock( void )
{
	// BANK <- FFEh
	asm mov dx,WINDOW_BANK_REG
	asm mov ax,0x0FFE
	asm out dx,ax
	
	// FONT BANK <- 21h
	asm mov dx,FONT_BANK_REG
	asm mov al,0x21
	asm out dx,al
	
	// WINDOW CTRL <- 01h
	asm mov dx,WINDOW_CTRL_REG
	asm mov al,0x01
	asm out dx,al
	
	// BUS MODE CTRL <- 81h
	asm mov dx,BUS_MODE_CTRL_REG
	asm mov al,0x81
	asm out dx,al
	
	for(int i = 0; i< 3000; i++ )
		;
	
	// BANK <- data segment
	asm mov dx,WINDOW_BANK_REG
	asm mov ax,0
	asm out dx,ax
	
	// FONT BANK <- 21h
	asm mov dx,FONT_BANK_REG
	asm mov al,0x21
	asm out dx,al
	
	// WINDOW CTRL <- 01h
	asm mov dx,WINDOW_CTRL_REG
	asm mov al,0x01
	asm out dx,al
	
	// BUS MODE CTRL <- 81h
	asm mov dx,BUS_MODE_CTRL_REG
	asm mov al,0x81
	asm out dx,al
	
	for ( i = 0; i< 3000; i++ )
		;
}

void sega_security_lock( void )
{
	asm mov dx,WINDOW_BANK_REG
	asm mov ax,0x0ffc
	asm out dx,ax
	
	asm mov dx,FONT_BANK_REG
	asm mov al,0x21
	asm out dx,al
	
	asm mov dx,WINDOW_CTRL_REG
	asm mov al,0x01
	asm out dx,al
	
	asm mov dx,BUS_MODE_CTRL_REG
	asm mov al,0x81
}

void sega_joy_init( void )
{
	static int first=1;
	if( first ){
		sega_security_unlock();
		atexit( sega_security_lock );
		first = 0;
	}
}

unsigned char far *get_window_address(void)
{
	unsigned short adrs;
	
	asm mov dx,WINDOW_RELOC_REG
	asm in al,dx
	asm xor ah,ah
	asm mov adrs,ax
	
	return (unsigned char far *)((unsigned long)adrs << 24);
}
unsigned open_bank(unsigned long address)
{
	unsigned short
		bank	= (unsigned short)( (address >> 12) & ~1	),
		offset	= (unsigned short)(  address & 0x1FFF		);
	
	/* ウィンドウコントロール
	 *		ウィンドウ使用許可
	 *	  	６８Ｋメモリ空間選択
	 */
	asm mov dx,WINDOW_CTRL_REG
	//	 (save)
	asm in	al,dx
	asm mov window_ctrl_save,al
	//	 (set)
	asm or	al,0x03
	asm out dx,al

	
	/*** バンクセット ***/
	asm mov dx,WINDOW_BANK_REG
	/*** (save) ***/
	asm in	ax,dx
	asm mov window_bank_save,ax
	/*** (set) ***/
	asm mov ax,bank
	asm out dx,ax
	
	return offset;
}
void close_bank(void)
{
	// バンク復帰
	asm mov dx,WINDOW_BANK_REG
	asm mov ax,window_bank_save
	asm out dx,ax
	
	// ウィンドウコントロール復帰
	asm mov dx,WINDOW_CTRL_REG
	asm mov al,window_ctrl_save
	asm out dx,al
}

void poke68k(unsigned long address, unsigned char data)
{
	( get_window_address() )[ open_bank( address ) ]
		= data;
	close_bank();
}

unsigned char peek68k(unsigned long address)
{
	unsigned char
		result = ( get_window_address() )[ open_bank( address ) ];
	close_bank();
	
	return result;
}
const static short joytable[16][2]={
	{ 0,0},{ 0, 1},{ 0,-1},{ 0, 0},
	{ 1,0},{ 1, 1},{ 1,-1},{ 1, 0},
	{-1,0},{-1, 1},{-1,-1},{-1, 0},
	{ 0,0},{ 0, 1},{ 0,-1},{ 0, 0},
};

unsigned sega_joystick0(int &dx,int &dy)
{
	unsigned th1,th0;
	
	poke68k( JOY_CTRL_0 , 0x40 );
	
	poke68k( JOY_DATA_0 , 0x40 );
	th1 = peek68k( JOY_DATA_0 );

	poke68k( JOY_DATA_0 ,   0  );
	th0 = peek68k( JOY_DATA_0 );

	dx = joytable[ th1 & 0xF ][0];
	dy = joytable[ th1 & 0xF ][1];
	return (  ((th1>>4)&3) | ((th0>>2)&12)   )^0xF ;
}

unsigned sega_joystick1(int &dx,int &dy)
{
	unsigned th1,th0;
	
	poke68k( JOY_CTRL_1 , 0x40 );
	
	poke68k( JOY_DATA_1 , 0x40 );
	th1 = peek68k( JOY_DATA_1 );
	poke68k( JOY_DATA_1 ,   0  );
	th0 = peek68k( JOY_DATA_1 );

	dx = joytable[ th1 & 0xF ][0];
	dy = joytable[ th1 & 0xF ][1];
	return (  ((th1>>4)&3) | ((th0>>2)&12)   )^0xF ;
}
