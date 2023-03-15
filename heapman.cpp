#include "heapman.h"

void *HeapMan::alloc()
{
	void *result;

	if( garbages ){	
		result=garbages;
		garbages = *(void**)garbages;
	}else{
		if( left<objsiz && (buffer=new char[left=bufsiz])==NULL ){
			left=0;		result=NULL;
		}else{
			result = &buffer[ left -= objsiz ];
		}
	}
	return result;
}

void *HeapMan::alloc( size_t n )
{
	size_t s = objsiz*n ;
	return s>left ? new char[s] : &buffer[ left-=s];
}

void HeapMan::free( void *obj )
{
	*(void**)obj = garbages;
	garbages = obj;
}
