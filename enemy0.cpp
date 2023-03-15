#include "game.h"
#include "enemies.h"
#include "pattern.h"

void Bullet::runs( void )
{
	int x = X() + vx;
	int y = Y() + vy;
	
	if( World::screen(x+4,y+4) >= ATARI ){
		status = DEAD;
	}else{
		putat( x , y , bullet );
	}
}

void Houdai::hit( void )
{
	if( ! is_exploding() )
		status = EXPLODING;
}

void Houdai::runs( void )
{
	const static int dir[4][2]={
		{ 0,-1} , {-1, 0} , { 0, 1} , { 1, 0}
	};
	const static Pattern pattern[]={
		houdai0,	houdai1,	houdai2,	houdai3,
	};
	
	if( ++timer > interval ){
		timer = 0;
		if( bomb_ratio !=0  &&  ++bomb_timer >= bomb_ratio ){
			bomb_timer=0;
			(void)new Bomb(		X()+4+dir[bullet_direct][0]*4  ,  Y()+4+dir[bullet_direct][2]*4  ,
								  dir[bullet_direct][0]  ,  dir[bullet_direct][1]   ,   interval  );
		}else{
			(void)new Bullet(	X()+4+dir[bullet_direct][0]*4  ,  Y()+4+dir[bullet_direct][2]*4  ,
								  dir[bullet_direct][0]  ,  dir[bullet_direct][1]  );
		}
		Pi();
	}
	putat( X(), Y(), pattern[ direct & 3 ] );
}
