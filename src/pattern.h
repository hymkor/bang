#ifndef PATTERN_H
#define PATTERN_H

extern Pattern 
	hibang0,hibang1,hibang2,minbang,
	houdai0,houdai1,houdai2,houdai3,
	fighter,bullet,bomb,
	psycho0,psycho1,psycho2,bigbang,
	
	bang_pattern[3],
	pika_pattern[3],
	hoki_pattern[4],
	tenta_pattern[2],
	logo_pattern[8],
	exclamation ;

const char *SpritePatternLoad( void );

#endif
