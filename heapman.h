#ifndef HeapMan_H
#define HeapMan_H

#include <assert.h>
#include <stddef.h>

class HeapMan {

	size_t	objsiz,bufsiz,left;
	void	*garbages;
	char	*buffer;

public:

	HeapMan( size_t s, size_t n=256 )
	: objsiz(s) , bufsiz(s*n) , left(0) ,
	  buffer(NULL) , garbages(NULL)
	{  assert( s >= sizeof(void*) );  }
	
	void *alloc();
	void *alloc(size_t);
	void  free(void *);

};

#endif
