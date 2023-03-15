#ifndef JOY_H
#define JOY_H

typedef unsigned (*stickfunc_t)(int *dx,int *dy);

/* �ȉ��̊֐��ł́A�W���C�X�e�B�b�N�̍��W�l���|�C���^�ŕԂ��A
 * �g���K�̒l��߂�l�ŕԂ��B
 *
 * keystick , sega_joystick? �ł� *dx,*dy�� -1,0,+1 ���Ԃ��Ă���B
 *
 * joystick? �̓A�i���O�W���C�X�e�B�b�N�Ȃ̂ŁA0�`360���Ԃ��Ă���B
 * �X�e�B�b�N�������̎��A���W�l�Ƃ��� (180,180) �t�� �̒l�� (dx,dy) �ɑ�������B
 * �܂��A(0,0)�̎��́A�W���C�X�e�B�b�N���h�����Ă��Ȃ��B
 * ���\�A�l�̕ϓ����������̂ŁAprintf�ŕ\�������ČX���������̖ڂŊm�F���������悢�B
 */

/* �L�|�{�|�h�ɂ��G�~�����[�g "keystick.c","scnkey.asm" */
unsigned keystick (int *dx,int *dy);

/* �Z�K�e���h���C�u�p�W���C�X�e�B�b�N "segajoy.c" */
void sega_joy_init( void );	/* �������֐� */
unsigned sega_joystick0(int *dx,int *dy);
unsigned sega_joystick1(int *dx,int *dy);

/* �h�a�l�|�o�b�^�`�s�@�Q�|���|�|�g�p�W���C�X�e�B�b�N  "joy.c"
 * �����A���܂������͂��A�e�X�g���Ă��Ȃ���.....
 */
unsigned joystick0(int *dx,int *dy);
unsigned joystick1(int *dx,int *dy);


/* ���A���^�C���L�[�X�L�����̂��߂̃��[�`�� "scnkey.asm"
 * ��x�Akeyscan_init���Ă�ł����΁A�Ȍ� ���荞�݃��[�`����
 * scantable�̓��e�������I�ɕω�������B
 *  0...������Ă��Ȃ��B not 0...������Ă���B
 */

extern volatile char scantable[128];
void keyscan_init( void );


#endif
