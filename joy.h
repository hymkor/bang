#ifndef JOY_H
#define JOY_H

static int dummy_for_stick;

typedef unsigned (*stickfunc_t)(int &dx=dummy_for_stick,int &dy=dummy_for_stick);

/* �L�|�{�|�h�G�~�����[�g */
unsigned keystick (int &dx=dummy_for_stick,int &dy=dummy_for_stick);
void keyscan(void);

/* �h�a�l�|�o�b�^�`�s�@�Q�|���|�|�g�p�W���C�X�e�B�b�N */
unsigned joystick0(int &dx=dummy_for_stick,int &dy=dummy_for_stick);
unsigned joystick1(int &dx=dummy_for_stick,int &dy=dummy_for_stick);

/* �Z�K�e���h���C�u�p�W���C�X�e�B�b�N */
unsigned sega_joystick0(int &dx=dummy_for_stick,int &dy=dummy_for_stick);
unsigned sega_joystick1(int &dx=dummy_for_stick,int &dy=dummy_for_stick);
void sega_joy_init( void );

#endif
