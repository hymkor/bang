#include <stddef.h>
#include "hayos.h"
#include "sprite.h"
#include "enemy.h"
#include "enemies.h"

/* static menber variables */

Enemy *Enemy::first = NULL;
HeapMan Enemy::heapman( sizeof(Enemy) + yoyusize , 32 );
int Enemy::sound_switch = 0;

void Enemy::sound_begin(void)
{
	if( sound_switch )
		beep(1);
}
void Enemy::sound_end(void)
{
	if( sound_switch ){
		beep(0);
		sound_switch = 0;
	}
}
Enemy::Enemy(int x, int y, int w, int h ) : Sprite(x,y) , width(w) , height(h)
{
	prev = NULL;
	if( (next = first) != NULL ){
		next -> prev = this;
	}
	first = this;
	status = ALIVE;
	num_enemy++;
}

Enemy::~Enemy( void )
{
	if( prev != NULL ){
		prev->next = next;
	}else{
		first = next;
	}
	
	if( next != NULL ){
		next->prev = prev;
	}
	num_enemy--;
}

void Enemy::run( void )
{
	for(EnemyIter iter ; iter != NULL ; iter++ ){
		/******* Á–Å ********/
		if( iter->is_dead()   ||  iter->X() < 0   ||  iter->X() > 319  ||  iter->Y() < 0  ||  iter->Y() > 199  ){
			delete (Enemy*)iter;
			
		/******* ”š”­’¼Œã *******/
		}else if( iter->status == EXPLODING ){	
			iter->putat(  iter->X()+((iter->Width()-16)>>1)  ,  iter->Y()+((iter->Height()-16)>>1)
			 				, pika_pattern[ --iter->status >> 2 ]  );
			
		/******* ”š”­’† *********/
		}else if( iter->is_exploding() ){
			iter->putat( iter->X() , iter->Y() , pika_pattern[ --iter->status >> 2 ]  );
			
		/******* ¶‘¶ / “ÆŽ©‚Ì”š”­ˆ— *********/
		}else{
			iter->runs();
		}
	}
}

void Enemy::scroll(int dx,int dy)
{
	for(EnemyIter iter ; iter != NULL ; iter++ )
		iter->scrolls(dx,dy);
}
void Enemy::kill_all( void )
{
	for(EnemyIter iter ; iter != NULL ; iter++ )
		delete (Enemy*)iter;
	sound_switch=0;
}
enum{
	HOUDAI,
	TENTA,
	BOMB,
	FIGHTER,
	PSYCHO,
};

void Enemy::yield(int x,int y,int type,int *info)
{
	switch( type ){
		case HOUDAI:	(void) new Houdai(x,y,info);	break;
		case TENTA:		(void) new Tenta (x,y,info);	break;
		case BOMB:		(void) new Bomb	(x,y,info);		break;
		case FIGHTER:	(void) new Fighter(x,y,info);	break;
		case PSYCHO:	(void) new Psycho(x,y,info);	break;
	};
}
