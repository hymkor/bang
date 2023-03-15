#ifndef JOY_H
#define JOY_H

static int dummy_for_stick;

typedef unsigned (*stickfunc_t)(int &dx=dummy_for_stick,int &dy=dummy_for_stick);

/* キ－ボ－ドエミュレート */
unsigned keystick (int &dx=dummy_for_stick,int &dy=dummy_for_stick);
void keyscan(void);

/* ＩＢＭ－ＰＣ／ＡＴ　ゲ－ムポ－ト用ジョイスティック */
unsigned joystick0(int &dx=dummy_for_stick,int &dy=dummy_for_stick);
unsigned joystick1(int &dx=dummy_for_stick,int &dy=dummy_for_stick);

/* セガテラドライブ用ジョイスティック */
unsigned sega_joystick0(int &dx=dummy_for_stick,int &dy=dummy_for_stick);
unsigned sega_joystick1(int &dx=dummy_for_stick,int &dy=dummy_for_stick);
void sega_joy_init( void );

#endif
