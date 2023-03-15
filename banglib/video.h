#ifndef VIDEO_H
#define	VIDEO_H


/* ビデオモード変更/参照
 *	13h ... 320x200x256
 *	 3h ... Text Mode   (80x25固定)
 *  70h ... V-Text Mode (桁・行数可変)
 * DOSに戻るときは、呼び出される以前のビデオモードに戻すのが望ましい。
 */
void set_video_mode(unsigned mode);
unsigned get_video_mode( void );




/* スクロール関係の関数群
 *	13hの時は、単に「add_vram_offset_13h」を使用すればよい。
 *	スクロールしたとき、(0,0)のドットは、
 *		segment ... A000h
 *		offset  ... vram_offset (unsigned short型)
 *	になる。AFFFFhとA0000hがつながっているのは言うまでもない。
 *
 *     (以下、蛇足)
 * set_start_address ......... (0,0)の位置を、バイト単位でずらす。
 * set_horizontal_planning ...      〃        ビット単位でずらす。
 *	後者は、一見 13h には不要に見えるが、13h という画面モードは本来、
 *	320x200x4色というプレーンを4枚組み合わせた構造になっているので、
 *	１ドットずらすのに実はこの関数で ２ビットずつずらしてやる必要がある。
 *	CPUがVRAMをアクセスするときは、VGA-CHIPが間に入って１ドット＝１バイトの
 *	１プレーンであるかのように見せているのである。
 *	add_vram_offset_13hを使う分には、その辺のややこしい事は考えなくてもよい。
 */


extern unsigned short vram_offset=0;

void set_start_address(unsigned short address)
void set_horizontal_planning( unsigned short npixel )

void set_crtc_register(unsigned short address,unsigned short data)
int get_crtc_register(unsigned short address)

void add_vram_offset_13h( signed int delta )
/* delta : 横スクロールするドット数。縦の時は、(320×ライン数)でよいはず */



#define	vsync()			(inportb(0x3DA) & 8)
	/* 帰線期間中なら not 0 になる */
#define vsync_wait()	do{ while( vsync() ); while( !vsync() ); }while(0)
	/* vsync_wait()実行後は、帰線期間のはず */


#endif
