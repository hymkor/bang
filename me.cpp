#include <stdio.h>
#include <string.h>

#include "hayos.h"
#include "map.h"
#include "me.h"
#include "mice.h"

int current_cell=0;
unsigned page_top = 0 ,
		 page_bottom = MAP_WINDOW_WIDTH;
Map map;
char default_file_name[FILENAME_MAX] = "\0" ;
char *cell_file_name;

static void set_current_cell( int cellno )
{
	static enum{ OFF , ON  } cell_cursor = OFF;

	if( cell_cursor == ON )
		xor_box( CELL_WINDOW + ((current_cell & 15)<<3)  ,  (current_cell>>4)<<3  ,  7  ,  7  ,  15   );
	current_cell = cellno;
	cell_cursor = ON;
	xor_box( CELL_WINDOW + ((current_cell & 15)<<3)  ,  (current_cell>>4)<<3  ,  7  ,  7  ,  15   );
}


int main(int argc,char **argv)
{
	if( argc < 2 ){
		fputs("usage : me <pattern-file> [ <map-file> ]",stderr);
		return 1;
	}
	if(	cell_load(cell_file_name=argv[1]) ){
		fputs("pattern file load error",stderr);
		return 1;
	}
	if( argc >= 3){
		if( map.load(argv[2]) ){
			fputs("cell file load error  or  memory allocation error\n",stderr);
			return 1;
		}
		strcpy(default_file_name , argv[2] );
	}else{
		if( map.alloc(40) ){
			fputs("memory allocation error\n",stderr);
			return 1;
		}
	}
	int oldmode=get_video_mode();
	set_video_mode(0x13);
	
	screen_init();

	if( mouse_init() ){
		set_video_mode(oldmode);
		fputs( "マウスドライバが、常駐されていません。\n",stderr);
		return 1;
	}
	mouse_on();

	set_current_cell( 0 );

	int x,y,trig;
	for(;;){
		int _x,_y,_trig;
		do{
			_x=x; _y=y; _trig=trig;
			trig=mouse(x,y);
			if( Kbhit() ){
				mouse_off();
				if( short_cut_key( Getch() ) != 0 )
					goto exit;
				mouse_on();
			}
		}while( x==_x && y==_y && trig==_trig);

		if( trig != 0 ){
			mouse_off();
			
			if( x >= CELL_WINDOW ){

				if( y >= ICON_TABLE ){		/***** アイコン選択 *****/

					int icon=((y-ICON_TABLE)>>3)*2 + ((x-CELL_WINDOW)>>6);
					xor_box_fill( (icon&1) ? (CELL_WINDOW+64) : CELL_WINDOW  , ICON_TABLE+((icon>>1)<<3) , 64 , 8 , 1 );
					switch( select_icon( icon ) ){
					case QUIT:
						goto exit;
					case CONTINUE:
						xor_box_fill( (icon&1) ? (CELL_WINDOW+64) : CELL_WINDOW  , ICON_TABLE+((icon>>1)<<3) , 64 , 8 , 1 );
						break;
					case CLS:
						screen_init();
						break;
					}

				}else{						/****** セル選択 *****/

					set_current_cell( ((y&~7)<<1)+((x-CELL_WINDOW)>>3)  );

				}

			}else if( x >= RIGHT_SWITCH_2 ){/****** 右スクロールスイッチ ******/
			
				if( page_bottom < map.Nlines() ){
					map_window_scroll( &map[ page_top ] , &map[ page_top + 1 ] );
					++page_top;
					++page_bottom;
					disp_window_page();
					
				}

			}else if( x >= MAP_WINDOW ){	/***** マップ書き込み ******/
				if( trig == 1 ){
				
					cell_putat( x & ~7 , y & ~7 ,current_cell);
					map[ page_top + ((x - MAP_WINDOW) >> 3 ) ][ y>>3 ] = current_cell;

				}else if( trig == 2 ){		/****** スポイトによるセル選択 ******/
				
					set_current_cell( map[ page_top + ((x - MAP_WINDOW) >> 3 ) ][ y>>3 ]  );

					/* 選択セルをフラッシュさせる. */
					xor_box_fill( x & ~7 , y & ~7 , 8 , 8 , 15 );
						irq0_sleep(2);
						
					xor_box_fill( x & ~7 , y & ~7 , 8 , 8 , 15 );
				}
				
			}else{ 							/****** 左スクロールスイッチ ******/

				if( page_top > 0 ){
					map_window_scroll( &map[ page_top ] , &map[ page_top - 1 ] );
					--page_top;
					--page_bottom;
					disp_window_page();
				}
				
			}
			mouse_on();
		}
	}
exit:
	mouse_off();
	set_video_mode(oldmode);
	for(event_list_t *ptr=event_list ; ptr !=NULL ; ptr=ptr->next ){
		if( ptr->memo != NULL )
			printf( "%d %d %s\n" , ptr->x , ptr->y , ptr->memo );
		else
			printf( "%d %d\n", ptr->x , ptr->y );
	}
	return 0;
}
