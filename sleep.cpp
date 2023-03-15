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

} *atsleep_begin = NULL , *atsleep_always = NULL , *atsleep_end = NULL ;


static unsigned get_vsync_reg( void )
{
	asm mov dx,0x3da
	asm in al,dx
	asm and al,8
	asm xor ah,ah
	return _AX;
}

static void call_atsleep_func( atsleep_list_t *ptr )
{
	while( ptr != NULL ){
		(*ptr->func)();
		ptr = ptr -> next;
	}
}

void vsync_sleep( void )		/* 1/60 •b */
{
	call_atsleep_func( atsleep_begin );
	while( get_vsync_reg() == 0 )
		call_atsleep_func( atsleep_always );
	call_atsleep_func( atsleep_end );
}

void irq0_sleep(unsigned int useconds) /* 1/18.2 •b */
{
	unsigned long timer = get_interval_timer() + useconds;
	
	call_atsleep_func( atsleep_begin );
	while( get_interval_timer() < timer )
		call_atsleep_func( atsleep_always );
	call_atsleep_func( atsleep_end );
}

void sleep(unsigned int seconds) /* ‚P•b */
{
	unsigned long timer = get_interval_timer() + seconds*91/5 ;

	call_atsleep_func( atsleep_begin );
	while( get_interval_timer() < timer )
		call_atsleep_func( atsleep_always );
	call_atsleep_func( atsleep_end );
	
}


int atsleep(void (*func)(void) , int flag )
{
	atsleep_list_t *neo = new atsleep_list_t;
	if( neo == NULL )	return 1;

	neo->func = func;

	if( flag < 0 ){
		neo->next = atsleep_begin;
		atsleep_begin = neo;
	}else if( flag > 0 ){
		neo->next = atsleep_end;
		atsleep_end = neo;
	}else{
		neo->next = atsleep_always;
		atsleep_always = neo;
	}

	return 0;
}
