#pragma inline
#include <alloc.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "hayos.h"
#include "sprite.h"

       char far *background = 0 ;
static char far *original_BGRAM = 0;

static void BGRAM_free( void )
{
	if( original_BGRAM != 0 )
		farfree( original_BGRAM );
}

int background_init(void)
{
	if( (original_BGRAM = (char far *)farmalloc( 65536L+16 ) ) == 0 )
		return 1;
	
	FpSeg(background) = FpSeg(original_BGRAM) + (FpOff(original_BGRAM)>>4) + 1;
	FpOff(background) = 0;
	
	asm push es
	asm push di
	asm pushf

	asm   les di,background
	asm   mov cx,32767
	asm   xor ax,ax
	asm   cld
	asm   rep stosw

	asm popf
	asm pop di
	asm pop es
	
	atexit( BGRAM_free );
	return 0;
}

Pattern Sprite::load(const char *fname )
{
	FILE *fp = fopen( fname , "rb" );
	if( fp==NULL ) return NOPATTERN;
	Pattern buffer = (Pattern)farmalloc( 2000 );
	size_t nbytes=0;

	if( buffer == NULL ){
		fclose( fp );
		return NULL;
	}
	
	Pattern ptr = buffer;
	while( !feof(fp) ){
		*ptr++ = getc(fp);
		nbytes++;
	}
	fclose(fp);
	return (Pattern)farrealloc(buffer,nbytes);
}

short Sprite::draw(unsigned short x,unsigned short y,Pattern pattern)
{
	if( pattern==NOPATTERN ) return 0;
	unsigned short adrs = x + y*320;
	
	asm push di
	asm push si
	asm push es
	asm mov dx,VRAM_END
	asm mov di,adrs
	asm add di,vram_offset
	asm push di

	asm lds si,pattern
	asm mov ax,0xa000
	asm mov es,ax
	
// メインループ
lp1:asm lodsw
	asm add di,ax

	asm lodsw
	asm and ax,ax
	asm mov cx,ax

	asm je end
	asm rep movsb
	asm jmp lp1
	
end:asm mov ax,ss
	asm mov ds,ax
	
	asm pop ax
	asm pop es
	asm pop si
	asm pop di
	
	return _AX;
}


void Sprite::clear( short vramptr , Pattern pattern )
{
	if( pattern==NOPATTERN ) return;

/*	es:di...GRAMポインタ
 *	bx:di...Back Ground Buffer
 *	dx:si...消すパターン
 *		ds を適宜切り換える
 *
 */
	asm push di
	asm push si
	asm push es
	asm push ds
	
	asm mov di,vramptr

	asm mov ax,0xa000
	asm mov es,ax
	
	asm mov bx,word ptr background[2]
	asm mov dx,word ptr pattern[2]
	asm mov si,word ptr pattern[0]

/*** メインループ ***/
lp1:asm mov ds,dx	/* dsをpatternに切り換える */
	asm lodsw
	asm add di,ax
	asm lodsw
	asm and ax,ax	/* ==0 : 終わり */
	asm je end
	asm add si,ax	/* 消すパターンを飛ばしておく */
	
	asm mov ds,bx	/* dsをbackgroundに切り換える */
	asm mov cx,ax
lp2:asm mov al,ds:[di]
	asm stosb
	asm loop lp2
	asm jmp lp1
	
end:
	asm pop ds
	asm pop es
	asm pop si
	asm pop di
}

void Sprite::putat(int X,int Y,Pattern P)
{
	clear( vramptr , pattern );
	if( P != NOPATTERN ) pattern=P;
	vramptr = draw( x=X , y=Y , pattern );
}
