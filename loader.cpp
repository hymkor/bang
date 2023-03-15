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
	/*** ばんぐほーき ***/
	{ "bang0.spr" , & bang_pattern[0] },	/* ホーキを持っている状態 */
	{ "bang1.spr" , & bang_pattern[1] },	/* ホーキを持っていない状態 */
/*	{ "bang2.spr" , & bang_pattern[2] },	/* 被弾したばんぐほーき */
	
	/*** ばんぐほーきのほーき ***/
	{ "hoki0.spr" , & hoki_pattern[0] },	/* このパターンは、16×16のサイズを前提に作られている */
	{ "hoki1.spr" , & hoki_pattern[1] },
	{ "hoki2.spr" , & hoki_pattern[2] },
	{ "hoki3.spr" , & hoki_pattern[3] },
	
	/*** 無敵モードのばんぐほーき ***/
	{ "hibang0.spr" , & hibang0 },
	{ "hibang1.spr" , & hibang1 },
	
	/*** 残機数表示用の小さなばんぐほーき(お気に入り) ***/
	{ "minbang.spr" , & minbang },
	
	/*** 触手 ***/
	{ "tenta0.spr" , & tenta_pattern[0] },
	{ "tenta1.spr" , & tenta_pattern[1] },
	
	/*** ｻｲｺﾊﾞﾝｸﾞﾎｰｷ ***/
	{ "psycho0.spr" , & psycho0 },
	{ "psycho1.spr" , & psycho1 },	/* 無敵 */
	{ "psycho2.spr" , & psycho2 },
	{ "bigbang.spr" , & bigbang },	/* 爆発 */
	
	/*** 爆発 ***/
	{ "pika0.spr" , & pika_pattern[0] },
	{ "pika1.spr" , & pika_pattern[1] },
	{ "pika2.spr" , & pika_pattern[2] },
	
	/*** 砲台 ***/
	{ "houdai0.spr" , & houdai0 },
	{ "houdai1.spr" , & houdai1 },
	{ "houdai2.spr" , & houdai2 },
	{ "houdai3.spr" , & houdai3 },
	
	/*** 炸裂弾 ***/
	{ "bomb.spr" , & bomb },
	
	/*** 普通の弾丸 ***/
	{ "bullet.spr" , & bullet },
	
	/*** 戦闘機 ***/
	{ "fighter.spr" , & fighter },
	
	/*** タイトルロゴ ***/
	{ "logo_ba.spr" , & logo_pattern[0] },	/* ば */
	{ "logo_n1.spr" , & logo_pattern[1] },	/* ん */
	{ "logo_gu.spr" , & logo_pattern[2] },	/* ぐ */
	{ "logo_tsu.spr" , & logo_pattern[3] },	/* っ */
	{ "logo_ex.spr" , & exclamation },		/* ！ */
	{ "logo_zo.spr" , & logo_pattern[4] },	/* ぞ */
	{ "logo_bar.spr" , & logo_pattern[5] },	/* ー */
	{ "logo_ki.spr" , & logo_pattern[6] },	/* き */
	{ "logo_n2.spr" , & logo_pattern[7] },	/* ん */
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