#include "hayos.h"

extern "C" void keyscan_init( void );
extern volatile char scantable[128];

/* 
 *	�L�[�e�[�u��	(������Ƃ��������ł͂Ȃ�,�h�n�|�[�g���璼�ړǂ݂������l�Ȃ̂�,�|�[�^�r���e�B�͋ɂ߂Ĉ���.)
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

unsigned keystick( int &dx, int &dy)
{
	static int not_initialized_flag = 1 ;
	if( not_initialized_flag ){
		keyscan_init();
		not_initialized_flag = 0;
	}
	
	if( Kbhit() ) Getch();
	
	if( scantable[ LEFT  ] )	dx = -1;	else	dx = 0;
	if( scantable[ RIGHT ] )	++dx;
	
	if( scantable[ UP    ] )	dy = -1;	else	dy = 0;
	if( scantable[ DOWN  ] )	++dy;
	
	unsigned result = 0;
	if( scantable[ TRIG1 ] )	result  = 1;
	if( scantable[ TRIG2 ] )	result |= 2;
	
	return result;
}
