#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

/* module "map" */

extern char far *cell_graphics;
int  cell_load(const char *fname);
void cell_putat(unsigned x,unsigned y,unsigned type);

enum{
	DEFAULT_MAP_SIZE	=80,
	MAP_WIDTH			=40,
	MAP_HEIGHT			=25,
};

struct MapScroll{
	unsigned char mapchs[ MAP_HEIGHT ];
	unsigned char &operator[](int n){ return mapchs[n]; }
};
class Map{
	MapScroll *map;
	unsigned   nlines;

public:
	operator const void*() const{ return (const void*)map; }
	int operator!() const{ return map==NULL ? 1 : 0 ; }
	unsigned Nlines(void)const{return nlines;}
	MapScroll &operator[](int n){ return map[n]; };

	int alloc(size_t n);
	int clear(void);
	int load(const char *fname);
	int save(const char *fname);

	 Map(void) : map( 0 ) , nlines( 0 ) { }
	 Map(size_t n){ alloc(n); }
	~Map()	{	if(map) free(map);	}

};

#endif
