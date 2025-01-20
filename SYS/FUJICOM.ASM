	ifndef	??version
?debug	macro
	endm
	endif
	?debug	S "fujicom.c"
_TEXT	segment	byte public 'CODE'
DGROUP	group	_DATA,_BSS,_TEXT
	assume cs:DGROUP,ds:DGROUP,ss:DGROUP
_TEXT	ends
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
	?debug	C E9ABA69D590966756A69636F6D2E63
	?debug	C E94505945905636F6D2E68
	?debug	C E900B23B550966756A69636F6D2E68
_BSS	ends
_TEXT	segment	byte public 'CODE'
	?debug	L 10
_fujicom_init	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	?debug	L 12
	mov	si,1016
	mov	di,12
	?debug	L 14
	mov	al,byte ptr [bp+4]
	mov	ah,0
	cmp	ax,1
	je	@4
	cmp	ax,2
	je	@5
	jmp	short @3
@3:
@4:
	?debug	L 18
	mov	si,1016
	?debug	L 19
	mov	di,12
	?debug	L 20
	jmp	short @2
@5:
	?debug	L 22
	mov	si,760
	?debug	L 23
	mov	di,11
	?debug	L 24
	jmp	short @2
@2:
	?debug	L 27
	push	di
	push	si
	call	near ptr _port_open
	pop	cx
	pop	cx
	mov	word ptr DGROUP:_port,ax
	?debug	L 28
	mov	ax,1
	push	ax
	mov	ax,8
	push	ax
	mov	al,78
	push	ax
	xor	dx,dx
	mov	ax,1200
	push	dx
	push	ax
	push	word ptr DGROUP:_port
	call	near ptr _port_set
	add	sp,12
@1:
	?debug	L 29
	pop	di
	pop	si
	pop	bp
	ret	
_fujicom_init	endp
	?debug	L 31
_fujicom_cksum	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	?debug	L 33
	mov	word ptr [bp-2],0
	?debug	L 34
	xor	si,si
	?debug	L 36
	xor	si,si
	jmp	short @10
@9:
	?debug	L 37
	mov	bx,word ptr [bp+4]
	mov	al,byte ptr [bx+si]
	mov	ah,0
	add	ax,word ptr [bp-2]
	mov	cl,8
	shr	ax,cl
	mov	bx,word ptr [bp+4]
	mov	dl,byte ptr [bx+si]
	mov	dh,0
	add	dx,word ptr [bp-2]
	and	dx,255
	add	ax,dx
	mov	word ptr [bp-2],ax
@8:
	inc	si
@10:
	cmp	si,word ptr [bp+6]
	jl	@9
@7:
	?debug	L 39
	mov	al,byte ptr [bp-2]
	jmp	short @6
@6:
	?debug	L 40
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_fujicom_cksum	endp
	?debug	L 48
__fujicom_send_command	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	?debug	L 50
	mov	word ptr [bp-2],-1
	?debug	L 51
	mov	si,word ptr [bp+4]
	?debug	L 54
	mov	ax,4
	push	ax
	push	si
	call	near ptr _fujicom_cksum
	pop	cx
	pop	cx
	mov	bx,word ptr [bp+4]
	mov	byte ptr [bx+4],al
	?debug	L 57
	mov	al,1
	push	ax
	push	word ptr DGROUP:_port
	call	near ptr _port_set_dtr
	pop	cx
	pop	cx
	?debug	L 60
	mov	ax,5
	push	ax
	push	si
	push	word ptr DGROUP:_port
	call	near ptr _port_put
	add	sp,6
	?debug	L 63
	mov	al,0
	push	ax
	push	word ptr DGROUP:_port
	call	near ptr _port_set_dtr
	pop	cx
	pop	cx
	?debug	L 65
	push	word ptr DGROUP:_port
	call	near ptr _port_getc_sync
	pop	cx
	mov	word ptr [bp-2],ax
	?debug	L 67
	mov	al,byte ptr [bp-2]
	jmp	short @11
@11:
	?debug	L 68
	pop	si
	mov	sp,bp
	pop	bp
	ret	
__fujicom_send_command	endp
	?debug	L 70
