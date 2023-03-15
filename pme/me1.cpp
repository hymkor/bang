#include <stdio.h>

#include "hayos.h"
#include "me.h"

static void cell_window_init(void)
{
	int cellno=0;
	for(int y=0 ; y<128 ; y+=8){
		for(int x=0 ; x<128 ; x+=8){
			cell_putat( CELL_WINDOW + x, y,cellno++);
		}
	}
}

void map_window_draw(MapScroll *map)
{
	for(int y=0; y<MAP_WINDOW_HEIGHT; y++){
		for(int x=0; x<MAP_WINDOW_WIDTH; x++){
			cell_putat( MAP_WINDOW + (x<<3) , y<<3 , map[x][y] );
		}
	}
}
void map_window_scroll(MapScroll *orgmap,MapScroll *newmap)
{
	for(int y=0; y<MAP_WINDOW_HEIGHT; y++){
		for(int x=0; x<MAP_WINDOW_WIDTH; x++){
			if( orgmap[x][y] != newmap[x][y] ){
				cell_putat( MAP_WINDOW + (x<<3) , y<<3 , newmap[x][y] );
			}
		}
	}
}

void disp_window_page( void )
{
	char buffer[16];
	sprintf( buffer , "WINDOW %3d-%3d" , page_top , page_bottom );
	writes( STATUS_COLUMN , STATUS_LINE , buffer , 9 );
}

void screen_init(void)
{
	cell_window_init();
	
	map_window_draw( &map[page_top] );
	icon_table_init();
	
	box_fill( LEFT_SWITCH_1,0,8,200,SWITCH_COLOR_1);
	box_fill( LEFT_SWITCH_2,0,8,200,SWITCH_COLOR_2);
	box_fill(RIGHT_SWITCH_2,0,8,200,SWITCH_COLOR_2);
	box_fill(RIGHT_SWITCH_1,0,8,200,SWITCH_COLOR_1);
	
	disp_window_page();
}
