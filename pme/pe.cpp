// #pragma inline
#include <stdio.h>
#include <string.h>

// #include "fix.h"
#include "hayos.h"
#include "mice.h"
#include "zahyo.h"
#include "palette.h"
#include "pe.h"

Zahyo CurrentPattern(0,0);

int CurrentColor=15;
int Scale   = 4,// or  8,16
	ScaleBit= 2,// or  3, 4
	EditSize=32;// or 16, 8

char default_file_name[FILENAME_MAX] = "\0";

char atrbuf[128][128];

void dotset(int x,int y,int color)
{
	int X=x<<ScaleBit,
		Y=y<<ScaleBit;
	
	if( color < 0 ){
		box_fill(X,Y,Scale,Scale,0);
		GRAM[ Y+Scale/2 ][ X+Scale/2 ]	= (x%8==7 || y%8==7) ? 3:7;
		GRAM[ CurrentPattern.y+y ][ REAL_LEFT+CurrentPattern.x+x ]=0;
		atrbuf[ CurrentPattern.y+y ][ CurrentPattern.x+x]=-1;
	}else{
		box_fill(X,Y,Scale,Scale,color);
		GRAM[ CurrentPattern.y+y ][ REAL_LEFT+CurrentPattern.x+x ]=color;
		atrbuf[ CurrentPattern.y+y ][ CurrentPattern.x+x]= 0;
	}
}
void edit_screen_flush()
{
	for(int y=0; y<EditSize; y++){
		for(int x=0; x<EditSize; x++){
			if( atrbuf[ CurrentPattern.y+y ][ CurrentPattern.x+x ] != 0){
				dotset(x,y,atrbuf[CurrentPattern.y+y][CurrentPattern.x+x]);
			}else{
				dotset(x,y,GRAM[CurrentPattern.y+y][ REAL_LEFT+CurrentPattern.x+x]);
			}
		}
	}
}

void edit_screen_init()
{
	for(int y=0; y<EditSize; y++){
		for(int x=0; x<EditSize; x++){
			dotset(x,y,-1);
		}
	}
}

void disp_current_color( void )
{
	box_fill( 0 , RGB_LINE<<3 , 64*2 + ((RGB_COLUMN+6)<<3)   , 24 , 0 );
	if( CurrentColor < 0 ){
		box_fill( 0 , BOTTOMS+8,  24 , 16 , 0 );
		box_fill( 11, BOTTOMS+15 , 2 ,  2 , 15);
		writes( 0 , RGB_LINE , "BGC" , 15 );
	}else{
		Palette palet( CurrentColor );
		palet.read();
		
		char buffer[32];
		/* 赤 */
		sprintf( buffer , "R[%3d]" , palet.red );
		writes( RGB_COLUMN , RGB_LINE   , buffer , 12 );
		box_fill( (RGB_COLUMN+6)<<3 , RGB_LINE<<3 , palet.red * 2 , 7 , 12 );
		
		/* 緑 */
		sprintf( buffer , "G[%3d]" , palet.green );
		writes( RGB_COLUMN , RGB_LINE+1 , buffer , 10 );
		box_fill( (RGB_COLUMN+6)<<3 , (RGB_LINE+1)<<3 , palet.green * 2 , 7 , 10 );
		
		/* 青 */
		sprintf( buffer , "B[%3d]" , palet.blue );
		writes( RGB_COLUMN , RGB_LINE+2 , buffer ,  9 );
		box_fill( (RGB_COLUMN+6)<<3 , (RGB_LINE+2)<<3 , palet.blue * 2 , 7 , 9 );
		
		sprintf( buffer , "%3d" , CurrentColor );
		writes( 0 , RGB_LINE , buffer, 15 );
		box_fill( 0 , BOTTOMS+8,  24 , 16 , CurrentColor );
		
	}
	
}

int main(int argc,char **argv)
{
	VgaSave vms;
	if( !vms ){
		fputs( "ヒープエラー",stderr );
		return 1;
	}
	if( mouse_init() ){
		fputs( "マウスドライバが、常駐されていません。\n",stderr);
		return 1;
	}
	set_video_mode( 0x13 );
	icon_init();
	palette_table_init();
	edit_screen_init();
	memset(atrbuf , -1, sizeof( atrbuf ) );
	
	if( argc >= 2){
		if( load_pattern( 0, 0, 128, 128, argv[1] ) == 0 ){
			strcpy( default_file_name , argv[1] );
		}
	}
	
	xor_box( REAL_LEFT+CurrentPattern.x, CurrentPattern.y, EditSize-1, EditSize-1, 15);
	writes( 0 , USAGE_LINE , "[LEFT]..PSET/SELECT  [RIGHT]..SPOIT",1);
	writes( 31, RGB_LINE   , "1993 (C)" );
	writes( 31, RGB_LINE+1 , "K.Hayama" );
	disp_current_color();
	
	mouse_on();
	
	int x,y,trig;
	for(;;){
		int _x,_y,_trig;
		for(;;){
			_x=x; _y=y; _trig=trig;
			trig=mouse(x,y);
			if( Kbhit() ){
				mouse_off();
				xor_box( REAL_LEFT+CurrentPattern.x, CurrentPattern.y, EditSize-1, EditSize-1, 15);
				clear_usage_line();
				if( short_cut_key( Getch() ) == ABORT )
					goto exit;
				writes( 0 , USAGE_LINE , "[LEFT]..PSET/SELECT  [RIGHT]..SPOIT",1);
				xor_box( REAL_LEFT+CurrentPattern.x, CurrentPattern.y, EditSize-1, EditSize-1,15);
				mouse_on();
			}
			if( x!=_x || y!=_y || trig!=_trig ){
				if( trig != 0 ){
					mouse_off();
					xor_box( REAL_LEFT+CurrentPattern.x, CurrentPattern.y,
								 EditSize-1, EditSize-1, 15);

					if( y < BOTTOMS ){

						if( x < EDIT_RIGHT ){
							int _x=x>>ScaleBit, _y=y>>ScaleBit;
							// Edit Screen
							if( trig & 1 ){
								dotset(_x,_y,CurrentColor);
							}else if( trig & 2 ){
								if( atrbuf[ CurrentPattern.y+_y ][ CurrentPattern.x+_x ] == 0 ){
									CurrentColor=GRAM[CurrentPattern.y+_y][REAL_LEFT+CurrentPattern.x+_x];
								}else{
									CurrentColor = -1;
								}
								disp_current_color();
							}
						}else if( x < ICON_RIGHT ){
							clear_usage_line();
							if( select_icon(y>>3)==ABORT )
								goto exit;
							writes( 0 , USAGE_LINE , "[LEFT]..PSET/SELECT  [RIGHT]..SPOIT",1);
						}else{
							// Real Screen
							int xsize=EDIT_RIGHT/ Scale,
								ysize=BOTTOMS	/ Scale;
							CurrentPattern.x = (x-REAL_LEFT) / xsize * xsize;
							CurrentPattern.y =  y            / ysize * ysize;
							edit_screen_flush();
						}

					}else if( PALET_TOP <= y ){

						select_palet(x,y);
						disp_current_color();

					}
					xor_box( REAL_LEFT+CurrentPattern.x, CurrentPattern.y, EditSize-1, EditSize-1,15);
					mouse_on();
				}
			}/* if( trig!=0 ) */
		}/* if( x!=.... */
	}/* for(;;) */
	
exit:	return 0;
}

