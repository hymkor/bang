#pragma inline
#define MOUSE_COM

int dummy_for_mouse;

int mouse_init(void)
{
	asm xor ax,ax
	asm int 0x33
	return !_AX;
}

void mouse_on(void)
{
	asm mov ax,1
	asm int 0x33
}

void mouse_off(void)
{
	asm mov ax,2
	asm int 0x33
}

int mouse( int &x, int &y )
{
	int result;
	asm mov ax,3
	asm int 0x33
	asm shr cx,1
	
	result	= _BX;
	x		= _CX;
	y		= _DX;
	
	return result;
}
void mouse_setpos(int x,int y)
{
	asm mov cx,x
	asm mov dx,y
	asm mov ax,4
	asm int 0x33
}
#if 0

void mouse_setplane(int plane)
{
	asm mov bx,plane
	asm mov ax,0x12
}


void mouse_setcursor(int x,int y,int plane,void *pattern)
{
// bit map を定義
	asm mov ax,9
	asm mov bx,x
	asm mov cx,y
	asm mov dx,pattern
	asm push es
	asm push ds
	asm pop es
	asm int 0x33
	asm pop es
// 表示するプレーンを定義
	asm mov bx,plane
	asm mov ax,0x12
}
#endif
