TARGET  = dskread.exe
AS      = wasm
ASFLAGS = -0 -mt -bt=DOS
CC      = wcc
CFLAGS  = -0 -bt=dos -I.. -osh -zu
LD	= wlink
LDFLAGS = system dos

CFILES  = dskread.c fujicom.c com.c
OBJS = $(CFILES:.c=.obj) $(AFILES:.asm=.obj)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) name $(TARGET) file {$(OBJS)}

.c.obj: .AUTODEPEND
        $(CC) $(CFLAGS) -fo=$@ $<
.asm.obj: .AUTODEPEND
	$(AS) $(ASFLAGS) -fo=$@ $<

clean : .SYMBOLIC
	rm -f $(TARGET) *.obj *.map *.err

$(TARGET):
