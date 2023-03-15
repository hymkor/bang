#ifndef SPRITE_H
#define SPRITE_H

/* Pattern ... �p�^�[����ۑ����Ă���ꏊ�������|�C���^�̌^
 *	�uchar far* �v�Ɖ���������̂��ʓ|������Atypedef���Ă��邾��
 */
typedef char far *Pattern;				

/* NOPATTERN ... �p�^�[����\�����Ă��Ȃ��Ƃ����ׂ̃t���O����̒萔�B
 *	���܂�A�債���g�����͂Ȃ��B
 */
#define	NOPATTERN		(Pattern)0


/* sprite_putat ... SPR �`���̃L�����N�^�[��\������B
 * ���̎��A�A��l��ۑ����Ă�������!
 * �L�����N�^�[�������Ƃ��A���̒l���Asprite_putoff�̑������ɓn���Ă��΁A
 * �X�N���[���Ȃǂ����č��W�ƃA�h���X�̑Ή����ς���Ă��܂��Ă��Ă��A�m����
 * �\�������L�����N�^�[���������Ƃ��ł���B
 * �܂�A�A��l�̓��e�́A�\���A�h���X�̃I�t�Z�b�g�������肷��킯���B
 */
short	sprite_putat( unsigned short X , unsigned short Y , Pattern pattern );

/* sprite_putoff ... �\�����Ă���L�����N�^�[�������Bʰ�޳����۰ق��Ă��Ă�OK!	*/
void	sprite_putoff( short vramptr , Pattern pattern );

/* sprite_load ... *.SPR �`���̃L�����N�^�[�����[�h����B
 *	���̊֐�����сA�ٍ�̃p�^�[���G�f�B�^�uPE�v�́A1-�L�����N�^�[:1-�t�@�C��
 *	�ƂȂ��Ă��āA�������������B���Ȃ݂ɁA�uPE�v�ɂ͉t�Z�[�u�ƕs�t�Z�[�u
 *	������Asprite_load�œǂ߂�̂͌�҂̕��ł���B�O�҂͕�����x�ɃZ�[�u���A
 *	�uPE�v�ł܂����[�h�ł��邪�A��҂͈�x�Ɉ�A�������uPE�v�ł̓��[�h�ł���
 *	���B
 */
Pattern sprite_load(const char *fname )


/* background ... �w�i�̕ۑ��̈�ւ�far�|�C���^�B
 *	��{�I�ɁA�ړ��L�����N�^�[�����݂��Ȃ��AVRAM�̃R�s�[�ƍl���Ă悢�B
 *	�܂�A(0,0)�̃A�h���X�́A&background[ vram_offset ]�ƂȂ�킯�ł���B
 *	VRAM�Ɠ��������O�\���Ȃ̂ŁA(x,y)�̂́A
 *		background[ (vram_offset + x + y*320) & 0xFFFF ]
 *	�ƂȂ�B�����Ax,y �� unsigned short �ɂƂ��Ă��΁A0xFFFF ������Ȃ��Ȃ�
 *	�͂��ł���B
 *	
 *	�Ȃ��Afar�|�C���^�Ƃ��Ă� background �́A�I�t�Z�b�g�����傤�� 0000 �ɂȂ�
 *	�悤�ɒ������Ă���̂ŁAVRAM �������|�C���^�� BG-RAM �������|�C���^�́A
 *	�����I�t�Z�b�g���g�p���邱�Ƃ��ł���B(�C�����C���A�Z���u���̎��łȂ��ƁA
 *	���܂�A�����b�g�͂Ȃ���)
 */
extern char far *background;


/* background_init ... background ��p��(farmalloc)���āA����������B
 *	sprite_off�����s����O�ɕK���Ă�ł�������!
 */
int background_init(void);


extern unsigned short vram_offset;	/* video.h �Q�� */

#endif SPRITE_H
