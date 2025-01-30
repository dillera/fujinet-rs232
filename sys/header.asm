DGROUP  group   _SYS_HEADER, _TEXT, _INIT

_INIT   segment word public 'INIT'

        public  _end_of_driver
        public  _small_code_

_end_of_driver label near

_small_code_    dw      ?

_INIT   ends

_SYS_HEADER segment word public 'SYS_HEADER'

        org     0

_sys_hdr_ label near

	extrn	Strategy_:near
	extrn	Interrupt_:near

	dd	-1
	dw	2000h
	dw	Strategy_
	dw	Interrupt_
	db	8, 0, 0, 0, 0, 0, 0, 0

_SYS_HEADER ends

        end     _sys_hdr_
