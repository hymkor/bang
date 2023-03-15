#pragma inline
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hayos.h"
#include "map.h"

#define VRAM_END 	320*200
#define CELL_SIZE 	64

char far *cell_graphics = 0;

static void cell_free(void)
{
	if( cell_graphics != 0 )
		farfree( cell_graphics );
	cell_graphics = 0;
}

int cell_load(const char *fname)
{
	FILE *fp=fopen(fname,"rb");
	if( fp == 0 ) return 1;
	
	if( cell_graphics != 0 )
		farfree( cell_graphics );
	else
		atexit( cell_free );

	char far *ptr=cell_graphics=(char far *)farmalloc( 256*CELL_SIZE );
	if( ptr == 0 ){
		fclose( fp );
		return 1;
	}

	for(int i=0; i<256*CELL_SIZE && !feof(fp); i++)
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
int Map::clear(void)
{
	if( map != NULL ){
		memset( map , 0 , nlines * sizeof( MapScroll )  );
		return 0;
	}else{
		return 1;
	}
}

int Map::alloc(size_t n)
{
	/* 初めて呼ばれた時は,そのサイズで初期化する.*/
	if( map == NULL ){
		map = (MapScroll*) malloc( n * sizeof(MapScroll) );
		clear();
	
	/* さもなければ,サイズを変更する.*/
	}else{
		map = (MapScroll*) realloc( map,  n * sizeof(MapScroll)  );
		if( n > nlines )
			memset( map + nlines , 0 , (n-nlines)*sizeof(MapScroll) );
	}
	nlines = n;
	
	if( map==NULL){
		nlines = 0;
		return 1;
	}
	return 0;
}

int Map::load(const char *fname)
{
	size_t size;
	FILE *fp=fopen(fname,"rb");
	if( fp == NULL )
		return 1;

	if( fread( &size, sizeof(size), 1, fp ) > 0   &&  alloc(size) == 0 )
		fread( map, sizeof(MapScroll), nlines, fp);

	fclose( fp );
	return 0;
}

int Map::save(const char *fname)
{
	FILE *fp=fopen(fname,"wb");
	if( fp==NULL )
		return 1;

	fwrite( &nlines, sizeof( nlines ), 1, fp);
	fwrite( map, sizeof(MapScroll), nlines , fp);

	fclose( fp );
	return 0;
}
