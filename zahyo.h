#ifndef Zahyo_H
#define Zahyo_H

extern "C" unsigned long intsqrt(unsigned long);

class Zahyo {
public:
	int	x,y;
	
	Zahyo(){ }
	Zahyo(int _x,int _y):x(_x),y(_y){ }
	
	Zahyo &operator += (const Zahyo &a)
	{ x+=a.x; y+=a.y; return *this; }

	Zahyo &operator -= (const Zahyo &a)
	{ x-=a.x; y-=a.y; return *this; }
	
	Zahyo operator + (const Zahyo &a) const
	{ Zahyo r=*this; return r+=a; }
	
	Zahyo operator - (const Zahyo &a) const
	{ Zahyo r=*this; return r-=a; }
	
	int operator == (const Zahyo &a)
	{ return  x==a.x  &&  y==a.y; }
	
	int operator != (const Zahyo &a)
	{ return  x!=a.x  ||  y!=a.y; }

};

void rotate(const Zahyo &source,int theta,Zahyo &destinate);
typedef Zahyo Coord,Position;

#endif
