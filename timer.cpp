#pragma inline
#include <stddef.h>

unsigned long get_interval_timer( void )
{
	unsigned long result;
	
	asm push ds
	asm push si

		asm mov ax,0x40
		asm mov ds,ax
		asm mov si,0x6C
		asm lodsw
		asm mov dx,ax
		asm lodsw

	asm pop si
	asm pop ds
	
	asm mov word ptr result[0],dx
	asm mov word ptr result[2],ax

	return result;
}
struct atsleep_list_t {
	void (*func)(void);
	atsleep_list_t *next;
} *atsleep_list = NULL;

static unsigned get_vsync_reg( void )
{
	asm mov dx,0x3da
	asm in al,dx
	asm and al,8
	asm xor ah,ah
	return _AX;
}

static void call_atsleep_func( void )
{
	atsleep_list_t *ptr = atsleep_list;
	while( ptr != NULL ){
		(*ptr->func)();
		ptr = ptr -> next;
	}
}

void vsync_sleep( void )
{
	while( get_vsync_reg() == 0 )
		call_atsleep_func();
	
	while( get_vsync_reg() != 0 )
		call_atsleep_func();
}

void usleep(unsigned int useconds)
{
	unsigned long timer = get_interval_timer() + useconds;
	
	while( get_interval_timer() < timer )
		call_atsleep_func();
}


void sleep(unsigned int seconds)
{
	usleep( seconds * 60 );
}


int atsleep(void (*func)(void) )
{
	atsleep_list_t *neo = new atsleep_list_t;
	if( neo == NULL )	return 1;
	
	neo->next = atsleep_list;
	neo->func = func;
	
	atsleep_list = neo;
	return 0;
}
