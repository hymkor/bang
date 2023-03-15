#include <stdio.h>
#include <string.h>
#include <process.h>

#include "hayos.h"
#include "me.h"
#include "mice.h"

static void keybuf_clear()
{
	while( Kbhit() )
		(void) Getch();
	while( mouse() )
			;
}

static int AreYouSure(const char *message = "Are You Sure ?" )
{
	keybuf_clear();
	
	writes(MESSAGE_COLUMN , MESSAGE_LINE   , message );
	writes(MESSAGE_COLUMN , MESSAGE_LINE+1 , "Yes.[LEFT] /\"Y\"",1);
	writes(MESSAGE_COLUMN , MESSAGE_LINE+2 , "No..[RIGHT]/\"N\"",1);

	int result;
	for(;;){
		if( Kbhit() ){
			int ch=Getch();
			if( ch=='Y'  ||  ch=='y'  ||  ch=='\r' ){
				result = 1; break;
			}else{
				result = 0;	break;
			}
		}
		int trig=mouse();
		if( trig==1 ){
			result = 1;		break;
		}
		if( trig==2 ){
			result = 0;		break;
		}
	}
	box_fill( MESSAGE_COLUMN<<3 , MESSAGE_LINE<<3 , 8*8*2 , 8*3 , 0 );
	return result;
}
static CmdStatus replace(void)
{
	int x,y,dst_cell;
	
	writes( MESSAGE_COLUMN , MESSAGE_LINE , "To which cell?" );
	mouse_on();
	while( mouse(x,y) != 0 )	/* 離すのを待つ */
		;
	while( mouse(x,y) == 0 )	/* 押すのを待つ */
		;
	while( mouse(x,y) != 0 )	/* 離すのを待つ */
		;
	mouse_off();
	box_fill( MESSAGE_COLUMN<<3 , MESSAGE_LINE<<3 , 8*8*2, 8 , 0 );
	if( x < RIGHT_SWITCH_2  &&  x >= MAP_WINDOW ){
		dst_cell = map[ page_top  + ((x - MAP_WINDOW) >> 3 ) ][ y>>3 ];
	}else if( x > CELL_WINDOW  &&  y < ICON_TABLE ){
		dst_cell = ((x-CELL_WINDOW)>>3) + (y>>3)*16;
	}else{
		return CONTINUE;
	}
	writes( MESSAGE_COLUMN , MESSAGE_LINE , "[ ] to [ ]" );
	cell_putat( (MESSAGE_COLUMN+1)<<3 , MESSAGE_LINE<<3 , current_cell );
	cell_putat( (MESSAGE_COLUMN+8)<<3 , MESSAGE_LINE<<3 , dst_cell );
	if( AreYouSure() ){
		for(int i=0 ; i < map.Nlines() ; i++ ){
			for(int j=0 ; j < MAP_HEIGHT ; j++ ){
				if( map[i][j] == current_cell ){
						map[i][j] = dst_cell;
				}
			}
		}
		map_window_draw( &map[page_top] );
	}
	box_fill( MESSAGE_COLUMN<<3 , MESSAGE_LINE<<3 , 8*8*2 , 8*3 , 0 );
	return CONTINUE;

}
static CmdStatus save(void)
{
	writes( MESSAGE_COLUMN , MESSAGE_LINE , "SAVE FILE NAME ?" );
	input( INPUT_COLUMN , INPUT_LINE , default_file_name , 15 );
	map.save( default_file_name );
	
	clear_message_area();
	
	return CONTINUE;
}
static CmdStatus quit(void)
{
	if( AreYouSure("Save ?") )
		save();
	
	return AreYouSure("Quit Sure ?") ? QUIT : CONTINUE ; 
}

static CmdStatus load(void)
{
	writes( MESSAGE_COLUMN , MESSAGE_LINE , "LOAD FILE NAME ?" );
	input( INPUT_COLUMN , INPUT_LINE , default_file_name , 15 );
	map.load( default_file_name );
	
	clear_message_area();

	map_window_draw( &map[0] );
	page_top=0;
	page_bottom=MAP_WINDOW_WIDTH;

	return CONTINUE ;
}

event_list_t *event_list = (event_list_t*)NULL;

