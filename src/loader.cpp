#include <alloc.h>
#include <stddef.h>
#include <stdlib.h>
#include "sprite.h"
#include "pattern.h"
#include "bang.h"

#undef	numof
#define numof(A)	( sizeof(A) / sizeof(A[0]) )

Pattern 
	hibang0 , hibang1 , hibang2 , minbang ,
	houdai0 , houdai1 , houdai2 , houdai3 ,
	fighter , bullet , bomb ,
	psycho0 , psycho1 , psycho2 , bigbang ,

	bang_pattern[3] ,
	pika_pattern[3] ,
	hoki_pattern[4] ,
	tenta_pattern[2] ,
	logo_pattern[8] ,
	exclamation ;

static struct{
	const char *filename;
	Pattern *buffer_ptrptr;
} patterns[]={
	/*** �΂񂮂ف[�� ***/
	{ "bang0.spr" , & bang_pattern[0] },	/* �z�[�L�������Ă����� */
	{ "bang1.spr" , & bang_pattern[1] },	/* �z�[�L�������Ă��Ȃ���� */
/*	{ "bang2.spr" , & bang_pattern[2] },	/* ��e�����΂񂮂ف[�� */
	
	/*** �΂񂮂ف[���̂ف[�� ***/
	{ "hoki0.spr" , & hoki_pattern[0] },	/* ���̃p�^�[���́A16�~16�̃T�C�Y��O��ɍ���Ă��� */
	{ "hoki1.spr" , & hoki_pattern[1] },
	{ "hoki2.spr" , & hoki_pattern[2] },
	{ "hoki3.spr" , & hoki_pattern[3] },
	
	/*** ���G���[�h�̂΂񂮂ف[�� ***/
	{ "hibang0.spr" , & hibang0 },
	{ "hibang1.spr" , & hibang1 },
	
	/*** �c�@���\���p�̏����Ȃ΂񂮂ف[��(���C�ɓ���) ***/
	{ "minbang.spr" , & minbang },
	
	/*** �G�� ***/
	{ "tenta0.spr" , & tenta_pattern[0] },
	{ "tenta1.spr" , & tenta_pattern[1] },
	
	/*** �����ݸ�ΰ� ***/
	{ "psycho0.spr" , & psycho0 },
	{ "psycho1.spr" , & psycho1 },	/* ���G */
	{ "psycho2.spr" , & psycho2 },
	{ "bigbang.spr" , & bigbang },	/* ���� */
	
	/*** ���� ***/
	{ "pika0.spr" , & pika_pattern[0] },
	{ "pika1.spr" , & pika_pattern[1] },
	{ "pika2.spr" , & pika_pattern[2] },
	
	/*** �C�� ***/
	{ "houdai0.spr" , & houdai0 },
	{ "houdai1.spr" , & houdai1 },
	{ "houdai2.spr" , & houdai2 },
	{ "houdai3.spr" , & houdai3 },
	
	/*** �y���e ***/
	{ "bomb.spr" , & bomb },
	
	/*** ���ʂ̒e�� ***/
	{ "bullet.spr" , & bullet },
	
	/*** �퓬�@ ***/
	{ "fighter.spr" , & fighter },
	
	/*** �^�C�g�����S ***/
	{ "logo_ba.spr" , & logo_pattern[0] },	/* �� */
	{ "logo_n1.spr" , & logo_pattern[1] },	/* �� */
	{ "logo_gu.spr" , & logo_pattern[2] },	/* �� */
	{ "logo_tsu.spr" , & logo_pattern[3] },	/* �� */
	{ "logo_ex.spr" , & exclamation },		/* �I */
	{ "logo_zo.spr" , & logo_pattern[4] },	/* �� */
	{ "logo_bar.spr" , & logo_pattern[5] },	/* �[ */
	{ "logo_ki.spr" , & logo_pattern[6] },	/* �� */
	{ "logo_n2.spr" , & logo_pattern[7] },	/* �� */
};
static void SpriteBufferFree( void )
{
	for(int i=0 ; i<numof( patterns ) ; i++ ){
		if( *patterns[i].buffer_ptrptr != NOPATTERN ){
			farfree( *patterns[i].buffer_ptrptr );
			*patterns[i].buffer_ptrptr = NOPATTERN;
		}
	}
}

const char *SpritePatternLoad( void )
{
	atexit( SpriteBufferFree );
	
	for(int i=0 ; i<numof( patterns ) ; i++ ){
		if( (*patterns[i].buffer_ptrptr = Sprite::load( patterns[i].filename )) == NOPATTERN ){
			return patterns[i].filename;
		}
	}
	return NULL;
}
