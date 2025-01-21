	ifndef	??version
?debug	macro
	endm
	endif
	?debug	S "video.c"
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
	?debug	C E9E6B81F5507766964656F2E63
	?debug	C E92010AB12115C74635C696E636C7564655C646F732E68
_BSS	ends
_TEXT	segment	byte public 'CODE'
	?debug	L 3
_Active_page	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	L 7
	mov	ah,15
	?debug	L 8
	int	16
	?debug	L 9
	mov	byte ptr [bp-1],bh
	?debug	L 10
	mov	al,byte ptr [bp-1]
	mov	ah,0
	jmp	short @1
@1:
	?debug	L 11
	mov	sp,bp
	pop	bp
	ret	
_Active_page	endp
	?debug	L 13
_Goto_XY	proc	near
	push	bp
	mov	bp,sp
	?debug	L 15
	call	near ptr _Active_page
	mov	bh,al
	?debug	L 16
	dec	word ptr [bp+6]
	mov	dh,byte ptr [bp+6]
	?debug	L 17
	dec	word ptr [bp+4]
	mov	dl,byte ptr [bp+4]
	?debug	L 18
	mov	ah,2
	?debug	L 19
	int	16
@2:
	?debug	L 20
	pop	bp
	ret	
_Goto_XY	endp
	?debug	L 22
_Get_char	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	L 26
	call	near ptr _Active_page
	mov	bh,al
	?debug	L 27
	mov	ah,8
	?debug	L 28
	int	16
	?debug	L 29
	mov	byte ptr [bp-1],al
	?debug	L 30
	mov	al,byte ptr [bp-1]
	jmp	short @3
@3:
	?debug	L 31
	mov	sp,bp
	pop	bp
	ret	
_Get_char	endp
	?debug	L 33
_Get_key	proc	near
	push	bp
	mov	bp,sp
	push	si
	?debug	L 37
	mov	ah,byte ptr [bp+4]
	?debug	L 38
	int	22
	?debug	L 39
	mov	si,ax
	?debug	L 40
	mov	ax,si
	jmp	short @4
@4:
	?debug	L 41
	pop	si
	pop	bp
	ret	
_Get_key	endp
	?debug	L 43
_Get_attr	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	L 47
	call	near ptr _Active_page
	mov	bh,al
	?debug	L 48
	mov	ah,8
	?debug	L 49
	int	16
	?debug	L 50
	mov	byte ptr [bp-1],ah
	?debug	L 51
	mov	al,byte ptr [bp-1]
	jmp	short @5
@5:
	?debug	L 52
	mov	sp,bp
	pop	bp
	ret	
_Get_attr	endp
	?debug	L 54
_Get_X	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	L 58
	call	near ptr _Active_page
	mov	bh,al
	?debug	L 59
	mov	ah,3
	?debug	L 61
	int	16
	?debug	L 63
	mov	byte ptr [bp-1],dl
	?debug	L 65
	inc	byte ptr [bp-1]
	mov	al,byte ptr [bp-1]
	mov	ah,0
	jmp	short @6
@6:
	?debug	L 66
	mov	sp,bp
	pop	bp
	ret	
_Get_X	endp
	?debug	L 68
_Get_Y	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	L 72
	call	near ptr _Active_page
	mov	bh,al
	?debug	L 73
	mov	ah,3
	?debug	L 75
	int	16
	?debug	L 77
	mov	byte ptr [bp-1],dh
	?debug	L 79
	inc	byte ptr [bp-1]
	mov	al,byte ptr [bp-1]
	mov	ah,0
	jmp	short @7
@7:
	?debug	L 80
	mov	sp,bp
	pop	bp
	ret	
_Get_Y	endp
	?debug	L 82
_Get_mode	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	L 86
	mov	ah,15
	?debug	L 88
	int	16
	?debug	L 90
	mov	byte ptr [bp-1],al
	?debug	L 92
	mov	al,byte ptr [bp-1]
	jmp	short @8
