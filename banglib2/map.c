#pragma inline
#include <alloc.h>	/* for farmalloc,farfree */
#include <stdio.h>
#include <stdlib.h>	/* atexit */

#include "map.h"

#define CELL_SIZE 	64	/* = 8 dots Å~ 8 lines */

char far *cell_graphics = (char far *)0;

static void cell_free(void)
{
	if( cell_graphics != 0 )
		farfree( cell_graphics );
	cell_graphics = 0;
}

int cell_load(const char *fname)
{
	int i;
	char far *ptr;
	FILE *fp=fopen(fname,"rb");
	
	if( fp == 0 ) return 1;
	
	if( cell_graphics != 0 )
		farfree( cell_graphics );
	else
		atexit( cell_free );

	ptr=cell_graphics=(char far *)farmalloc( 256*CELL_SIZE );
	if( ptr == (char far*)0 ){
		fclose( fp );
		return 1;
	}

	for(i=0; i<256*CELL_SIZE && !feof(fp); i++)
		*ptr++ = getc(fp);

	fclose( fp );
	return 0;
}

void cell_putat(unsigned x,unsigned y,unsigned type)
{
	unsigned short adrs = x + 320*y + vram_offset;

	asm	mov ax,type
	asm	and ax,0x0F
	asm	shl ax,1
	asm	shl ax,1
	asm	shl ax,1

	asm	mov dx,type
	asm	and dx,0xF0
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1

	asm	add dx,ax

	asm	push es
	asm	push ds

	asm	mov ax,0xa000
	asm	mov es,ax
	asm	mov di,adrs
	
	asm	lds si,cell_graphics
	asm	add si,dx
	
	asm mov ax,8
lp1:asm mov cx,8	// 8bytes movs
	asm rep movsb
	
	asm add si,128-8
	asm add di,320-8
	
	asm dec ax
	asm jne lp1
	
	asm pop ds
	asm pop es
	
}
