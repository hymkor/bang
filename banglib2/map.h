#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

/*** ������cell(�Z��)�Ƃ́A8dots�~8lines �̔w�i�u���b�N�������B
 *** �f�[�^�\���́A�uPE�v�́uSAVE-ALL�v�ɂ���ăZ�[�u���ꂽ�f�[�^�`����
 *** ���̂܂ܗ��p���A256�̃Z�����ꊇ���Ĉ������ƂɂȂ�B
 *** ( X1��P.C.G.�݂����Ȃ��̂̂��� )
 ***
 *** �ȉ��̃��[�`���ł́A256���z���鐔�̃Z������x�ɗ��p���邱�Ƃ��ł��Ȃ��B
 *** �������B
 ***/


/* �Z�����i�[�����o�b�t�@�ւ̃|�C���^�B�o�b�t�@�͌Œ肳��Ă��āA
 * ���������Ƃ��ł��Ȃ��B���������񂾁B
 */
extern char far *cell_graphics;


/* SAVE-ALL�ɂ���ăZ�[�u���ꂽ�Z�������[�h����B*/
int  cell_load(const char *fname);


/* �w�肳�ꂽ���W�� type �Ԗڂ̃Z����\������B
 * �Z���̔ԍ��́APE�̉�ʂł����ƁA
 *		�O �P �Q ..... 15
 *		16 17 18 ..... 31
 *		 :             :
 *		240 ......... 255
 *	�Ăȋ�ɕ���ł����͂��B�m���B
 *
 */
void cell_putat(unsigned x,unsigned y,unsigned type);

#endif
