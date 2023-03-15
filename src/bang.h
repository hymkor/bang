#ifndef BANG_H
#define BANG_H

#include "hayos.h"
#include "sprite.h"
#include "joy.h"

extern int burner_switch;
extern int hard_flag;

class BangHoki {
	enum{ ERROR , NOFLY , GOING , WAITING , BACK } mode;

	Sprite	bang,hoki,minbang[10];  /* ちょっと多めに確保 */
	int angle;	/* ホーキショットの回転角度　0-3 */
	int hoki_dx,hoki_dy; /* ホーキ発射時の移動量 */
	int last_dx,last_dy; /* 最後に本体が移動した方向(ホーキが飛んでいく方向の逆) */
	int waitcount;	/* ホーキの対空時間用カウンター (mode==WAITINGの時のみ) */
	int left;	/* 残り残機数 */
	stickfunc_t stick;
	static int sound_switch;
	static void hoki_sound( void );

	int	status;	/* 以下のような値をとる. */
	enum{
		HYPER		=-100,	/*  無敵 */
		ALIVE		=-1,	/*	無傷 */
		DEAD		= 0,	/*	消去 : ただちにデストラクトされる */
	/*	 :						爆発中 */
		EXPLODING	= 24,	/*	爆発開始 */
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
	
	/* リポート関数 */
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

extern BangHoki banghoki;	/* これはメイン関数からのみ参照する.*/

#endif
