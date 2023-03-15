#pragma inline
#include "hayos.h"

static char far *romfont = (char far * )0 ;

static void get_romfont_address( void )
{
	asm	push es
	asm	push bp

	asm	mov ax,0x1130
	asm	mov bh,0x03

	asm	int 0x10

	asm	mov word ptr romfont[0] , bp
	asm	mov word ptr romfont[2] , es
	
	asm	pop bp
	asm	pop es
}

void symbol(int x,int y,const char *string,int xtimes,int ytimes,int color)
{
	if( romfont == 0 ) get_romfont_address();
	
	while( *string != '\0' ){
		char far *font = romfont + *string * 8;
		int X,Y;
		
		Y=y;
		for(int i=0 ; i<8 ; i++){
			int linpat = *font++;
			
			X=x;
			for(int i=0; i<8; i++ ){
				if( (linpat <<= 1) & 256 ){
					box_fill( X , Y , xtimes , ytimes , color );
				}
				X += xtimes;
			}
			Y += ytimes;
		}
		x = X;
		string++;
	}
}
