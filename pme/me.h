#ifndef ME_H
#define ME_H

#include "map.h"
#include "hayos.h"

enum{
// X zahyo

	LEFT_SWITCH_1	=  0,
	LEFT_SWITCH_2	=  8,
	MAP_WINDOW		= 16,
	RIGHT_SWITCH_2	=176,
	RIGHT_SWITCH_1	=184,
	CELL_WINDOW		=192,

	MESSAGE_COLUMN	= CELL_WINDOW / 8,
	INPUT_COLUMN	= MESSAGE_COLUMN,
	STATUS_COLUMN	= MESSAGE_COLUMN,

	MESSAGE_COLOUM	= MESSAGE_COLUMN,	// この二つは,スペルミス
	INPUT_COLOUM	= INPUT_COLUMN,

	SWITCH_COLOR_1	= 24,
	SWITCH_COLOR_2	= 26,

// Y zahyo

	ICON_TABLE		=128,

	STATUS_LINE		=21,
	MESSAGE_LINE	=22,	/* キャラクタ座標 */
	INPUT_LINE		=23,

// others

	MAP_WINDOW_WIDTH	= 20,
	MAP_WINDOW_HEIGHT	= MAP_HEIGHT,

	MESSAGE_LEN_MAX		= 128 / 8,
	INPUT_LEN_MAX		= MESSAGE_LEN_MAX,
	INPUT_SIZE_MAX		= INPUT_LEN_MAX + 1,
};

enum CmdStatus {	CONTINUE,	QUIT,	CLS		};

struct icon_table_t {
	const char name[9];
	char shortcut;
	CmdStatus (*func)( void );
};

struct event_list_t{
	int x,y;
	char *memo;
	event_list_t *next;
};
/* module "ME" */

extern int current_cell;
extern unsigned page_top , page_bottom;
extern Map map;
extern char default_file_name[];
extern char *cell_file_name;


/* module "ME0"  icon */


extern icon_table_t  icon_table[];
extern event_list_t *event_list;

void icon_table_init(void);
CmdStatus select_icon(unsigned no);
CmdStatus short_cut_key(int key);


/* module "ME1" */

void map_window_draw(MapScroll *map);
void map_window_scroll(MapScroll *orgmap,MapScroll *newmap);
void screen_init( void );
void disp_window_page( void );


/* others */

inline void clear_message_area(void)
{	box_fill( MESSAGE_COLOUM * 8, MESSAGE_LINE * 8, 128, 16 ,0 );	}

#endif
