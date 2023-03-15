#ifndef MICE_H
#define MICE_H

extern int dummy_for_mouse;

int  mouse_init(void);
void mouse_on(void);
void mouse_off(void);

int mouse( int &x=dummy_for_mouse, int &y=dummy_for_mouse );

void mouse_setpos(int x,int y);
void mouse_setplane(int plane);
void mouse_setcursor(int x,int y,int plane,void *pattern);

void mouse_delta(int &x,int &y);

#endif
