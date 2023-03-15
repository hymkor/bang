#ifndef JOY_H
#define JOY_H

typedef unsigned (*stickfunc_t)(int *dx,int *dy);

/* 以下の関数では、ジョイスティックの座標値をポインタで返し、
 * トリガの値を戻り値で返す。
 *
 * keystick , sega_joystick? では *dx,*dyに -1,0,+1 が返ってくる。
 *
 * joystick? はアナログジョイスティックなので、0～360が返ってくる。
 * スティックが中央の時、座標値として (180,180) 付近 の値が (dx,dy) に代入される。
 * また、(0,0)の時は、ジョイスティックが刺さっていない。
 * 結構、値の変動が激しいので、printfで表示させて傾向を自分の目で確認した方がよい。
 */

/* キ－ボ－ドによるエミュレート "keystick.c","scnkey.asm" */
unsigned keystick (int *dx,int *dy);

/* セガテラドライブ用ジョイスティック "segajoy.c" */
void sega_joy_init( void );	/* 初期化関数 */
unsigned sega_joystick0(int *dx,int *dy);
unsigned sega_joystick1(int *dx,int *dy);

/* ＩＢＭ－ＰＣ／ＡＴ　ゲ－ムポ－ト用ジョイスティック  "joy.c"
 * 多分、うまく動くはず、テストしていないが.....
 */
unsigned joystick0(int *dx,int *dy);
unsigned joystick1(int *dx,int *dy);


/* リアルタイムキースキャンのためのルーチン "scnkey.asm"
 * 一度、keyscan_initを呼んでおけば、以後 割り込みルーチンが
 * scantableの内容を自動的に変化させる。
 *  0...押されていない。 not 0...押されている。
 */

extern volatile char scantable[128];
void keyscan_init( void );


#endif
