#pragma inline
#include "joy.h"

unsigned joystick0(int &deltax,int &deltay)
{
	/* Stick input */
	asm mov ah,0x84
	asm mov dx,1
	asm int 0x15

	asm mov si,deltax
	asm mov [si],ax
	asm mov si,deltay
	asm mov [si],bx
	
	/* trig input */
	asm mov ah,0x84
	asm xor dx,dx
	asm int 0x15
	asm shr ax,4
	asm and ax,3

	return _AX;
}

unsigned joystick1(int &deltaX,int &deltaY)
{
	/* Stick input */
	asm mov ah,0x84
	asm mov dx,1
	asm int 0x15
	
	asm mov si,deltaX
	asm mov [si],cx
	asm mov si,deltaY
	asm mov [si],dx
	
	/* trig input */
	asm mov ah,0x84
	asm xor dx,dx
	asm int 0x15
	asm shr ax,6
	asm and ax,3

	return _AX;
}
