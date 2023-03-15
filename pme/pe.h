#ifndef PE_H
#define PE_H

#include <stddef.h>
#include "Zahyo.h"

extern char atrbuf[ 128 ][ 128 ];
extern Zahyo CurrentPattern;
extern int CurrentColor;
extern int Scale,ScaleBit;
extern int EditSize;

enum{
// Y ç¿ïW
	BOTTOMS		=128,
	PALET_TOP	=152,

	RGB_COLUMN	=3,
	RGB_LINE	=16,
	TITLE_LINE  =22,
	MESSAGE_LINE=23,
	USAGE_LINE	=24,
					 
// X ç¿ïW			 
	EDIT_SCREEN_SIZE=128,
	EDIT_RIGHT	=128,
	ICON_LEFT	=128,	ICON_COLUMN=16,
	ICON_RIGHT	=192,
	REAL_LEFT	=192,

// ÇªÇÃëº
	PALET_DOT_SIZE = 5,
	PALET_NUM_LINE =64,

};
enum CmdStatus { CONTINUE , ABORT };

inline void clear_message_line()
{	box_fill(0,MESSAGE_LINE<<3,320,8,0);	}

inline void clear_usage_line()
{	box_fill(0,USAGE_LINE<<3,320,8,0);	}


// module PE

extern int CurrentColor;
extern char default_file_name[];

void dotset(int x,int y,int color);
void edit_screen_init();
void edit_screen_flush();
void disp_current_color( void );

// module PE0

void icon_init(void);
CmdStatus select_icon(int no);
CmdStatus short_cut_key(int key);
void palette_table_init();
void select_palet(int x,int y);
int select_window(int &x,int &y);


// module PE1

void clear_message_line();

int  save_pattern(int x1,int y1,int x2,int y2,const char *fname);
int  load_pattern(int x1,int y1,int x2,int y2,const char *fname);
int write_pattern(int x1,int y1,int x2,int y2,const char *fname);

// module PE2

CmdStatus Rotate90(void);
CmdStatus Line(void);
CmdStatus Box(void);

#endif