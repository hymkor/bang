#pragma inline

#include <string.h>
#include <ctype.h>

#include "hayos.h"

void writes(int x,int y,const char *string,int color,int page)
{
	size_t len=strlen(string);
	
	asm mov dh,byte ptr y
	asm mov dl,byte ptr x

	asm mov bh,byte ptr page
	asm mov bl,byte ptr color
	asm mov cx,len

	// String Ptr (segment)
	asm mov ax,ds
	asm push es
	asm mov es,ax
	//    ...     (offset)
	asm mov ax,string
	asm push bp
	asm mov bp,ax

	asm mov ax,0x1301
	asm int 0x10

	asm pop bp
	asm pop es
}
#if 0

/***** バックスペースのみの入力ルーチン ******/

size_t line_input(int x,int y,char *buffer,size_t size)
{
	size_t len=0;
	int ch;
	
	while( (ch=Getch()) != '\r' ){
		if( ch=='\b' && len > 0 ){
			buffer[   len ]='\0';
			buffer[ --len ]= ' ';
			writes(x,y,buffer);
		}else if( isprint( ch ) && len < size ){
			buffer[   len ]= ch;
			buffer[ ++len ]='\0';
			writes(x,y,buffer);
		}
	}
	buffer[len]='\0';
	return len;
}
size_t line_input(int x,int y,const char *prompt,char *buffer,size_t size)
{
	writes( x, y , prompt );
	x += strlen( prompt );
	return line_input(x,y,buffer,size);
}
#endif

#define ctrl(c)		(c & 0x1F)

int input(int x,int y,char *buffer,size_t size)
{
	size_t len=strlen(buffer),pos=len;
	int ch;
	
	writes( x , y , buffer );
	xor_box_fill( (x+pos)<<3 , y<<3 , 8, 8 ,9);

	while(  (ch = Getch() ) != '\r' ){
		xor_box_fill( (x+pos)<<3 , y<<3 , 8, 8 ,9);

		switch( ch ){

		case ctrl('d'):
			if( pos == len )
				break;
			++pos;
			/* continue to next case */

		case '\b':
			if( pos > 0 ){ /* 0 <= pos <= len < size */
				for(int i=--pos; i<len; i++)
					buffer[ i ] = buffer[ i+1 ];
				--len;
				writes( x+pos , y , &buffer[pos] );
				box_fill( (x+len)<<3 , y<<3 , 8 , 8 , 0 );
			}
			break;

		case ctrl('b'):	if( pos >  0  ) {--pos;}		break;

		case ctrl('f'):	if( pos < len ) {++pos;}		break;

		case ctrl('a'):	pos = 0;	break;

		case ctrl('e'):	pos = len;	break;

		case ctrl('g'):
		case ctrl('['):	return -1;

		default:
			if( isprint( ch )  &&  len < size ){
				for(int i=++len ; i > pos; --i)
					buffer[ i ] = buffer[ i-1 ];
				buffer[ pos ] = ch;
				writes( x + pos++ , y , &buffer[pos] );
			}
			break;
		}
		
		xor_box_fill( (x+pos)<<3 , y<<3 , 8, 8 ,9);
	}
	return len;
}
int input(int x,int y,const char *prompt,char *buffer,size_t size)
{
	writes( x, y , prompt );
	x += strlen( prompt );
	return input(x,y,buffer,size);
}
