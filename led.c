#pragma inline

#define	KEYCMD	0x60 /* �L�[�{�[�h�R���g���[���̓��o�̓o�b�t�@�|�[�g */
#define KEYSTT	0x64 /* �L�[�{�[�h�R���g���[���̃X�e�[�^�X�|�[�g	 */
#define	ACK		0xFA /* �L�[�{�[�h�̉����R�[�h						 */
#define	LEDSET	0xED /* �L�[�{�[�h��LED�Z�b�g���Z�b�g�R�}���h		 */

void LED(unsigned int patn)
{
	static unsigned int lastpatn = -1;
	if( lastpatn != patn ){
		lastpatn = patn ;

		asm cli
	/* LED�R�}���h�𑗂� */
		asm	mov	al,LEDSET
		asm out KEYCMD,al
	lop1:
		asm in	al,KEYCMD
		asm cmp al,ACK
		asm jne	lop1
	
	/* LED�f�[�^�𑗂� */
		asm mov al,byte ptr patn
		asm and	al,7
		asm out	KEYCMD,al
	led2:
		asm in	al,KEYCMD
		asm cmp	al,ACK
		asm jne	led2
		asm sti
	
	/* �������Ƃ͎v�����A�o�b�t�@���t���Ȃ�҂� */
	led3:
		asm	in	al,KEYSTT
		asm and	al,2
		asm jne	led3
	}
}
