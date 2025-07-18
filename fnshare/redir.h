#ifndef _REDIR_H
#define _REDIR_H

#include "dosdata.h"
#include <fujifs.h>

#define DOS_INT_REDIR   0x2F
#define REDIRECTOR_FUNC 0x11

extern uint8_t fn_drive_num;
extern fujifs_handle fn_host;
extern char *fn_volume;
extern char fn_cwd[];

typedef void (interrupt far *INTVECT)();
extern INTVECT prev_int2f_vector;

enum {
  SUBF_INQUIRY          = 0x00,
  SUBF_REMOVEDIR        = 0x01,
  SUBF_MAKEDIR          = 0x03,
  SUBF_CHDIR            = 0x05,
  SUBF_CLOSE            = 0x06,
  SUBF_COMMIT           = 0x07,
  SUBF_READ             = 0x08,
  SUBF_WRITE            = 0x09,
  SUBF_LOCK             = 0x0A,
  SUBF_UNLOCK           = 0x0B,
  SUBF_GETDISKSPACE     = 0x0C,
  SUBF_SETATTR          = 0x0E,
  SUBF_GETATTR          = 0x0F,
  SUBF_RENAME           = 0x11,
  SUBF_DELETE           = 0x13,
  SUBF_OPENEXIST        = 0x16,
  SUBF_OPENCREATE       = 0x17,
  SUBF_FINDFIRST        = 0x1B,
  SUBF_FINDNEXT         = 0x1C,
  SUBF_CLOSEALL         = 0x1D,
  SUBF_DOREDIR          = 0x1E,
  SUBF_PRINTERSETUP     = 0x1F,
  SUBF_FLUSHBUFFERS     = 0x20,
  SUBF_SEEK             = 0x21,
  SUBF_PROCTERM         = 0x22,
  SUBF_QUALIFYPATH      = 0x23,
  SUBF_REDIRPRINTER     = 0x25,
  SUBF_EXTENDATTR       = 0x2D,
  SUBF_OPENEXTENDED     = 0x2E,
};

extern DIRREC_PTR dirrec_ptr1;
extern DIRREC_PTR dirrec_ptr2;
//extern SIGREC sigrec;
extern SRCHREC_PTR srchrec_ptr1;
extern SRCHREC_PTR srchrec_ptr2;
extern char far *cds_path_root;
extern char far *current_path;
extern char far *fcbname_ptr1;
extern char far *fcbname_ptr2;
extern char far *filename_ptr1;
extern char far *filename_ptr2;
extern char our_drive_str[];
extern uint16_t cds_root_size;
extern uint8_t far *sda_ptr;
extern uint8_t far *srch_attr_ptr;
extern uint8_t fn_drive_num;

/* The following structure is compiler specific, and maps
        onto the registers pushed onto the stack for an interrupt
        function. */
typedef struct {
#ifdef __BORLANDC__
  uint16_t bp, di, si, ds, es, dx, cx, bx, ax;
#else
#ifdef __WATCOMC__
  uint16_t gs, fs;
#endif /* __WATCOMC__ */
  uint16_t es, ds, di, si, bp, sp, bx, dx, cx, ax;
#endif
  uint16_t ip, cs, flags;
} ALL_REGS;

extern void interrupt far redirector(ALL_REGS entry_regs);

#endif /* _REDIR_H */
