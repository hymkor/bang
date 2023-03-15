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
		fputs( "カレントディレクトリーに\"PSYCH0.SPR\"を置くように!\n" , stderr );
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
	
	/* とりあえず、ただの移動 */
	for(i=2;i<280;i+=2){
		vsync_wait();
		sprite_putoff(pos,ptn);
		pos=sprite_putat( i,80 , ptn );
	}
	
	(void) getch();
	
	/* スクロールテスト */
	for(i=280;i>2;i-=2){
		vsync_wait();
		add_vram_offset_13h( +2 );
	}
	
	(void) getch();
	
	/* スクロールして座標が変わっていても、ちゃんと消える。*/
	sprite_putoff( pos , ptn );
	
	(void) getch();
	
	set_video_mode( oldmode );
}
