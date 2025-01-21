	ifndef	??version
?debug	macro
	endm
	endif
	?debug	S "com.c"
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
	?debug	C E90AA9215A05636F6D2E63
	?debug	C E92010AB12145C74635C696E636C7564655C7374646C69622E68
	?debug	C E92010AB12115C74635C696E636C7564655C646F732E68
	?debug	C E945059459115C74635C696E636C7564655C636F6D2E68
_BSS	ends
_DATA	segment word public 'DATA'
com	label	word
	dw	0
_DATA	ends
_TEXT	segment	byte public 'CODE'
	?debug	L 41
interrupt_service_routine	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	ds,cs:DGROUP@
@1:
	?debug	L 43
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
interrupt_service_routine	endp
_TEXT	ends
	?debug	C E9
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
_TEXT	segment	byte public 'CODE'
	extrn	DGROUP@:word
_TEXT	ends
_com	equ	com
_interrupt_service_routine	equ	interrupt_service_routine
	end
