#ifndef MICE_H
#define MICE_H

int mouse_init(void);
void mouse_on(void);
void mouse_off(void);
int mouse( int *x, int *y );	/* 座標系は、320*200基準。*/

#endif
