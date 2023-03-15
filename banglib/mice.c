#pragma inline

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

int mouse( int *x, int *y )	/* 座標系は、320*200基準。*/
{
	asm mov ax,3
	asm int 0x33
	asm shr cx,1 /* 他の座標系にしたいときは、ここを変更 */

	asm push di

	asm mov	di,x
	asm and di,di
	asm jz	no_x
	asm	mov	[di],cx
no_x:
	asm mov di,y
	asm and di,di
	asm jz	no_y
	asm mov	[di],dx
no_y:
	asm	pop	di

	return _BX;
}
