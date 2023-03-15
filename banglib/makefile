test.exe : test.obj video.obj sprite.obj

.c.obj :
	tcc -c $<

test.obj	: test.c
sprite.obj	: sprite.c sprite.h
video.obj	: video.c video.h
mice.obj	: mice.c mice.h
