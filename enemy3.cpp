#include "game.h"
#include "enemies.h"
#include "pattern.h"

void Psycho::hit( void )
{
	if( !hyper ){
		hyper = 110;
		if( --hitpoint == 0 ){
			status = ORIGINAL_EXPLODING;
		}
	}
}

void Psycho::runs( void )
{
	static int dirtbl[4][2]={
		{ -1,  0 },
		{  0, -1 },
		{ +1,  0 },
		{  0, +1 },
	};
	
	
	if( is_alive() ){
		int x,y;
		for(int i=0;;i++){
			x = X() + dirtbl[ direct ][ 0 ];
			y = Y() + dirtbl[ direct ][ 1 ];
			if( World::is_movable( x + 4 , y + 4 , x + 28 , y + 28 , ATARI )  )
				break;
			if( i==4 ){
				x = X(); y = Y();
				break;
			}
			direct = (direct + 1) % 4;
		}
		if( hyper ){
			putat( x,y, psycho1 );
			--hyper;
		}else if( is_alive() ){
			putat( x,y, psycho0 );
		}

		if( ++dir_timer >= 150 ){
			direct = (direct+1) % 4;
			dir_timer = 0;
		}
		if( ++gun_timer == 80 ){
			(void) new Bomb( x , y+12 , -1 , -1 , 80 );
			(void) new Bomb( x , y+12 , -1 , +1 , 80 );
			gun_timer = 0;
		}
	}else{
	
		/*** “Æ©”š”­ƒ‹[ƒ`ƒ“ ***/
		putat( X() , Y() , bigbang );
		if( ++status == -1 )
			status = EXPLODING;
	}
	
}