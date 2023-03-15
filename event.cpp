#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hayos.h"
#include "game.h"
#include "file.h"

#define DEM " \n\t\r"

HeapMan Event::heapman( sizeof(Event)  );

char *Stage::errmsg[]={
	"成功しました.",
	"シナリオファイルをオープンできません.",
	"EOFがいきなりあらわれました.",
	"マップファイルをオープンできません.",
	"文法間違いあり.",
	"ヒープがたりません.",
	"ロード途中でエラーが起こりました.",
};

int Stage::load(const char *fname)
{
	char buffer[256],*token;

	File file(fname,"r");
	if( !file ) return CANT_OPEN;
	

	/**** マップファイル名の入力 ****/

	do{
		if( file.gets( buffer , sizeof(buffer) )==NULL )
			return BAD_EOF;
		token=strtok(buffer,DEM);
	}while( token==NULL  ||  token[0] == '#' );
	
	if( Map::load( token ) )
		return CANT_MAP_OPEN;
	
	
	/**** イベントの読み込み ****/
	
	Event **ptrptr = &event ;
	Event  *ptr    =  event = new Event;
	int nevent=0;
	for(;;){
		/*** ヒープ確保失敗のチェック ***/
		if( ptr == NULL )
			return HEAP_ERR;
		
		/*** 空行のスキップと横位置の入力 ***/
		do{
			if( file.gets( buffer , sizeof(buffer) )==NULL ){
				if( !feof(file) )
					return LOAD_ERROR;
				*ptrptr = NULL;
				delete ptr;
				fprintf(stderr,"Event:%d\n",nevent);
				return SUCCESS;
			}
			token=strtok(buffer,DEM);
		}while( token==NULL  ||  token[0] == '#' );
		
		ptr->pos = atoi( token );
		
		/*** 縦位置の入力 ***/
		
		token=strtok(NULL,DEM);
		if( token==NULL )
			return SYNTAX_ERROR;
		ptr-> y  = atoi( token );
		
		/*** タイプの入力 ***/
		
		token=strtok(NULL,DEM);
		if( token==NULL )
			return SYNTAX_ERROR;
		ptr -> type = atoi( token );
		
		nevent++;
		
		/*** その他の情報の入力 ***/
		
		for(int i=0  ;  i < numof(ptr->info)  ;  i++ ){
			/* 途中で項がなくなったり,#がでてきたら,'0'で埋める */
			if(  (token=strtok(NULL,DEM)) == NULL  ||  token[0]=='#' ){
				while( i < numof(ptr->info) )
					ptr -> info[ i++ ] = atoi( token );
				break;
			}
			ptr -> info[ i ] = atoi( token );
		}
		
		/*** ヒープの確保(失敗のチェックはループの先頭で行う) ***/
		ptrptr = &ptr->next ;
		ptr    =  ptr->next = new Event ;

	}/* end of for(;;) */
}

