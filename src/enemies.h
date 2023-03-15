#ifndef ENEMIES_H
#define ENEMIES_H

#include "hayos.h"
#include "enemy.h"


/** "�e" (�V�i���I�t�@�C�����璼�ڌĂяo�����Ƃ͂Ȃ�) **/

class Bullet : public Enemy {
	int vx,vy;
public:
	Bullet(int x0,int y0,int Vx,int Vy) : Enemy(x0,y0,8,8) , vx(Vx) , vy(Vy) {  }
	void runs( void );
};


/** "�C��" (�V�i���I�t�@�C�� : X , Y , 0 , �C�����[0-3] , �e���˕p�x[0-] , �e���ˌ���[0-3]  , ���e��[0:�łȂ�] ) **/

class Houdai : public Enemy {
	int interval , direct , timer , bullet_direct , bomb_ratio , bomb_timer;
public:
	Houdai(int x0,int y0,int *info)
	 : Enemy(x0,y0,16,16) , direct(info[0]) , interval(info[1]) , timer(0) , bullet_direct(info[2]) ,
	   bomb_ratio(info[3]) , bomb_timer(0) { }
	void hit( void );
	void runs( void );
};


/** "�G��"(�V�i���I�t�@�C�� : X , Y , 1 ) ****/


class Tenta : public Enemy {
	int direct	, dir_timer  ,
		form	, form_timer ;
public:
	Tenta(int x0,int y0,int*)
	 : Enemy(x0,y0,16,16) , direct( 0 ) , dir_timer( 0 ) , form( 0 ) , form_timer( 0 ) { }

	void hit( void );
	void runs( void );
};


/** "���e"(�V�i���I�t�@�C�� : X , Y , 2 , Vx , Vy , �������� ) **/

class Bomb : public Enemy {
	int vx,vy,timer;
public:
	Bomb(int x0,int y0,int *info)	: Enemy(x0,y0,8,8) , vx( info[0] ) , vy( info[1] ) , timer( info[2] ) {  }
	Bomb(int x0,int y0,int Vx,int Vy,int Timer) : Enemy(x0,y0,8,8) , vx(Vx) , vy(Vy) , timer( Timer ) {  }
	
	void hit( void );
	void runs( void );
};


/*** "�퓬�@"(�V�i���I�t�@�C�� : X , Y , 3 ) ***/

class Fighter : public Enemy {
	int vy;
public:
	Fighter(int x0,int y0,int *info);

	void hit( void );
	void runs( void );
};


/**** �T�C�R�o���O(�V�i���I�t�@�C�� : X , Y , 4 ) ****/

class Psycho : public Enemy {
	int direct;
	int hyper;
	int dir_timer;
	int gun_timer;
	int hitpoint;
	enum{ ORIGINAL_EXPLODING = -40 };	/* -5 to -2 �܂�,�Ǝ��̋��唚��,���̌㕁�ʔ����Ɉڍs���� */
public:
	Psycho(int x0,int y0,int *) : Enemy(x0-16,y0-16,32,32) , dir_timer(0) , gun_timer(0) , hyper(0) , hitpoint(4) {  }
	
	void hit(void);
	void runs(void);
};

#endif
