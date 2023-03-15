#include <stdlib.h>
#include "file.h"

int main(int argc,char **argv)
{
	unsigned width=320;
	
	if( argc < 4 ){
		puts("usage : shiftsp  <filename>  <shift-x>  <shift-y>  [ <screen-width> ]");
		return 0;
	}
	if( argc == 5 ){
		width = atoi( argv[4] );
	}
	
	File file( argv[1] , "r+b" );
	if( file==NULL ) return 1;
	
	rewind( file );
	unsigned short skip = getw( file );
	skip += atoi( argv[2] ) + atoi( argv[3] )*width;
	rewind( file );
	putw( skip , file );
	
	return 0;
}
