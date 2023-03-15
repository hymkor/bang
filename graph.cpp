#pragma inline
#include "hayos.h"

#undef	VWIDTH	320
#define VWIDTH	320

void box_fill(int x,int y,int width,int height,int color)
{
	unsigned short adrs = x + VWIDTH * y + vram_offset;
	
	asm push es
	
	asm mov ax,0xa000
	asm mov es,ax
	asm mov di,adrs
	asm mov al,byte ptr color

	asm mov si,width

	asm mov bx,320
	asm sub bx,si	// bx ... スキップ数
	
	asm mov dx,height
	
L0:	asm mov cx,si
	asm rep stosb
	asm add di,bx

	asm dec dx
	asm jne L0

	asm pop es
}

void xor_box_fill(int x,int y,int width,int height,int color)
{
	unsigned short adrs = x + VWIDTH * y + vram_offset;
	
	asm push es
	
	asm mov ax,0xa000
	asm mov es,ax
	asm mov di,adrs
	asm mov ah,byte ptr color

	asm mov si,width

	asm mov bx,320
	asm sub bx,si	// bx ... スキップ数
	
	asm mov dx,height
	
L0:	asm mov cx,si
L1:	asm mov al,ah
	asm xor al,es:[di]
	asm stosb
	asm loop L1
	asm add di,bx

	asm dec dx
	asm jne L0

	asm pop es
}

void xor_box(int x,int y,int width,int height,int color)
{
	for(int i=0; i<=width;  i++){
		GRAM[ y        ][ x+i ] ^= color;
		GRAM[ y+height ][ x+i ] ^= color;
	}
	for(    i=1; i<height; i++){
		GRAM[ y+i ][ x       ] ^= color;
		GRAM[ y+i ][ x+width ] ^= color;
	}
}