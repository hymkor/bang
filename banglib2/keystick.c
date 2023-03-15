#include <conio.h>	/* for kbdhit(),getch() */
#include "joy.h"

/* 
 *	キーテーブル	(きちんとした資料ではなく,ＩＯポートから直接読みだした値なので,ポータビリティは極めて悪い.)
 */
enum{
	TENKEY_2	= 80,
	TENKEY_4	= 75,
	TENKEY_6	= 77,
	TENKEY_8	= 72,
	
	RETURN_KEY	= 28,
	SPACE_KEY	= 57,
	
	CTRL_KEY	= 29,
	ALT_KEY		= 56,
};

enum{
	LEFT	= TENKEY_4,
	RIGHT	= TENKEY_6,
	UP		= TENKEY_8,
	DOWN	= TENKEY_2,
	TRIG1	= CTRL_KEY,
	TRIG2	= ALT_KEY,
};

unsigned keystick( int *deltaX, int *deltaY)
{
	unsigned result = 0;
	int dx,dy;
	static int not_initialized_flag = 1 ;
	
	if( not_initialized_flag ){
		keyscan_init();
		not_initialized_flag = 0;
	}
	
	if( kbhit() ) getch();	/* キーバッファにたまって五月蝿いから */
	
	if( scantable[ LEFT  ] )	dx = -1;	else	dx = 0;
	if( scantable[ RIGHT ] )	++dx;
	
	if( scantable[ UP    ] )	dy = -1;	else	dy = 0;
	if( scantable[ DOWN  ] )	++dy;
	
	if( scantable[ TRIG1 ] )	result  = 1;
	if( scantable[ TRIG2 ] )	result |= 2;
	
	if( deltaX != NULL )	*deltaX = dx;
	if( deltaY != NULL )	*deltaY = dy;
	
	return result;
}
