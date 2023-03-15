;;;;; キーボード割り込みをフックして、
;;;;; リアルタイムキースキャンを実現するモジュール

;;;;; 一度、keyscan_init を呼んでおけば、
;;;;; 以後、volatile型配列変数である scantable[]が
;;;;; 適宜変化する。
;;;;;	0...押されていない。 not 0(多分 1)...押されている。



	.MODEL	SMALL
	.DATA
	public	_scantable

_scantable db 128 dup (0)

	.CODE
old15h	dd	2 dup (0)

new15h	proc	far
 	pushf	
 	cmp	ah,04fH
	jne	exit
 	
 	push	ax
 	push	es
 	push	bx
 	
 	mov	bx,seg _DATA
 	mov	es,bx
 	mov	bx,ax
 	and	bx,07FH
 	add	bx,offset _scantable
 	rol	al,1
 	and	al,1
 	xor	al,1
 	mov	es:[bx],al
 	
 	pop	bx
 	pop	es
	pop	ax
	
exit: 	popf	
 	jmp	cs:[dword ptr old15h]
 	
new15h	endp
	
int15h	equ	15h*4

;
; atexit関数に登録するルーチン
; 割り込みベクタをもとどおりにする
;

restore	proc	near
	push	es
	push	di
	
	xor	ax,ax
	mov	es,ax
	mov	di,int15h
	
	mov	ax,cs:word ptr old15h[0] ; offset
	mov	es:[di+0],ax
	mov	ax,cs:word ptr old15h[2] ; segment
	mov	es:[di+2],ax
	
	pop	di
	pop	es
	ret
restore	endp

;
; 割り込みベクタを引き込むルーチン
;
	extrn	_atexit:near
	public	_keyscan_init
	
_keyscan_init proc near
	push	es
	push	di
	
	xor	ax,ax
	mov	es,ax
	mov	di,int15h
	
; ベクタを保存
	mov	ax,es:[di+0] ; offset
	mov	cs:word ptr old15h[0],ax
	mov	ax,es:[di+2] ; segment
	mov	cs:word ptr old15h[2],ax
; ベクタを変更
	mov	ax,offset new15h ; offset
	mov	es:word ptr [di+0],ax
	mov	ax,seg new15h ; segment
	mov	es:word ptr [di+2],ax
; atexitに登録
	mov	ax,offset restore
	push	ax
	call	_atexit
	add	sp,2
	
	pop	di
	pop	es
	ret
	
_keyscan_init endp
	end