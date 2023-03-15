#include <stdio.h>
#include <string.h>
#include "hayos.h"
#include "mice.h"
#include "pe.h"

static void keybuf_clear()
{
	while( Kbhit() )
		(void) Getch();
	while( mouse() )
			;
}

static CmdStatus AreYouSure(const char *message = "Are You Sure ?")
{
	keybuf_clear();
	
	writes(0,MESSAGE_LINE,	message);
	writes(0,USAGE_LINE,	"Yes..[LEFT]/\"Y\"  No..[RIGHT]/\"N\"",1);
	for(;;){
		if( Kbhit() ){
			int ch=Getch();
			clear_message_line();
			clear_usage_line();
			if( ch=='Y'  ||  ch=='y' ||  ch=='\r'){
				keybuf_clear();
				return CONTINUE;
			}else{
				keybuf_clear();
				return ABORT;
			}
		}
		int trig=mouse();
		if( trig==1 ){
			clear_message_line();
			clear_usage_line();
			keybuf_clear();
			return CONTINUE;
		}
		if( trig==2 ){
			clear_message_line();
			clear_usage_line();
			keybuf_clear();
			return ABORT;
		}
	}
}
static CmdStatus Clear()
{
	if( AreYouSure()==CONTINUE )
		edit_screen_init();

	return CONTINUE;
}

static CmdStatus BgColor(){ CurrentColor=-1; disp_current_color(); return CONTINUE;}

static void modify_position()
{
	EditSize= EDIT_SCREEN_SIZE / Scale;
	
	CurrentPattern.x = CurrentPattern.x / EditSize * EditSize;
	CurrentPattern.y = CurrentPattern.y / EditSize * EditSize;
	
	edit_screen_flush();
}

static CmdStatus Narrow()
{
	if( ScaleBit < 4 ){
		Scale <<= 1;	++ScaleBit;
		modify_position();
	}
	return CONTINUE;
}
static CmdStatus Widen()
{
	if( ScaleBit > 2 ){
		Scale >>= 1;	--ScaleBit;
		modify_position();
	}
	return CONTINUE;
}
static CmdStatus SaveWindow()
{
	char fname[ FILENAME_MAX ] = "\0";
	
	writes( 0 , USAGE_LINE , "ESC...CANCEL" , 1 );
	int len=input( 0, MESSAGE_LINE,"File Name:",fname,sizeof(fname));
	clear_message_line();
	clear_usage_line();
	
	if( len<=0 ) return CONTINUE;

	save_pattern(	CurrentPattern.x,			CurrentPattern.y,
					CurrentPattern.x+EditSize,	CurrentPattern.y+EditSize,
					fname);
	return CONTINUE;
}
static CmdStatus LoadWindow()
{
	char fname[ FILENAME_MAX ] = "\0";
	
	writes( 0 , USAGE_LINE , "ESC...CANCEL" , 1 );
	int len=input( 0, MESSAGE_LINE,"File Name:",fname,sizeof(fname));
	clear_message_line();
	clear_usage_line();
	
	if( len <= 0 ) return CONTINUE;

	load_pattern(	CurrentPattern.x,			CurrentPattern.y,
					CurrentPattern.x+EditSize,	CurrentPattern.y+EditSize,
					fname);
	return CONTINUE;
}
static CmdStatus WriteWindow()
{
	char fname[ FILENAME_MAX ] = "\0";
	
	writes( 0 , USAGE_LINE , "ESC...CANCEL" , 1 );
	int len=input( 0, MESSAGE_LINE,"File Name:",fname,sizeof(fname));
	clear_message_line();
	clear_usage_line();
	
	if( len <= 0 ) return CONTINUE;

	write_pattern(	CurrentPattern.x ,			CurrentPattern.y,
					CurrentPattern.x+EditSize,	CurrentPattern.y+EditSize,
					fname);
	return CONTINUE;
}

static CmdStatus SaveAll()
{
	writes( 0 , USAGE_LINE , "ESC...CANCEL" , 1 );
	int len=input( 0, MESSAGE_LINE,"File Name:",default_file_name,39);
	clear_message_line();
	clear_usage_line();
	
	if( len <= 0 ) return CONTINUE;

	save_pattern(0,0,128,128,default_file_name);
	return CONTINUE;
}
static CmdStatus LoadAll()
{
	writes( 0 , USAGE_LINE , "ESC...CANCEL" , 1 );
	int len=input( 0, MESSAGE_LINE,"File Name:",default_file_name,39);
	clear_message_line();
	clear_usage_line();
	
	if( len <= 0 ) return CONTINUE;

	load_pattern(0,0,128,128,default_file_name);
	return CONTINUE;
}
static CmdStatus Quit()
{
	if( AreYouSure("Save ?")==CONTINUE )
		SaveAll();
		
	return AreYouSure("Quit Sure ?")==CONTINUE ? ABORT : CONTINUE;
}

