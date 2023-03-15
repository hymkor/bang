#pragma inline

#include "palette.h"

#define STATUS			0x3c7
#define READ_REG_NO		0x3c7
#define WRITE_REG_NO	0x3c8
#define	DATA			0x3c9

void Palette::write(int code)
{
	if( code == -1 )
		code = Palette::code;

/* 変更するパレットコードをセット */
	asm mov dx,WRITE_REG_NO
	asm	mov	al,byte ptr code
	asm out dx,al
	
/* RGBをセット */
	asm	mov dx,DATA
	asm mov bx,this
	asm mov al,[bx].red
	asm out dx,al
	asm mov al,[bx].green
	asm out dx,al
	asm mov al,[bx].blue
	asm out dx,al
}
void Palette::read(int code)
{
	if( code == -1 )
		code = Palette::code;

/* 変更するパレットコードをセット */
	asm mov dx,READ_REG_NO
	asm	mov	al,byte ptr code
	asm out dx,al
	
/* RGBをセット */
	asm	mov dx,DATA
	asm mov bx,this
	asm in	al,dx
	asm mov [bx].red,al
	asm in	al,dx
	asm mov [bx].green,al
	asm in	al,dx
	asm mov [bx].blue,al
}
