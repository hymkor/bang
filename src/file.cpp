#include <ctype.h>
#include <stdarg.h>

#include "file.h"

size_t File::printf( const char *format ,... )
{
	va_list varptr;
	size_t	result;
	
	++lnum;
	va_start( varptr , format );
		result = vfprintf( fp , format , varptr );
	va_end( varptr );
	
	return result;
}

void fncat(char *dp,const char *dir,const char *fname)
{
	int countor=0;
	while( *dir ){
		*dp++ = *dir++;
		countor++;
	}
		
	if( countor > 0   &&   dp[-1] != '\\'   &&   dp[-1] != ':'  )
		*dp++ = '\\';
	
	int i=0;
	while( *fname && !isspace(*fname) && i++ < 12)
		*dp++ = *fname++;
		
	*dp   = '\0';
}
