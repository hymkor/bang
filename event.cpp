#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hayos.h"
#include "game.h"
#include "file.h"

#define DEM " \n\t\r"

HeapMan Event::heapman( sizeof(Event)  );

char *Stage::errmsg[]={
	"�������܂���.",
	"�V�i���I�t�@�C�����I�[�v���ł��܂���.",
	"EOF�������Ȃ肠����܂���.",
	"�}�b�v�t�@�C�����I�[�v���ł��܂���.",
	"���@�ԈႢ����.",
	"�q�[�v������܂���.",
	"���[�h�r���ŃG���[���N����܂���.",
};

int Stage::load(const char *fname)
{
	char buffer[256],*token;

	File file(fname,"r");
	if( !file ) return CANT_OPEN;
	

	/**** �}�b�v�t�@�C�����̓��� ****/

	do{
		if( file.gets( buffer , sizeof(buffer) )==NULL )
			return BAD_EOF;
		token=strtok(buffer,DEM);
	}while( token==NULL  ||  token[0] == '#' );
	
	if( Map::load( token ) )
		return CANT_MAP_OPEN;
	
	
	/**** �C�x���g�̓ǂݍ��� ****/
	
	Event **ptrptr = &event ;
	Event  *ptr    =  event = new Event;
	int nevent=0;
	for(;;){
		/*** �q�[�v�m�ێ��s�̃`�F�b�N ***/
		if( ptr == NULL )
			return HEAP_ERR;
		
		/*** ��s�̃X�L�b�v�Ɖ��ʒu�̓��� ***/
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
		
		/*** �c�ʒu�̓��� ***/
		
		token=strtok(NULL,DEM);
		if( token==NULL )
			return SYNTAX_ERROR;
		ptr-> y  = atoi( token );
		
		/*** �^�C�v�̓��� ***/
		
		token=strtok(NULL,DEM);
		if( token==NULL )
			return SYNTAX_ERROR;
		ptr -> type = atoi( token );
		
		nevent++;
		
		/*** ���̑��̏��̓��� ***/
		
		for(int i=0  ;  i < numof(ptr->info)  ;  i++ ){
			/* �r���ō����Ȃ��Ȃ�����,#���łĂ�����,'0'�Ŗ��߂� */
			if(  (token=strtok(NULL,DEM)) == NULL  ||  token[0]=='#' ){
				while( i < numof(ptr->info) )
					ptr -> info[ i++ ] = atoi( token );
				break;
			}
			ptr -> info[ i ] = atoi( token );
		}
		
		/*** �q�[�v�̊m��(���s�̃`�F�b�N�̓��[�v�̐擪�ōs��) ***/
		ptrptr = &ptr->next ;
		ptr    =  ptr->next = new Event ;

	}/* end of for(;;) */
}

