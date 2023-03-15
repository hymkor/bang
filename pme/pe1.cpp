#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hayos.h"
#include "pe.h"

int save_pattern(int x1,int y1,int x2,int y2,const char *fname)
{
	FILE *fp=fopen( fname,"wb" );
	if( fp==NULL){
		writes(0,MESSAGE_LINE,"Cannot open the file !",15);
		Getch();
		clear_message_line();
		return 1;
	}
	for(int y=y1; y<y2; y++){
		for(int x=x1; x<x2; x++){
			putc( GRAM[ y ][ x + REAL_LEFT ],fp);
		}
	}
	unsigned bitbuf=0,bitcnt=0;
	for(    y=y1; y<y2; y++){
		for(int x=x1; x<x2; x++){
			bitbuf <<=1;
			if( atrbuf[ y ][ x ] !=0 )
				bitbuf |= 1;
			if( ++bitcnt >=8 ){
				putc( bitbuf , fp );
				bitbuf = bitcnt = 0;
			}
		}
	}
	fclose(fp);
	clear_message_line();
	return 0;
}
int load_pattern(int x1,int y1,int x2,int y2,const char *fname)
{
	int result=0;
	FILE *fp=fopen( fname,"rb" );
	if( fp==NULL){
		writes(0,MESSAGE_LINE,"Cannot open the file !",15);
		Getch();
		clear_message_line();
		return 1;
	}
	for(int y=y1; y<y2; y++){
		for(int x=x1; x<x2; x++){
			if( feof(fp) ){
				writes(0,MESSAGE_LINE,"File is too small !",15);
				Getch();
				result = 2;
				goto exit;
			}
			GRAM[ y ][ x + REAL_LEFT ]= getc(fp) & 0xff ;
		}
	}
	unsigned bitbuf=0,bitcnt=0;
	for(    y=y1; y<y2; y++){
		for(int x=x1; x<x2; x++){
			if( bitcnt == 0 ){
				if( feof(fp) ){
					writes(0,MESSAGE_LINE,"File is too small !",15);
					Getch();
					result = 2;
					goto exit;
				}
				bitbuf = getc(fp) & 0xff;
				bitcnt = 8;
			}
			bitbuf <<=1;
			if( bitbuf & (1<<8) ){
				atrbuf[ y ][ x ]= -1;
				GRAM[ y ][ x + REAL_LEFT ]=0;
			}else{
				atrbuf[ y ][ x ]= 0;
			}
			bitcnt--;
		}
	}
	
exit:
	
	fclose(fp);
	clear_message_line();
	edit_screen_flush();
	return result;
}

static void back_putw(int w,fpos_t *pos,FILE *fp)
{
	fpos_t tmp;
	fgetpos( fp, &tmp);
	fsetpos( fp,  pos);
	putw(w,fp);
	fsetpos( fp, &tmp);
}

int write_pattern(int x1,int y1,int x2,int y2,const char *fname)
{
	char _linesize[10]="320";
	int	len=input( 0, MESSAGE_LINE,"Skip Size:",_linesize,sizeof(_linesize));
	int linesize=( len==0 ? 320 : atoi(_linesize) );
	
	FILE *fp=fopen( fname,"wb" );
	if( fp==NULL){
		writes(0,MESSAGE_LINE,"Cannot open the file !",15);
		Getch();
		clear_message_line();
		return 1;
	}
	
	
	fpos_t back;
	enum{ SKIP , DRAW } mode = SKIP;
	
	unsigned countor=0;			// DRAW-MODEの時だけ意味あり
	
	Zahyo lastdot( x1, y1);		// 
	
	for(int y=y1; y<y2; y++){
		for(int x=x1; x<x2; x++){
			switch( mode ){
			case SKIP:
				if( atrbuf[ y ][ x ] == 0 ){
					putw( (x-lastdot.x)+linesize*(y-lastdot.y) , fp );
					fgetpos( fp, &back );
					putw( 0 , fp); // この位置に後でバイト数を入れる
					
					putc( GRAM[ y ][ x+REAL_LEFT ] , fp);
					countor=1;
					mode=DRAW;
					
				}
				break;
			case DRAW:
				if( atrbuf[ y ][ x ] == 0){
					putc( GRAM[ y ][ x+REAL_LEFT ] , fp);
					++countor;
				}else{
					back_putw( countor, &back, fp);
					
					lastdot.x=x; lastdot.y=y;
					mode=SKIP;
				}
				break;
			}
		}// end of X loop
		
		if( mode == DRAW ){
			back_putw( countor , &back, fp);
			
			lastdot.x=x; lastdot.y=y;
			mode=SKIP;
		}
		
	}// end of Y loop
	
	if( mode== DRAW )
		back_putw( countor, &back, fp);
		
	putw( 0 , fp );// Skip Byte なし
	putw( 0 , fp );// Draw Byte なし => 描画終了マーク
	
	fclose(fp);
	clear_message_line();
	return 0;
}