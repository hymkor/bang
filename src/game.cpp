#pragma inline

#include "hayos.h"
#include "map.h"
#include "enemy.h"
#include "joy.h"
#include "game.h"

/* この関数は,重ね合せバッファにのみ描画する.
 *	ＶＲＡＭに書き込む関数(cell_putat)と分けたのは，
 *	後者はマップエディタで使うが，前者は使わないため．
 */

void bg_cell_putat(unsigned x,unsigned y,unsigned type)
{

/*** セルエリアのアドレスを計算する. ***/
	unsigned short adrs = x + 320*y + vram_offset;

	asm	mov ax,type
	asm	and ax,0x0F
	asm	shl ax,1
	asm	shl ax,1
	asm	shl ax,1

	asm	mov dx,type
	asm	and dx,0xF0
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1

	asm	add dx,ax
	
	asm	push es
	asm	push ds

	asm mov es,word ptr background[2]	/* <--- This is diffrent. */
	asm	mov di,adrs
	asm	lds si,cell_graphics
	asm	add si,dx

	asm mov ax,8
lp1:asm mov cx,8	// 8bytes movs
	asm rep movsb
	
	asm add si,128-8
	asm add di,320-8
	
	asm dec ax
	asm jne lp1
	
	asm pop ds
	asm pop es
	
}

/* マップセルの特定の縦一列の8ﾄﾞｯﾄを画面および背景バッファに書き込む.
 * これは,マップエディタから使われることはない.
 */

void cell_vertical_putat(unsigned short x , unsigned short y , unsigned short type , unsigned short cut)
{
	unsigned short adrs = x + 320*y + vram_offset;

/*** セルエリアのアドレスを計算する. ***/
	asm	mov ax,type
	asm	and ax,0x0F
	asm	shl ax,1
	asm	shl ax,1
	asm	shl ax,1

	asm	mov dx,type
	asm	and dx,0xF0
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1
	asm	shl dx,1

	asm	add dx,ax
	asm add dx,cut
	
	asm	push es
	asm	push ds

	asm	mov di,adrs
	asm	lds si,cell_graphics
	asm	add si,dx


/*** 画面に表示 ***/
	asm push si
	asm push di

	asm	mov ax,0xa000
	asm mov es,ax

	asm mov cx,8
lp1:asm movsb
	
	asm add si,128-1
	asm add di,320-1
	
	asm loop lp1
	
	asm pop di
	asm pop si

/*** 重ね合せバッファに書き込み ***/
	asm mov es,word ptr ss:background[2]

	asm mov cx,8
lp2:asm movsb
	
	asm add si,128-1
	asm add di,320-1
	
	asm loop lp2
	
	asm pop ds
	asm pop es
	
}

Stage *World::map;
unsigned World::top_byte=0,
		 World::top_bit =0,				/* ｘ座標０のマップ上での位置 */
		 World::tail_byte=MAP_WIDTH-1,
		 World::tail_bit=7;				/* ｘ座標３１９のマップ上での位置 */
Event *World::next_event;

void World::init( Stage *story )
{
	for(int x=0; x < MAP_WIDTH ; x++ ){
		for(int y=0; y < MAP_HEIGHT ; y++ ){
			   cell_putat( x<<3 , y<<3 , (*story)[ x ][ y ] );
			bg_cell_putat( x<<3 , y<<3 , (*story)[ x ][ y ] );
		}
	}
	 top_byte = 0;				 top_bit = 0;
	tail_byte = MAP_WIDTH-1;	tail_bit = 7;
	
	map = story;
	next_event = story->event_top();
}

int World::screen(int x, int y) // x,y : graphics座標
{
	return (*map)[ top_byte + ((x+top_bit)>>3) ][ y>> 3 ];
}

int World::is_crushed(int x1 ,int y1 , int x2 , int y2 , unsigned int limit )
{
	int cx1 = top_byte + ((x1+top_bit)>>3)	,
		cx2 = top_byte + ((x2+top_bit)>>3)	,
		cy1 = y1 >> 3	,
		cy2 = y2 >> 3	;
	
	return (*map)[cx1][cy1] >= limit  ||  (*map)[cx2][cy1] >= limit
		|| (*map)[cx1][cy2] >= limit  ||  (*map)[cx2][cy2] >= limit ;
}

int World::scroll( void )	/* マップが終わったら非0を返す */
{
	if( top_byte + MAP_WIDTH < map->Nlines()  ){
		box_fill( 0,0, 1,200, 0 );
		for(int y=0; y < MAP_HEIGHT; y++ )
			cell_vertical_putat( 319 , y<<3 , (*map)[ tail_byte ][ y ] , tail_bit );
#ifdef UNROLL
		if( burner_switch ){
			UnrollSprite::scroll_flush( +1 );
		}else{
#endif
			add_vram_offset_13h( +1 );
#ifdef UNROLL
			UnrollSprite::flush();
		}
#endif
		
		if( ++tail_bit >= 8 ){
			tail_bit = 0;
			tail_byte++;
		}
		
		if( ++top_bit >= 8 ){
			while( next_event != 0  &&  next_event->Pos() <= top_byte + MAP_WIDTH-2 ){
				Enemy::yield( (next_event->Pos() - top_byte)<<3 , next_event->Y()<<3 ,
								next_event->Type() ,  next_event->Info() );
				next_event = next_event->next;
			}
			top_bit = 0;
			++top_byte;
		}
		return SCROLL_CONT;
	}else{
		return  Enemy::number()==0  ?  STAGE_CLEAR : SCROLL_STOP ;
	}
}