_fujicom_command	proc	near
	push	bp
	mov	bp,sp
	?debug	L 72
	push	word ptr [bp+4]
	call	near ptr __fujicom_send_command
	pop	cx
	?debug	L 74
	push	word ptr DGROUP:_port
	call	near ptr _port_getc_sync
	pop	cx
	jmp	short @12
@12:
	?debug	L 75
	pop	bp
	ret	
_fujicom_command	endp
	?debug	L 77
_fujicom_command_read	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	?debug	L 82
	push	word ptr [bp+4]
	call	near ptr __fujicom_send_command
	pop	cx
	cbw	
	mov	word ptr [bp-2],ax
	?debug	L 84
	cmp	word ptr [bp-2],78
	jne	@14
	?debug	L 85
	mov	al,byte ptr [bp-2]
	jmp	short @13
@14:
	?debug	L 89
	push	word ptr DGROUP:_port
	call	near ptr _port_getc_sync
	pop	cx
	mov	word ptr [bp-2],ax
	?debug	L 91
	cmp	word ptr [bp-2],67
	jne	@15
	?debug	L 95
	xor	si,si
	jmp	short @19
@18:
	?debug	L 97
	push	word ptr DGROUP:_port
	call	near ptr _port_getc_sync
	pop	cx
	mov	bx,word ptr [bp+6]
	mov	byte ptr [bx+si],al
@17:
	inc	si
@19:
	cmp	si,word ptr [bp+8]
	jl	@18
@16:
	?debug	L 101
	push	word ptr DGROUP:_port
	call	near ptr _port_getc_sync
	pop	cx
@15:
	?debug	L 105
	mov	al,byte ptr [bp-2]
	jmp	short @13
@13:
	?debug	L 106
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_fujicom_command_read	endp
	?debug	L 108
_fujicom_command_write	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	L 114
	push	word ptr [bp+4]
	call	near ptr __fujicom_send_command
	pop	cx
	mov	byte ptr [bp-2],al
	?debug	L 116
	cmp	byte ptr [bp-2],78
	jne	@21
	?debug	L 117
	mov	al,byte ptr [bp-2]
	jmp	short @20
@21:
	?debug	L 120
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr DGROUP:_port
	call	near ptr _port_put
	add	sp,6
	?debug	L 123
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _fujicom_cksum
	pop	cx
	pop	cx
	mov	byte ptr [bp-1],al
	?debug	L 124
	mov	ax,1
	push	ax
	lea	ax,word ptr [bp-1]
	push	ax
	push	word ptr DGROUP:_port
	call	near ptr _port_put
	add	sp,6
	?debug	L 127
	push	word ptr DGROUP:_port
	call	near ptr _port_getc_sync
	pop	cx
	mov	byte ptr [bp-2],al
	?debug	L 129
	cmp	byte ptr [bp-2],78
	jne	@22
	?debug	L 130
	mov	al,byte ptr [bp-2]
	jmp	short @20
@22:
	?debug	L 133
	push	word ptr DGROUP:_port
	call	near ptr _port_getc_sync
	pop	cx
	jmp	short @20
@20:
	?debug	L 134
	mov	sp,bp
	pop	bp
	ret	
_fujicom_command_write	endp
	?debug	L 136
_fujicom_done	proc	near
	?debug	L 138
	push	word ptr DGROUP:_port
	call	near ptr _port_close
	pop	cx
@23:
	?debug	L 139
	ret	
_fujicom_done	endp
_TEXT	ends
_BSS	segment word public 'BSS'
_port	label	word
	db	2 dup (?)
_BSS	ends
	?debug	C E9
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
_TEXT	segment	byte public 'CODE'
	extrn	_port_getc_sync:near
	extrn	_port_set_dtr:near
	extrn	_port_open:near
	extrn	_port_close:near
	extrn	_port_put:near
	extrn	_port_set:near
_TEXT	ends
	public	_fujicom_command
	public	_fujicom_cksum
	public	_fujicom_init
	public	_fujicom_done
	public	_port
	public	_fujicom_command_write
	public	__fujicom_send_command
	public	_fujicom_command_read
	end
