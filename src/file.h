#ifndef	FILE_H
#define FILE_H

#include <stdio.h>

class File {
	FILE *fp;
	int lnum;
public:
	 File( const char *fname , const char *mode ) : fp( fopen( fname , mode ) ) , lnum(0) { /* no-operation */ }
	~File( void ){ if( fp != NULL ) fclose( fp ); }

	operator FILE* ( void ){ return fp; }
	FILE *operator->( void ){ return fp; }
	int operator! ( void ) const { return fp==NULL; }
	
	File &puts(char *buffer){ lnum++;fputs(buffer,fp); return *this; }
	char *gets(char *buffer,size_t size){ lnum++; return fgets(buffer,size,fp); }
	size_t read(void *ptr,size_t size,size_t n){ return fread(ptr,size,n,fp); }
	size_t write(void *ptr,size_t size,size_t n){ return fwrite(ptr,size,n,fp); }

	int Lnum( void )const { return lnum;}
};

#endif
