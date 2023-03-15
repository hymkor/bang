#ifndef ENEMIES_H
#define ENEMIES_H

#include "hayos.h"
#include "enemy.h"


/** "弾" (シナリオファイルから直接呼び出すことはない) **/

class Bullet : public Enemy {
	int vx,vy;
public:
	Bullet(int x0,int y0,int Vx,int Vy) : Enemy(x0,y0,8,8) , vx(Vx) , vy(Vy) {  }
	void runs( void );
};


/** "砲台" (シナリオファイル : X , Y , 0 , 砲台向き[0-3] , 弾発射頻度[0-] , 弾発射向き[0-3]  , 爆弾率[0:でない] ) **/

class Houdai : public Enemy {
	int interval , direct , timer , bullet_direct , bomb_ratio , bomb_timer;
public:
	Houdai(int x0,int y0,int *info)
	 : Enemy(x0,y0,16,16) , direct(info[0]) , interval(info[1]) , timer(0) , bullet_direct(info[2]) ,
	   bomb_ratio(info[3]) , bomb_timer(0) { }
	void hit( void );
	void runs( void );
};


/** "触手"(シナリオファイル : X , Y , 1 ) ****/


class Tenta : public Enemy {
	int direct	, dir_timer  ,
		form	, form_timer ;
public:
	Tenta(int x0,int y0,int*)
	 : Enemy(x0,y0,16,16) , direct( 0 ) , dir_timer( 0 ) , form( 0 ) , form_timer( 0 ) { }

	void hit( void );
	void runs( void );
};


/** "爆弾"(シナリオファイル : X , Y , 2 , Vx , Vy , 爆発時間 ) **/

class Bomb : public Enemy {
	int vx,vy,timer;
public:
	Bomb(int x0,int y0,int *info)	: Enemy(x0,y0,8,8) , vx( info[0] ) , vy( info[1] ) , timer( info[2] ) {  }
	Bomb(int x0,int y0,int Vx,int Vy,int Timer) : Enemy(x0,y0,8,8) , vx(Vx) , vy(Vy) , timer( Timer ) {  }
	
	void hit( void );
	void runs( void );
};


/*** "戦闘機"(シナリオファイル : X , Y , 3 ) ***/

class Fighter : public Enemy {
	int vy;
public:
	Fighter(int x0,int y0,int *info);

	void hit( void );
	void runs( void );
};


/**** サイコバング(シナリオファイル : X , Y , 4 ) ****/

class Psycho : public Enemy {
	int direct;
	int hyper;
	int dir_timer;
	int gun_timer;
	int hitpoint;
	enum{ ORIGINAL_EXPLODING = -40 };	/* -5 to -2 まで,独自の巨大爆発,その後普通爆発に移行する */
public:
	Psycho(int x0,int y0,int *) : Enemy(x0-16,y0-16,32,32) , dir_timer(0) , gun_timer(0) , hyper(0) , hitpoint(4) {  }
	
	void hit(void);
	void runs(void);
};

#endif
