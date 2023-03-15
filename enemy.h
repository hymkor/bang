#ifndef ENEMY_H
#define ENEMY_H

#include "heapman.h"
#include "sprite.h"
#include "hayos.h"

class EnemyIter;

class Enemy : public Sprite {
	friend class EnemyIter;

	int width,height;

		   Enemy	*prev,	*next;			/* prev==NULL �Ȃ��  *this �� first�̎����I�u�W�F�N�g */
	static Enemy	*first;
	static HeapMan	 heapman;
	static int sound_switch;
	static int num_enemy;
	
	enum{ yoyusize	= 32 };					/* �q�[�v���蓖�Ď��̗]�T�̃T�C�Y */
	static void sound_begin(void);
	static void sound_end(void);
protected:
	int	status;	/* �ȉ��̂悤�Ȓl���Ƃ�. */
	enum{
	/*	 :						�Ǝ��̔��� */
		ALIVE		=-1,	/*	���� */
		DEAD		= 0,	/*	���� : �������Ƀf�X�g���N�g����� */
	/*	 :						������ */
		EXPLODING	= 12,	/*	�����J�n */
	};
	void Pi( void ){ sound_switch = 1; }
public:
	static void sound_on( void ){ atsleep(sound_begin,-1); atsleep(sound_end,+1); }
	
	 Enemy( int x , int y , int width=16 , int height=16);
	~Enemy( void );
	
	virtual void runs( void ) = 0;
	virtual void hit( void ){ status = DEAD; }
	
	void *operator new(size_t)	{ return heapman.alloc();	}
	void  operator delete(void *obj){ 	 heapman.free(obj);	}

	int Width ( void ) const { return width; }
	int Height( void ) const { return height; }

	int is_alive( void ) const { return status==ALIVE; }
	int is_dead( void ) const { return status==DEAD; }
	int is_exploding( void ) const { return status > DEAD; }

	static void run( void );
	static void scroll(int dx,int dy);
	static void yield(int x,int y,int type,int *informations);
	static void kill_all( void );
	static int number( void ){ return num_enemy; }
};


class EnemyIter{
	Enemy *current,*next;
public:
	 EnemyIter( void ) : current( Enemy::first ) , next( Enemy::first ? Enemy::first->next : 0 )
	  {  }
	~EnemyIter( void ) {  }
	
	operator Enemy*( void ) { return current; }
	int operator!( void ) { return current==NULL;}
	Enemy &operator *( void ) { return *current; }
	Enemy *operator->( void ) { return  current; }
	void   operator++( void ) { current=next;	if(next) next=next->next; }
};

inline int sign(int r){ return r<0 ? -1 : r>0 ? 1 : 0 ; }

#endif