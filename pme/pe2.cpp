#include <stdlib.h>

// #include "fix.h"
#include "hayos.h"
#include "mice.h"
#include "pe.h"

CmdStatus Rotate90(void)
{
	int dx,dy;
	char buffer[ 10 ];
	
	if( select_window(dx,dy) ) return CONTINUE;
	clear_message_line();
	
	for(int i=0 ; i<EditSize ; i++ ){
		for(int j=0 ; j<EditSize ; j++){

			if( atrbuf[ CurrentPattern.y + j ][ CurrentPattern.x +EditSize-i-1 ] ==0 ){
				GRAM[ dy+i ][ REAL_LEFT+dx+j ]=	GRAM[ CurrentPattern.y+j ][ REAL_LEFT+CurrentPattern.x+EditSize-i-1 ];
				atrbuf[ dy+i ][ dx+j ] = 0;

			}
		}
	}
	return CONTINUE;
}



#if 0
CmdStatus Rotate(void)
{
	int dx,dy;
	char buffer[ 10 ]="";
	
	if( select_window(dx,dy) ) return CONTINUE;
	input(0,MESSAGE_LINE,"angle:",buffer,sizeof(buffer) );
	clear_message_line();
	
	int degree = atoi( buffer );
	if( degree == 0 ) return 0;
	degree %= 360; if( degree < 0 ) degree += 360;
	
	for(int y=0;y<EditSize;y++){
		for(int x=0;x<EditSize;x++){
			// (CP+_x,CP+_y) >> (dx+x,dx+y) 転送
		
			int _x=(int)long( (x-EditSize/2)*COS[degree] +(y-EditSize/2)*SIN[degree] + EditSize/2),
				_y=(int)long(-(x-EditSize/2)*SIN[degree] +(y-EditSize/2)*COS[degree] + EditSize/2);
			if( _x < 0 || EditSize <=_x || _y < 0 || EditSize <=_y )
				continue;
			
			if( atrbuf[ CurrentPattern.y+_y ][ CurrentPattern.x+_x ] ==0 ){
				GRAM[ dy+y ][ REAL_LEFT+dx+x ]=
					GRAM[ CurrentPattern.y+_y ][ REAL_LEFT+CurrentPattern.x+_x ];
				atrbuf[ dy+y ][ dx+x ] = 0;
			}
		}
	}
	return CONTINUE;
}
#endif

static int _Line(int x1,int y1,int x2,int y2)
{
	unsigned dx,dy;// 長さ
	  signed sx,sy;// = -1,0,+1
	
	if( x1 < x2 ){
		dx = x2 - x1;
		sx = +1;
	}else if( x1 > x2 ){
		dx = x1 - x2;
		sx = -1;
	}else{// x1 == x2
		dx = 0;
		sx = 0;
	}
	
	if( y1 < y2 ){
		dy = y2 - y1;
		sy = 1;
	}else if( y1 > y2 ){
		dy = y1 - y2;
		sy = -1;
	}else{// y1==y2
		dy = 0;
		sy = 0;
	}
	
	int x=x1,	y=y1;
	int c=dx/2;
	
	while( (x != x2 || y!= y2) && 0<=x && x<EditSize && 0<=y && y<EditSize ){
		dotset(x,y,CurrentColor);
		if( c >= dx ){
			y += sy;
			c -= dx;
		}else{
			x += sx;
			c += dy;
		}
	}
	return 0;
}

CmdStatus Line(void)
{
	int x1,y1,x2,y2;
	
	writes(0,MESSAGE_LINE,"Select Start Point !",15);
	
	mouse_on();
	while( mouse(x1,y1) != 0 )
		;
	while( mouse(x1,y1) == 0 )
		;
	mouse_off();
	
	clear_message_line();
	
	if( x1 >= EDIT_RIGHT || y1 >= BOTTOMS )
		return CONTINUE;

	for(;;){
		writes(0,MESSAGE_LINE,"Select Next Point !",15);
		mouse_on();
		while( mouse(x2,y2) != 0 )
			;
		while( mouse(x2,y2) == 0 )
			;
		mouse_off();
		clear_message_line();
		
		if( x2 >= EDIT_RIGHT || y2 >= BOTTOMS )
			return CONTINUE;
		
		_Line(x1>>ScaleBit , y1>>ScaleBit , x2>>ScaleBit , y2>>ScaleBit );
		x1=x2;	y1=y2;
	}
}
CmdStatus Box(void)
{
	int x1,y1,x2,y2;
	
	writes(0,MESSAGE_LINE,"Select Start Point !",15);
	
// 始点の取得

	mouse_on();
	while( mouse(x1,y1) != 0 )
		;
	while( mouse(x1,y1) == 0 )
		;
	mouse_off();
	clear_message_line();

	if( x1 >= EDIT_RIGHT || y1 >= BOTTOMS )
		return CONTINUE;
	x1 >>= ScaleBit;	y1 >>= ScaleBit;
	
//  終点の取得

	writes(0,MESSAGE_LINE,"Select End Point !",15);

	mouse_on();
	while( mouse(x2,y2) != 0 )
		;
	while( mouse(x2,y2) == 0 )
		;
	mouse_off();
	clear_message_line();

	if( x2 >= EDIT_RIGHT || y2 >= BOTTOMS )
		return CONTINUE;
	x2 >>= ScaleBit;	y2 >>= ScaleBit;
	

	if( x1 > x2 ){ int tmp=x2; x2=x1; x1=tmp;}
	if( y1 > y2 ){ int tmp=y2; y2=y1; y1=tmp;}

	for(int y=y1; y<=y2; y++){
		for(int x=x1; x<=x2; x++){
			dotset( x,y,CurrentColor );
		}
	}
	return CONTINUE;
}
