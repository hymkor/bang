#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "heapman.h"

extern int burner_switch;
void bg_cell_putat(unsigned x,unsigned y,unsigned type);

class Stage;

class Event{
	friend class Stage;
	friend class World;
	
	static HeapMan heapman;
	Event *next;
	int pos,y,type;
	int info[15];
public:
	int Pos( void ) const { return pos; }
	int Y( void ) const { return y; }
	int Type( void ) const { return type; }
	int *Info( void ) const { return info; }
	
	void *operator new(size_t){ return heapman.alloc(); }
	void operator delete(void *obj){ heapman.free( obj ); }
};

class Stage : public Map {
	Event *event;
public:
	enum{ SUCCESS , CANT_OPEN , BAD_EOF , CANT_MAP_OPEN , SYNTAX_ERROR , HEAP_ERR , LOAD_ERROR , NUM_OF_ERR};
	static char *errmsg[NUM_OF_ERR];

	Event *event_top( void ) const { return event; }
	int load(const char *fname);
	char stage_name[20];
};

class World{	/* オブジェクトは静的な一つしか存在しない */
	static Stage *map;
	static Event *next_event;
	static unsigned top_byte,	top_bit;	/* ｘ座標０のマップ上での位置 */
	static unsigned tail_byte,	tail_bit;	/* ｘ座標３１９のマップ上での位置 */
public:
	enum{ SCROLL_CONT , SCROLL_STOP , STAGE_CLEAR };
	static void init(Stage *story);
	static int	scroll( void );				/* マップが終わったら非0を返す */
	static int	screen(int x, int y);
	static int	is_crushed(int x1 ,int y1 , int x2 , int y2 , unsigned int limit );
	static int	is_movable(int x1 ,int y1 , int x2 , int y2 , unsigned int limit )
	{	return 8 < x1  &&  x2 < 312  &&  8 < y1  &&  y2 < 192  &&  !is_crushed(x1,y1,x2,y2,limit ) ; }
};

#endif
