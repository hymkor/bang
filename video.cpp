#pragma inline
#include <stddef.h>
#include "hayos.h"

void set_video_mode(unsigned mode)
{
	asm xor ah,ah
	asm mov al,byte ptr mode
	asm int 0x10
}

unsigned get_video_mode()
{
	asm mov ah,0x0f
	asm int 0x10
	asm xor ah,ah
	return _AX;
}
#ifndef VGA_DEF_OFF
static unsigned short get_vga_info_size(unsigned short flag)
{
	asm mov ax,0x1c00
	asm mov cx,flag
	asm int 0x10
	return _BX;
}

static void save_vga_info( void *buffer , unsigned short flag )
{
	asm push es
		asm mov ax,0x1c01
		asm mov cx,flag
		asm mov bx,buffer
		asm mov dx,ds
		asm mov es,dx
		asm int 0x10
	asm pop es
}
#endif

VgaSave::VgaSave( int _flag )
{
	buffer = new char[ size = get_vga_info_size(flag = _flag)*64 ];
	if( buffer != 0 ){
		save_vga_info( buffer , _flag );
	}
	video_mode = get_video_mode();
	
}

VgaSave::~VgaSave( void )
{
	if( buffer ){
		asm push es
			asm mov si,this
			asm mov ax,0x1c02
			asm mov cx,[si].flag
			asm mov bx,[si].buffer
			asm mov dx,ds
			asm mov es,dx
			asm int 0x10
		asm pop es
		delete [size]buffer;
	}
	set_video_mode( video_mode );
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


#if 0
void set_line_compare(unsigned short line)
{
// upper 1 bit
	int upper = get_crtc_register(0x9);
	if( (line & (1<<8))!=0 ){
		set_crtc_register(	0x9,	upper |  (1<<6) );
	}else{
		set_crtc_register(	0x9,	upper & ~(1<<6) );
	}
// lower 8 bit
	set_crtc_register(0x18,line & 0xFF);
}

/***** 13hê—p *****/
void pset(unsigned x,unsigned y,int color)
{
	size_t adrs = x + 320 * y ;
	
	asm push es
	
	asm mov ax,0xa000
	asm mov es,ax
	asm mov di,adrs
	asm mov ax,color
	
	asm stosb
	
	asm pop es
}
#endif
