TARGET  = dskread.exe
AS      = wasm
ASFLAGS = -0 -mt -bt=DOS
CC      = wcc
CFLAGS  = -0 -bt=dos -I../fujicom -osh -zu
LD	= wlink
LDFLAGS = SYSTEM dos LIBPATH ../fujicom

CFILES  = dskread.c
OBJS = $(CFILES:.c=.obj) $(AFILES:.asm=.obj)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) NAME $(TARGET) FILE {$(OBJS)} LIBRARY fujicoms.lib

.c.obj: .AUTODEPEND
        $(CC) $(CFLAGS) -fo=$@ $<
.asm.obj: .AUTODEPEND
	$(AS) $(ASFLAGS) -fo=$@ $<

clean : .SYMBOLIC
	rm -f $(TARGET) *.obj *.map *.err

$(TARGET):
