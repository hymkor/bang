#pragma inline

unsigned Inputch()
{
	asm xor ah,ah
	asm int 0x16
	return _AX;
}

unsigned Kbhit()
{
	asm mov ah,1
	asm int 0x16
	asm jz  no
/* ZF:off ... 1 ‚ð•Ô‚· */
	asm mov ax,1
	asm jmp exit
/* ZF:on .... 0 ‚ð•Ô‚· */
no:	asm xor ax,ax
exit:
	return _AX;
}

unsigned ShiftScan()
{
	asm mov ah,2
	asm int 0x16
	asm xor ah,ah
	return _AX;
}
