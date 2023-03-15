#include "enemy.h"
#include "enemies.h"
#include "game.h"
#include "pattern.h"

void Tenta::hit( void )
{
	status = 12;
}

void Tenta::runs( void )
{
	static const char dirtbl[ 4 ][ 2 ] = {
		{ -1,-1 },{ -1,+1 },{ +1,+1 },{ +1,-1 }
	};
	
	if( ++form_timer >= 10 ){
		form_timer = 0 ;
		form ^= 1;	/* form ¸ { 0 , 1 } */
	}
	
	if( ++dir_timer >= 23 ){
		dir_timer = 0;
		direct = (direct+1)%4;
	}
	
	int x,y;
	
	for(int i=0;;i++){
		x = X() + dirtbl[ direct ][ 0 ];
		y = Y() + dirtbl[ direct ][ 1 ];
		if( World::is_movable( x + 4 , y + 4 , x + 12 , y + 12 , ATARI )  )
			break;
		if( i==4 ){
			x = X(); y = Y();
			break;
		}
		direct = (direct + 1) % 4;
	}
	putat( x,y,tenta_pattern[form] );
}

void Bomb::hit( void )
{
	if( !is_exploding() )
		status = EXPLODING ;
}

void Bomb::runs( void )
{
	int x=X()+vx,y=Y()+vy;
	
	if( --timer == 0  ||  World::screen(x+4,y+4) >= ATARI  ){
		status = DEAD;
		( void ) new Bullet( x-4 , y-4 , -2 , -2 );		// 7
		( void ) new Bullet( x   , y-4 ,  0 , -2 );		// 8
		( void ) new Bullet( x+4 , y-4 , +2 , -2 );		// 9
		( void ) new Bullet( x+4 , y   , +2 ,  0 );		// 6
		( void ) new Bullet( x+4 , y+4 , +2 , +2 );		// 3
		( void ) new Bullet( x   , y+4 ,  0 , +2 );		// 2
		( void ) new Bullet( x-4 , y+4 , -2 , +2 );		// 2
		( void ) new Bullet( x-4 , y   , -2 ,  0 );		// 4
		Pi();
	}else{
		putat(x,y,bomb);
	}
}