static CmdStatus where( void )
{
	keybuf_clear();
	for(;;){
		int x,y,trig;
		
		mouse_on();
		while( mouse(x,y) == 0 )	/* 押すのを待つ */
			;
		while( mouse(x,y) != 0 )	/* 離すのを待つ */
			;
		mouse_off();
		
		/****** 終了 ******/
		if( x >= CELL_WINDOW ){
			return CONTINUE;
			
		/****** 右スクロールスイッチ ******/
		}else if( x >= RIGHT_SWITCH_2 ){
			if( page_bottom < map.Nlines() ){
				map_window_scroll( &map[ page_top ] , &map[ page_top + 1 ] );
				++page_top;
				++page_bottom;
				disp_window_page();
			}
			
		/****** 左スクロールスイッチ ******/
		}else if( x < MAP_WINDOW ){
			if( page_top > 0 ){
				map_window_scroll( &map[ page_top ] , &map[ page_top - 1 ] );
				--page_top;
				--page_bottom;
				disp_window_page();
			}
			
		/***** 登録 ******/
		}else{
		
			xor_box_fill( x & ~7 , y & ~7 , 16 , 16 , 15 );
		
			int cx=((x-MAP_WINDOW)>>3) + page_top;
			int cy=  y            >>3  ;
			char buffer[32];
			
			sprintf( buffer , "MEMO[%3d,%3d]" , cx , cy );
			writes( MESSAGE_COLUMN , MESSAGE_LINE , buffer );
			buffer[0]=0;
			int len=input( INPUT_COLUMN , INPUT_LINE , ">" , buffer , 14 );
			if( len >= 0 ){
				event_list_t *neo=new event_list_t;
				if( neo==NULL  ||  (len > 0  &&  (neo->memo=new char[len+1])==NULL ) ){
					writes( MESSAGE_COLUMN , MESSAGE_LINE , "CANNOT RESERVE");
					box_fill( MESSAGE_COLUMN<<3 , MESSAGE_LINE<<3 , 8*8*2 , 8*3 , 0 );
					xor_box_fill( x & ~7 , y & ~7 , 16 , 16 , 15 );
					return CONTINUE;
				}
				event_list_t *ptr=event_list , **ptrptr=&event_list ;
				while( ptr != NULL  &&  ( ptr->x < cx  ||  (ptr->x == cx  &&  ptr->y < cy) )  ){
					ptrptr	= &ptr->next;
					ptr		=  ptr->next;
				}
				neo->next = ptr;
				neo->x = cx;
				neo->y = cy;
				if( len > 0 )
					strcpy( neo->memo , buffer );
				else
					neo->memo = NULL;
			
				*ptrptr   = neo;
			}
			box_fill( MESSAGE_COLUMN<<3 , MESSAGE_LINE<<3 , 8*8*2 , 8*3 , 0 );
			xor_box_fill( x & ~7 , y & ~7 , 16 , 16 , 15 );
		}
	}
}
static CmdStatus call_pe(void)
{
	if( AreYouSure() ){
		spawnlp( P_WAIT , "PE" , "PE.EXE" , cell_file_name );
		cell_load(cell_file_name);
		return CLS;
	}else{
		return CONTINUE;
	}
}
static CmdStatus clear(void)
{
	if( AreYouSure() )
		map.clear();
	
	return CONTINUE;
}

static CmdStatus widen(void)
{
	if( AreYouSure() ){
		if( map.alloc( map.Nlines()+8 ) ){
			writes( MESSAGE_COLUMN , MESSAGE_LINE , "FATAL ERROR" );
			(void)Getch();
			return QUIT;
		}
	}
	return CONTINUE;
} 


struct icon_table_t icon_table[]={
	{"QUIT",		'q',	quit	},
	{"SAVE",		's',	save	},
	{"LOAD",		'l',	load	},
	{"WHERE",		'w',	where	},
	{"WIDEN",		'W',	widen	},
	{"REPLACE",		'r',	replace	},
	{"CALL\"PE\"",	'p',	call_pe	},
};

void icon_table_init(void)
{
	for(int i=0 ; i<numof(icon_table) ; i++){
		writes( (CELL_WINDOW>>3)+(i&1)*8,(ICON_TABLE>>3)+(i>>1),icon_table[i].name );
	}
}

CmdStatus select_icon(unsigned no)
{
	if( no < numof( icon_table ) )
		return (*icon_table[no].func)();
	else
		return CONTINUE;
}

CmdStatus short_cut_key(int key)
{
	for(int i=0 ; i < numof(icon_table) ; i++){
		if( icon_table[ i ].shortcut == key ){
			return (*icon_table[ i ].func)();
		}
	}
}
