#ifndef SPRITE_H
#define SPRITE_H

#define VRAM_END		320*200

#include <stddef.h>

typedef char far *Pattern;
#define	NOPATTERN		(Pattern)0

extern char far *background;
extern unsigned short vram_offset;
extern Pattern pika_pattern[3];

enum { ATARI  =  0x40 };

int background_init(void);

class Sprite {
public:/* 静的メンバ */
	static short	draw ( unsigned short x , unsigned short y , Pattern pattern );
	static void		clear( short vramptr , Pattern pattern );
	static Pattern	load(const char *fname );
protected:
	short  x, y;	/* メンバー関数 X() Y() で返される画面位置 */
	short vramptr;	
	Pattern pattern;
public:
	
	void putat(int x,int y,Pattern=NOPATTERN);
	void putoff( void ){ clear(vramptr , pattern); pattern=NOPATTERN; }
	
	short X(void) const { return x; }
	short Y(void) const { return y; }
	
	void scrolls(int dx,int dy){ x += dx ; y += dy ; }

	 Sprite(void) : pattern(NULL) { }
	 Sprite(int X,int Y) : x(X) , y(Y) , pattern(NULL) { }
	~Sprite(void) { putoff(); }
};
int screen(int x, int y);	/* graphics zahyo */
int is_movable( int x1 ,int y1 , int x2 , int y2 , unsigned int limit );
							/* graphics zahyo */
#endif
