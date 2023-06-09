#include "bang.h"
#include "game.h"
#include "enemy.h"
#include "enemies.h"
#include "pattern.h"

Fighter::Fighter(int x0,int y0,int *) : Enemy(x0,y0,16,16)
{
	vy=sign( banghoki.Y() - Y() );
	if( vy==0 ) vy=-1;
}

void Fighter::hit( void )
{
	status = EXPLODING;
}

void Fighter::runs( void )
{
	/* BangHokiとすれちがいざまに発射 */
	int delta = banghoki.Y() - Y();
	if( delta==0  &&  banghoki.X() < X() ){
		(void) new Bullet( X()-4 , Y() , -4 , 0 );
	}
		
	/* 離れ過ぎたらBanghokiの方へ反転 */
	if( vy < 0 ){
		if( Y() < banghoki.Y()-50 )
			vy = +1;
	}else{
		if( Y() > banghoki.Y()+50 )
			vy = -1;
	}
	
	int x=X()-1;
	int y=Y()+vy;
	
	/* 天井/地面にぶつかったとき,反転 */
	if( World::is_crushed(x+4,y+4,x+11,y+11,ATARI) ){
		y = Y()+( vy = -vy );
		
		/* 正面衝突の時,爆発 */
		if( World::is_crushed(x+4,y+4,x+11,y+11,ATARI) ){
			status = EXPLODING;
			return;
		}
	}
	putat(x,y,fighter);
}