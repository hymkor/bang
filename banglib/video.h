#ifndef VIDEO_H
#define	VIDEO_H


/* �r�f�I���[�h�ύX/�Q��
 *	13h ... 320x200x256
 *	 3h ... Text Mode   (80x25�Œ�)
 *  70h ... V-Text Mode (���E�s����)
 * DOS�ɖ߂�Ƃ��́A�Ăяo�����ȑO�̃r�f�I���[�h�ɖ߂��̂��]�܂����B
 */
void set_video_mode(unsigned mode);
unsigned get_video_mode( void );




/* �X�N���[���֌W�̊֐��Q
 *	13h�̎��́A�P�Ɂuadd_vram_offset_13h�v���g�p����΂悢�B
 *	�X�N���[�������Ƃ��A(0,0)�̃h�b�g�́A
 *		segment ... A000h
 *		offset  ... vram_offset (unsigned short�^)
 *	�ɂȂ�BAFFFFh��A0000h���Ȃ����Ă���̂͌����܂ł��Ȃ��B
 *
 *     (�ȉ��A�֑�)
 * set_start_address ......... (0,0)�̈ʒu���A�o�C�g�P�ʂł��炷�B
 * set_horizontal_planning ...      �V        �r�b�g�P�ʂł��炷�B
 *	��҂́A�ꌩ 13h �ɂ͕s�v�Ɍ����邪�A13h �Ƃ�����ʃ��[�h�͖{���A
 *	320x200x4�F�Ƃ����v���[����4���g�ݍ��킹���\���ɂȂ��Ă���̂ŁA
 *	�P�h�b�g���炷�̂Ɏ��͂��̊֐��� �Q�r�b�g�����炵�Ă��K�v������B
 *	CPU��VRAM���A�N�Z�X����Ƃ��́AVGA-CHIP���Ԃɓ����ĂP�h�b�g���P�o�C�g��
 *	�P�v���[���ł��邩�̂悤�Ɍ����Ă���̂ł���B
 *	add_vram_offset_13h���g�����ɂ́A���̕ӂ̂�₱�������͍l���Ȃ��Ă��悢�B
 */


extern unsigned short vram_offset=0;

void set_start_address(unsigned short address)
void set_horizontal_planning( unsigned short npixel )

void set_crtc_register(unsigned short address,unsigned short data)
int get_crtc_register(unsigned short address)

void add_vram_offset_13h( signed int delta )
/* delta : ���X�N���[������h�b�g���B�c�̎��́A(320�~���C����)�ł悢�͂� */



#define	vsync()			(inportb(0x3DA) & 8)
	/* �A�����Ԓ��Ȃ� not 0 �ɂȂ� */
#define vsync_wait()	do{ while( vsync() ); while( !vsync() ); }while(0)
	/* vsync_wait()���s��́A�A�����Ԃ̂͂� */


#endif
