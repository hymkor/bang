;;;;; �L�[�{�[�h���荞�݂��t�b�N���āA
;;;;; ���A���^�C���L�[�X�L�������������郂�W���[��

;;;;; ��x�Akeyscan_init ���Ă�ł����΁A
;;;;; �Ȍ�Avolatile�^�z��ϐ��ł��� scantable[]��
;;;;; �K�X�ω�����B
;;;;;	0...������Ă��Ȃ��B not 0(���� 1)...������Ă���B



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
; atexit�֐��ɓo�^���郋�[�`��
; ���荞�݃x�N�^�����Ƃǂ���ɂ���
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
; ���荞�݃x�N�^���������ރ��[�`��
;
	extrn	_atexit:near
	public	_keyscan_init
	
_keyscan_init proc near
	push	es
	push	di
	
	xor	ax,ax
	mov	es,ax
	mov	di,int15h
	
; �x�N�^��ۑ�
	mov	ax,es:[di+0] ; offset
	mov	cs:word ptr old15h[0],ax
	mov	ax,es:[di+2] ; segment
	mov	cs:word ptr old15h[2],ax
; �x�N�^��ύX
	mov	ax,offset new15h ; offset
	mov	es:word ptr [di+0],ax
	mov	ax,seg new15h ; segment
	mov	es:word ptr [di+2],ax
; atexit�ɓo�^
	mov	ax,offset restore
	push	ax
	call	_atexit
	add	sp,2
	
	pop	di
	pop	es
	ret
	
_keyscan_init endp
	end