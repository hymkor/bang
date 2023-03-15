#include <stdio.h>

#include "video.h"
#include "sprite.h"

int main( void )
{
	Pattern ptn;
	short pos;
	int oldmode;
	
	int i;
	
	ptn = sprite_load( "PSYCHO0.SPR" );
	if( ptn == NOPATTERN ){
		fputs( "�J�����g�f�B���N�g���[��\"PSYCH0.SPR\"��u���悤��!\n" , stderr );
		return 1;
	}
	
	if( background_init() ){
		fputs( "Heap error" , stderr );
		return 2;
	}
	
	oldmode = get_video_mode();
	set_video_mode( 0x13 );
	
	pos = sprite_putat( 0,80 , ptn );
	
	(void) getch();
	
	/* �Ƃ肠�����A�����̈ړ� */
	for(i=2;i<280;i+=2){
		vsync_wait();
		sprite_putoff(pos,ptn);
		pos=sprite_putat( i,80 , ptn );
	}
	
	(void) getch();
	
	/* �X�N���[���e�X�g */
	for(i=280;i>2;i-=2){
		vsync_wait();
		add_vram_offset_13h( +2 );
	}
	
	(void) getch();
	
	/* �X�N���[�����č��W���ς���Ă��Ă��A�����Ə�����B*/
	sprite_putoff( pos , ptn );
	
	(void) getch();
	
	set_video_mode( oldmode );
}
