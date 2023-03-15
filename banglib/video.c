#pragma inline

void set_video_mode(unsigned mode)
{
	asm xor ah,ah
	asm mov al,byte ptr mode
	asm int 0x10
	vram_offset = 0;
}

unsigned get_video_mode()
{
	asm mov ah,0x0f
	asm int 0x10
	asm xor ah,ah
	return _AX;
}

void set_crtc_register(unsigned short address,unsigned short data)
{
	asm mov dx,0x3d4
	asm mov al,byte ptr address
	asm out dx,al
	
	asm mov dx,0x3d5
	asm mov al,byte ptr data
	asm out dx,al
}

int get_crtc_register(unsigned short address)
{
	asm mov dx,0x3d4
	asm mov al,byte ptr address
	asm out dx,al
	
	asm mov dx,0x3d5
	asm in  al,dx
	asm xor ah,ah
	
	return _AX;
}	

unsigned short vram_offset=0;

void set_start_address(unsigned short address)
{
	set_crtc_register( 0xC , address >> 8  );
	set_crtc_register( 0xD , address & 0xFF);
}

void set_horizontal_planning( unsigned short npixel )
{
	/* set address-mode */
	asm mov dx,0x3da
	asm in al,dx

	/* set address */
	asm mov dx,0x3c0
	asm in	al,dx
	asm	and	al,0xe0
	asm or	al,0x13
	asm out dx,al

	/* set data */
	asm mov dx,0x3c0
	asm in	al,dx
	asm and al,0xF0
	asm or	al,byte ptr npixel
	asm out dx,al
}

void add_vram_offset_13h( signed int delta )
{
	vram_offset += delta;

	set_start_address( vram_offset >> 2 );
	set_horizontal_planning( (vram_offset & 3) << 1 );
}
