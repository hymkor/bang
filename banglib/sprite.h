#ifndef SPRITE_H
#define SPRITE_H

/* Pattern ... パターンを保存している場所を差すポインタの型
 *	「char far* 」と何回も書くのが面倒だから、typedefしているだけ
 */
typedef char far *Pattern;				

/* NOPATTERN ... パターンを表示していないという為のフラグ代わりの定数。
 *	あまり、大した使われ方はない。
 */
#define	NOPATTERN		(Pattern)0


/* sprite_putat ... SPR 形式のキャラクターを表示する。
 * この時、帰り値を保存しておくこと!
 * キャラクターを消すとき、この値を、sprite_putoffの第一引数に渡してやれば、
 * スクロールなどをして座標とアドレスの対応が変わってしまっていても、確実に
 * 表示したキャラクターを消すことができる。
 * つまり、帰り値の内容は、表示アドレスのオフセットだったりするわけだ。
 */
short	sprite_putat( unsigned short X , unsigned short Y , Pattern pattern );

/* sprite_putoff ... 表示しているキャラクターを消す。ﾊｰﾄﾞｳｪｱｽｸﾛｰﾙしていてもOK!	*/
void	sprite_putoff( short vramptr , Pattern pattern );

/* sprite_load ... *.SPR 形式のキャラクターをロードする。
 *	この関数および、拙作のパターンエディタ「PE」は、1-キャラクター:1-ファイル
 *	となっていて、やや効率が悪い。ちなみに、「PE」には可逆セーブと不可逆セーブ
 *	があり、sprite_loadで読めるのは後者の方である。前者は複数一度にセーブし、
 *	「PE」でまたロードできるが、後者は一度に一つ、しかも「PE」ではロードできな
 *	い。
 */
Pattern sprite_load(const char *fname )


/* background ... 背景の保存領域へのfarポインタ。
 *	基本的に、移動キャラクターが存在しない、VRAMのコピーと考えてよい。
 *	つまり、(0,0)のアドレスは、&background[ vram_offset ]となるわけである。
 *	VRAMと同じリング構造なので、(x,y)のは、
 *		background[ (vram_offset + x + y*320) & 0xFFFF ]
 *	となる。だが、x,y を unsigned short にとってやれば、0xFFFF もいらなくなる
 *	はずである。
 *	
 *	なお、farポインタとしての background は、オフセットがちょうど 0000 になる
 *	ように調整してあるので、VRAM を差すポインタと BG-RAM を差すポインタは、
 *	同じオフセットを使用することができる。(インラインアセンブラの時でないと、
 *	あまり、メリットはないが)
 */
extern char far *background;


/* background_init ... background を用意(farmalloc)して、初期化する。
 *	sprite_offを実行する前に必ず呼んでおくこと!
 */
int background_init(void);


extern unsigned short vram_offset;	/* video.h 参照 */

#endif SPRITE_H
