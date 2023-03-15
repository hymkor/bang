#include "hayos.h"
#include "sprite.h"

int main( int argc , char **argv )
{
	int old_mode = get_video_mode();
	set_video_mode( 0x13 );
	
	for(int i=1 ; i < argc ; i++){
		Pattern ptn=Sprite::load( argv[i] );
		if( ptn != NULL ){
			Sprite::draw(140,90,ptn);
			Getch();
		}
	}
	
	set_video_mode( old_mode );
	return 0;
}
