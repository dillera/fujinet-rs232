TARGET  = fujinet.sys
AS      = wasm
ASFLAGS = -0 -mt -bt=DOS
CC      = wcc
CFLAGS  = -0 -bt=dos -ms -I../fujicom -s -osh -zu
LD	= wlink
LDFLAGS = &
	SYSTEM dos &
	ORDER &
	  clname SYS_HEADER &
	  clname DATA &
	  clname CODE &
	  clname BSS &
	  clname INIT &
	OPTION MAP &
	LIBPATH ../fujicom

CFILES  = commands.c dispatch.c init.c print.c
AFILES  = header.asm
OBJS = $(CFILES:.c=.obj) $(AFILES:.asm=.obj)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) &
	  disable 1014, &
	  statics &
	  option start=Init_cmd_ &
	  name $@ &
	  file {$(OBJS)} &
	  library {fujicoms.lib clibs.lib}

init.obj: init.c .AUTODEPEND
	$(CC) $(CFLAGS) -nt=_INIT -nc=INIT -fo=$@ $<

.c.obj: .AUTODEPEND
        $(CC) $(CFLAGS) -fo=$@ $<
.asm.obj: .AUTODEPEND
	$(AS) $(ASFLAGS) -fo=$@ $<

clean : .SYMBOLIC
	rm -f $(TARGET) *.obj *.map *.err
