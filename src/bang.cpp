#include "hayos.h"
#include "sprite.h"
#include "enemy.h"
#include "joy.h"
#include "map.h"
#include "game.h"
#include "bang.h"
#include "pattern.h"

int burner_switch = 0;  /* ��0 : �΂񂮂ف[���̓X�N���[���ɗ�����Ă䂭  ,  ��0 : ������Ȃ����A�J�N�J�N���� */
int BangHoki::sound_switch = 0;
int hard_flag=0;

void BangHoki::runs( void )
{
	if(  is_alive()  ){
		int dx,dy,trig = (*stick)(dx,dy);
		int x,y;

		if( dx != 0  ||  dy !=0 ){
			last_dx = dx;
			last_dy = dy;
		}

		/***** �z�[�L�̏��� ******/
	
		switch( mode ){
	
		case NOFLY:		/* �z�[�L�͔��ł��Ȃ� */
			if( trig & 2 ){
				if( is_hyper() )
					status = ALIVE;
				x = bang.X()+8 ; hoki_dx = -last_dx<<1;
				y = bang.Y()+8 ; hoki_dy = -last_dy<<1;
				mode = GOING;
				sound_switch++;
				hoki.putat( x,y,hoki_pattern[ (angle=(angle+1)&7)>>1 ] );
			}
			break;
	
		case GOING:		/* �z�[�L���˒� */
			x = hoki.X();
			y = hoki.Y();
			
			/* hard_flag�������Ă���ƁA�ǂɓ�����Ȃ�����A�A���Ă��Ȃ�.
			 *          �����Ă��Ȃ��ƁA�g���K�𗣂������_�ŋA���Ă���.
			 */
			
			if(	   0 <= x  &&  x < 320-16  &&  0 <= y  &&  y < 200-16  &&  World::screen(x+8,y+8) < ATARI  
				&& ( (trig & 2) || hard_flag ) )
			{
				x += hoki_dx;
				y += hoki_dy;
			}else{
				mode = WAITING;
				waitcount = 10;
			}
			hoki.putat( x , y ,hoki_pattern[ (angle=(angle+1)&7)>>1 ] );
			break;
	
		case WAITING:	/* �z�[�L�͉�ʂ̒[,���邢�͏�Q���̂Ƃ����,�����|�����Ă��� */
			x = hoki.X();
			y = hoki.Y();
			if( --waitcount <= 0 )
				mode = BACK;
			hoki.putat( x,y,hoki_pattern[ (angle=(angle+1)&7)>>1 ] );
			break;
	
		case BACK:	/* �z�[�L�A�Ғ� */
			  signed int sx,sy;
			unsigned int xdis,ydis;
		
				 if( hoki.X() < bang.X() ){	sx = +2;	xdis = bang.X() - hoki.X();	}
			else if( hoki.X() > bang.X() ){	sx = -2;	xdis = hoki.X() - bang.X();	}
			else						  { sx =  0;	xdis = 0; }

				 if( hoki.Y() < bang.Y() ){	sy = +2;	ydis = bang.Y() - hoki.Y();	}
			else if( hoki.Y() > bang.Y() ){	sy = -2;	ydis = hoki.Y() - bang.Y();	}
			else						  { sy =  0;	ydis = 0; }

			if( xdis < 4  &&  ydis < 4 ){
				mode = NOFLY;
				sound_switch--;
				hoki.putoff();
			}else{
				x = hoki.X() + sx;
				y = hoki.Y() + sy;
				hoki.putat( x,y,hoki_pattern[ (angle=(angle+1)&7)>>1 ] );
			}
			break;
		}/* end of switch */

		for(EnemyIter iter ; iter != NULL ; iter++ ){
			/**** �z�[�L�̓����蔻�� ****/
			if( mode != NOFLY ){
				if(		hoki.X() < iter->X()+iter->Width()		&&	iter->X() < hoki.X()+16
					&&  hoki.Y() < iter->Y()+iter->Height()		&&	iter->Y() < hoki.Y()+16 )
				{
					iter->hit();
				}
			}
			/***** �{�̂̓����蔻�� *****/
			if(		bang.X() < iter->X()+iter->Width()-6	&&	iter->X() < bang.X()+18
				&&  bang.Y() < iter->Y()+iter->Height()-6	&&	iter->Y() < bang.Y()+18 )
			{
				if( is_hyper() )
					iter->hit();
				else
					status = EXPLODING;
			}
		}

		/****** �{�̂̏��� ******/

		Pattern pat;
		if( is_hyper() ){
			if( (status & 7) < 4 )
				pat = hibang0;
			else
				pat = hibang1;
			++status;
		}else{
			pat = bang_pattern[ mode==NOFLY ? 0 : 1 ] ;
		}
		
		x = bang.X()+dx;	y = bang.Y()+dy;
		
		if( x < 0 )			x = 0;
		if( x > 319-32 )	x = 320-32;
		if( y < 0 )		y = 0;
		if( y > 199-32 )	y = 199-32;
		
		if( !World::is_crushed( x+12,y+12,x+19,y+19,ATARI )  ||  is_hyper() ){
			bang.putat( x , y , pat );
		}else{
			status = EXPLODING;
		}
	}else if( status != DEAD ) {
		sound_switch=0;
		bang.putat(bang.X(),bang.Y(), pika_pattern[ --status >> 3 ] );
		if( status==DEAD ){
			if( --left != 0 ){
				status=HYPER;
				if( mode != NOFLY ){
					mode = NOFLY;
					hoki.putoff();
				}
			}else{
				minbang[ left ].putoff();
			}
		}
	}
#if 1
	/* �c�@�\�� */
	const static unsigned int   ledpat[] = {
		0 , /* dummy �c�@�� 0 �̎��́A�K�v�Ȃ�(����PowerLed���������Ă���) */
		0 , /* PowerLed only */
		ScrollLockLed ,
		ScrollLockLed | CapsLockLed ,
		ScrollLockLed | CapsLockLed | NumLockLed ,
	};
	LED( ledpat[ left ] );
#endif
	for(int i=0; i < left-1 ; i++ ){
		minbang[i].putat(i<<3 , 0 , ::minbang );
	}
}

