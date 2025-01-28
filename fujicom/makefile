# The 's' is for (S)mall memory model
TARGET  = fujicoms.lib
AS      = wasm
ASFLAGS = -0 -mt -bt=DOS
CC      = wcc
CFLAGS  = -0 -bt=dos -ms -s -osh -zu
LIB     = wlib

CFILES  = fujicom.c com.c
OBJS = $(CFILES:.c=.obj) $(AFILES:.asm=.obj)

$(TARGET): $(OBJS)
	$(LIB) -n $(TARGET) $(OBJS)

.c.obj: .AUTODEPEND
        $(CC) $(CFLAGS) -fo=$@ $<
.asm.obj: .AUTODEPEND
	$(AS) $(ASFLAGS) -fo=$@ $<

clean : .SYMBOLIC
	rm -f $(TARGET) *.obj *.map *.err
