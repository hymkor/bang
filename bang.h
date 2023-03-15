#ifndef BANG_H
#define BANG_H

#include "hayos.h"
#include "sprite.h"
#include "joy.h"

extern int burner_switch;
extern int hard_flag;

class BangHoki {
	enum{ ERROR , NOFLY , GOING , WAITING , BACK } mode;

	Sprite	bang,hoki,minbang[10];  /* ������Ƒ��߂Ɋm�� */
	int angle;	/* �z�[�L�V���b�g�̉�]�p�x�@0-3 */
	int hoki_dx,hoki_dy; /* �z�[�L���ˎ��̈ړ��� */
	int last_dx,last_dy; /* �Ō�ɖ{�̂��ړ���������(�z�[�L�����ł��������̋t) */
	int waitcount;	/* �z�[�L�̑΋󎞊ԗp�J�E���^�[ (mode==WAITING�̎��̂�) */
	int left;	/* �c��c�@�� */
	stickfunc_t stick;
	static int sound_switch;
	static void hoki_sound( void );

	int	status;	/* �ȉ��̂悤�Ȓl���Ƃ�. */
	enum{
		HYPER		=-100,	/*  ���G */
		ALIVE		=-1,	/*	���� */
		DEAD		= 0,	/*	���� : �������Ƀf�X�g���N�g����� */
	/*	 :						������ */
		EXPLODING	= 24,	/*	�����J�n */
	};
public:

	/*** constructors ****/

	 BangHoki(stickfunc_t func)
		: angle(0) , mode(NOFLY) , status(ALIVE) , last_dx(1) , last_dy(0) , stick( func ) , left( 3 ) {  }
	 BangHoki(int x,int y,stickfunc_t func)
		: angle(0) , mode(NOFLY) , status(ALIVE) , last_dx(1) , last_dy(0) , stick( func ) , bang(x,y) , left( 3 ) {  }
	~BangHoki() {  }
	
	/*** methods ***/

	void begin(int x,int y);
	void begin( void );
	void end  ( void );
	
	void runs( void );
	void scrolls(int dx,int dy);
	
	/* ���|�[�g�֐� */
	int X() const { return bang.X(); }
	int Y() const { return bang.Y(); }
	int HokiX() const { return hoki.X(); }
	int HokiY() const { return hoki.Y(); }
	int Left() const { return left; }
	void set_left(int n){ left=n; }
	
	int is_alive(void) const { return status <= ALIVE; }
	int is_hyper(void) const { return status <  ALIVE; }
	int	is_over(void)  const { return status == DEAD ; }
	
	void operator = ( stickfunc_t func ){ stick=func; }

	static void sound_on( void ){ atsleep(hoki_sound);  beep_off(); }
	static void sound_stop( void ){ sound_switch=0; }
};

extern BangHoki banghoki;	/* ����̓��C���֐�����̂ݎQ�Ƃ���.*/

#endif