static int countor = 0;				// ���ʂW�r�b�g:: 0-7Fh : ���Ă��Ȃ� 80h-FFh : ���Ă���
static int prev_sound_switch = 0;	// 1 : �O�ɌĂ΂ꂽ���ɃX�C�b�`�������Ă���		0 : ���Ȃ�

void BangHoki::scrolls(int dx,int dy)
{
	bang.scrolls(dx,dy);
	hoki.scrolls(dx,dy);
	for(int i=0; i < left-1; i++){
		minbang[i].scrolls(dx,dy);
	}
}

void BangHoki::hoki_sound( void )
{
	if( sound_switch  ){
		countor++;
		prev_sound_switch = 1;
		
		if( (countor & 0xFF) == 0x80 )
			beep(1);
		else if( ( countor & 0xFF ) == 0 )
			beep(0);
		
	}else if( prev_sound_switch ){
		prev_sound_switch = 0;
		if( (countor & 0xFF) > 0x80 )
			beep(0);
	}
}
void BangHoki::begin( void )
{
	mode = NOFLY;
	status = ALIVE;
	for(int i=0; i < left-1 ; i++ ){
		minbang[i].putat(i<<3 , 0 , ::minbang );
	}
}
void BangHoki::begin(int x,int y )
{
	mode = NOFLY;
	status = ALIVE;
	
	bang.putat( x , y , bang_pattern[0] );
	for(int i=0; i < left-1 ; i++ ){
		minbang[i].putat(i<<3 , 0 , ::minbang );
	}
}
void BangHoki::end( void )
{
	bang.putoff();
	hoki.putoff();
	sound_switch=0;
	for(int i=0; i < left-1 ; i++){
		minbang[i].putoff();
	}
}

BangHoki banghoki( keystick );
