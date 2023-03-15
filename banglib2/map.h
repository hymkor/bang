#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

/*** ここでcell(セル)とは、8dots×8lines の背景ブロックを差す。
 *** データ構造は、「PE」の「SAVE-ALL」によってセーブされたデータ形式を
 *** そのまま利用し、256個のセルを一括して扱うことになる。
 *** ( X1のP.C.G.みたいなもののつもり )
 ***
 *** 以下のルーチンでは、256個を越える数のセルを一度に利用することができない。
 *** 困った。
 ***/


/* セルが格納されるバッファへのポインタ。バッファは固定されていて、
 * 複数持つことができない。困ったもんだ。
 */
extern char far *cell_graphics;


/* SAVE-ALLによってセーブされたセルをロードする。*/
int  cell_load(const char *fname);


/* 指定された座標に type 番目のセルを表示する。
 * セルの番号は、PEの画面でいうと、
 *		０ １ ２ ..... 15
 *		16 17 18 ..... 31
 *		 :             :
 *		240 ......... 255
 *	てな具合に並んでいたはず。確か。
 *
 */
void cell_putat(unsigned x,unsigned y,unsigned type);

#endif