@8:
	?debug	L 93
	mov	sp,bp
	pop	bp
	ret	
_Get_mode	endp
	?debug	L 95
_Set_mode	proc	near
	push	bp
	mov	bp,sp
	?debug	L 97
	mov	al,byte ptr [bp+4]
	?debug	L 98
	mov	ah,0
	?debug	L 99
	int	16
@9:
	?debug	L 100
	pop	bp
	ret	
_Set_mode	endp
	?debug	L 102
_Clear_screen	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	L 106
	call	near ptr _Get_mode
	mov	byte ptr [bp-1],al
	?debug	L 108
	mov	bh,0
	?debug	L 109
	xor	cx,cx
	?debug	L 110
	mov	dx,6223
	?debug	L 111
	mov	ax,1536
	?debug	L 113
	int	16
	?debug	L 115
	push	word ptr [bp-1]
	call	near ptr _Set_mode
	pop	cx
	?debug	L 117
	mov	ax,1
	push	ax
	mov	ax,1
	push	ax
	call	near ptr _Goto_XY
	pop	cx
	pop	cx
@10:
	?debug	L 118
	mov	sp,bp
	pop	bp
	ret	
_Clear_screen	endp
	?debug	L 120
_Write_chr	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	?debug	L 124
	call	near ptr _Get_attr
	mov	byte ptr [bp-1],al
	?debug	L 125
	mov	bl,byte ptr [bp-1]
	?debug	L 126
	mov	al,byte ptr [bp+4]
	?debug	L 127
	mov	ah,14
	?debug	L 128
	int	16
@11:
	?debug	L 129
	mov	sp,bp
	pop	bp
	ret	
_Write_chr	endp
	?debug	L 134
_Write_tty	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	?debug	L 142
	call	near ptr _Get_X
	mov	byte ptr [bp-4],al
	?debug	L 143
	call	near ptr _Get_Y
	mov	byte ptr [bp-3],al
	?debug	L 144
	push	word ptr [bp+4]
	call	near ptr _strlen
	pop	cx
	mov	si,ax
	?debug	L 145
	call	near ptr _Active_page
	mov	byte ptr [bp-2],al
	?debug	L 146
	call	near ptr _Get_attr
	mov	byte ptr [bp-1],al
	?debug	L 148
	mov	ax,es
	mov	word ptr DGROUP:es_static,ax
	?debug	L 149
	mov	word ptr DGROUP:bp_static,bp
	?debug	L 151
	mov	cx,si
	?debug	L 152
	dec	byte ptr [bp-3]
	mov	dh,byte ptr [bp-3]
	?debug	L 153
	dec	byte ptr [bp-4]
	mov	dl,byte ptr [bp-4]
	?debug	L 154
	mov	bh,byte ptr [bp-2]
	?debug	L 155
	mov	bl,byte ptr [bp-1]
	?debug	L 156
	push	ds
	pop	es
	?debug	L 157
	mov	bp,word ptr [bp+4]
	?debug	L 158
	mov	ax,4865
	?debug	L 160
	int	16
	?debug	L 162
	mov	bp,word ptr DGROUP:bp_static
	?debug	L 163
	mov	es,word ptr DGROUP:es_static
@12:
	?debug	L 164
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_Write_tty	endp
_TEXT	ends
_BSS	segment word public 'BSS'
bp_static	label	word
	db	2 dup (?)
es_static	label	word
	db	2 dup (?)
_BSS	ends
	?debug	C E9
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
_TEXT	segment	byte public 'CODE'
	extrn	_strlen:near
_TEXT	ends
	public	_Clear_screen
	public	_Write_tty
	public	_Write_chr
	public	_Active_page
_es_static	equ	es_static
	public	_Get_attr
	public	_Get_char
_bp_static	equ	bp_static
	public	_Get_key
	public	_Set_mode
	public	_Get_mode
	public	_Goto_XY
	public	_Get_Y
	public	_Get_X
	end
