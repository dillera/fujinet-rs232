	name	DOS_hdr
_DATA	segment word public 'DATA'
_d0 label		byte
_DATA	ends
_BSS	segment word public 'BSS'
_b0 label		byte
_BSS	ends
_TEXT	segment byte public 'CODE'
DGROUP	group	_DATA,_BSS,_TEXT
	assume	cs:DGROUP,ds:DGROUP,ss:DGROUP
_TEXT	ends
	end
