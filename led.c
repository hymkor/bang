#pragma inline

#define	KEYCMD	0x60 /* キーボードコントローラの入出力バッファポート */
#define KEYSTT	0x64 /* キーボードコントローラのステータスポート	 */
#define	ACK		0xFA /* キーボードの応答コード						 */
#define	LEDSET	0xED /* キーボードのLEDセットリセットコマンド		 */

void LED(unsigned int patn)
{
	static unsigned int lastpatn = -1;
	if( lastpatn != patn ){
		lastpatn = patn ;

		asm cli
	/* LEDコマンドを送る */
		asm	mov	al,LEDSET
		asm out KEYCMD,al
	lop1:
		asm in	al,KEYCMD
		asm cmp al,ACK
		asm jne	lop1
	
	/* LEDデータを送る */
		asm mov al,byte ptr patn
		asm and	al,7
		asm out	KEYCMD,al
	led2:
		asm in	al,KEYCMD
		asm cmp	al,ACK
		asm jne	led2
		asm sti
	
	/* 無い事とは思うが、バッファがフルなら待つ */
	led3:
		asm	in	al,KEYSTT
		asm and	al,2
		asm jne	led3
	}
}