int select_window(int &x,int &y)
{
	int dx,dy;
	
	writes(0,MESSAGE_LINE,"select destinate area.");
	xor_box( REAL_LEFT+CurrentPattern.x, CurrentPattern.y, EditSize-1, EditSize-1, 14);
	mouse_on();
	while( mouse(dx,dy) != 0 )
		;
	while( mouse(dx,dy) == 0 )
		;
	mouse_off();
	xor_box( REAL_LEFT+CurrentPattern.x, CurrentPattern.y, EditSize-1, EditSize-1, 14);
	clear_message_line();
	
	if( dx < REAL_LEFT || dy > BOTTOMS )
		return 1;
	
	x = ((dx-REAL_LEFT)/EditSize)*EditSize;
	y = ( dy           /EditSize)*EditSize;
	
	return 0;
}

static CmdStatus Copy()
{
	int dx,dy;
	
	if( select_window(dx,dy) ) return CONTINUE;
	
	for(int y=0;y<EditSize;y++){
		for(int x=0;x<EditSize;x++){
			if( atrbuf[ CurrentPattern.y+y ][ CurrentPattern.x+x ] ==0 ){
				GRAM[ dy+y ][ REAL_LEFT+dx+x ]=
					GRAM[ CurrentPattern.y+y ][ REAL_LEFT+CurrentPattern.x+x ];
				atrbuf[ dy+y ][ dx+x ] = 0;
			}
		}
	}
	return CONTINUE;
}

static struct IconTable_t{
	const char name[9];
	CmdStatus (*func)();
	int shortcut;
} IconTable[]={
	{"BGCOLOR",	BgColor,	'b'},
	{"LINE",	Line,		'l'},
	{"BOX",		Box,		'x'},
	{"WIDEN",	Widen,		'w'},
	{"NARROW",	Narrow,		'n'},
	{"COPY",	Copy,		'c'},
	{"ROT90",	Rotate90,	'r'},
	{"SAVE",	SaveWindow,	's'},
	{"LOAD",	LoadWindow,	'l'},
	{"SAVEALL",	SaveAll,	'S'},
	{"LOADALL",	LoadAll,	'L'},
	{"WRITE",	WriteWindow,'W'},
	{"CLS",		Clear,		'C'},
	{"QUIT" ,	Quit,		'q'},
};

void icon_init(void)
{
	IconTable_t *ptr=IconTable;
	
	for(int y=0; y<numof(IconTable); y++)
		writes( ICON_COLUMN , y , ptr++ ->name );
}

CmdStatus select_icon(int no)
{
	if( no < numof(IconTable) ){
		xor_box_fill( ICON_COLUMN << 3 , no<<3 , 8<<3 , 8 , 8 );
		CmdStatus result = (*IconTable[no].func)();
		xor_box_fill( ICON_COLUMN << 3 , no<<3 , 8<<3 , 8 , 8 );
		return result;
	}else{
		return CONTINUE;
	}
}

CmdStatus short_cut_key(int key)
{
	for(int i=0; i<numof(IconTable); i++){
		if( IconTable[i].shortcut == key ){
			xor_box_fill( ICON_COLUMN << 3 , i<<3 , 8<<3 , 8 , 8 );
			CmdStatus result = (*IconTable[i].func)();
			xor_box_fill( ICON_COLUMN << 3 , i<<3 , 8<<3 , 8 , 8 );
			return result;
		}
	}
	return CONTINUE;
}

void palette_table_init()
{
	for(int i=0; i<256; i++){
		box_fill( 			(i%PALET_NUM_LINE)*PALET_DOT_SIZE,
			 	  PALET_TOP+(i/PALET_NUM_LINE)*PALET_DOT_SIZE,
			 	  PALET_DOT_SIZE-1,
			 	  PALET_DOT_SIZE-1,
			 	i );
	}
}
void select_palet(int x,int y)
{
	int c=x/PALET_DOT_SIZE+((y-PALET_TOP)/PALET_DOT_SIZE)*PALET_NUM_LINE;
	if( c < 256 )
		CurrentColor = c;
}
