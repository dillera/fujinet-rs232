#include "commands.h"
#include "fujinet.h"
#include "fujicom.h"
#include "com.h"
#include "print.h"
#include "dispatch.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <dos.h>

#ifndef VERSION
#define VERSION "0.8"
#endif

// FIXME - use SIMPLE with year + century, not APETIME
struct _tm {
  char tm_mday;
  char tm_month;
  char tm_year;
  char tm_hour;
  char tm_min;
  char tm_sec;
};

cmdFrame_t cmd;
union REGS regs;
extern void *end_of_driver;

#pragma data_seg("_CODE")

uint8_t get_set_time(uint8_t set_flag);
void check_uart();

uint16_t Init_cmd(SYSREQ far *req)
{
  uint8_t err;


  regs.h.ah = 0x30;
  intdos(&regs, &regs);
  consolef("\nFujiNet driver " VERSION " loaded on MS-DOS %i.%i\n", regs.h.al, regs.h.ah);

  req->req_type.init_req.end_ptr = MK_FP(getCS(), &end_of_driver);

  fujicom_init();
  check_uart();

  // FIXME - check if /NOTIME was passed on command line
  err = get_set_time(1);

  // If get_set_time returned error, FujiNet is probably not connected
  if (err) {
    fujicom_done();
    return ERROR_BIT;
  }

  /* Construct BPB table and pointers */
  {
    int idx;


    req->req_type.init_req.num_of_units = 1; // FN_MAX_DEV;

    for (idx = 0; idx < FN_MAX_DEV; idx++) {
      /* 5.25" 360k BPB */
      fn_bpb_table[idx].bps = 512;
      fn_bpb_table[idx].spau = 2;
      fn_bpb_table[idx].rs = 1;
      fn_bpb_table[idx].num_FATs = 2;
      fn_bpb_table[idx].root_entries = 0x0070;
      fn_bpb_table[idx].num_sectors = 0x02d0;
      fn_bpb_table[idx].media_descriptor = 0xfd;
      fn_bpb_table[idx].spfat = 2;
      fn_bpb_table[idx].spt = 9;
      fn_bpb_table[idx].heads = 2;
      fn_bpb_table[idx].hidden = 0;
      fn_bpb_table[idx].num_of_sectors_32 = 0;

      fn_bpb_pointers[idx] = &fn_bpb_table[idx];
    }

    fn_bpb_pointers[idx] = NULL;
    req->req_type.build_bpb_req.BPB_table = MK_FP(getCS(), fn_bpb_pointers);
  }

  return OP_COMPLETE;
}

/* Returns non-zero on error */
uint8_t get_set_time(uint8_t set_flag)
{
  char reply = 0;
  struct _tm cur_time;
  uint16_t year_wcen;


  cmd.device = DEVICEID_APETIME;
  cmd.comnd = APETIMECMD_GETTZTIME;

  reply = fujicom_command_read(&cmd, (uint8_t *) &cur_time, sizeof(cur_time));

  if (reply != 'C') {
    consolef("Could not read time from FujiNet %i.\nAborted.\n", reply);
    return 1;
  }

  year_wcen = cur_time.tm_year + 2000;
  printDTerm("Current FujiNet date & time: $");
  printDec(cur_time.tm_month, 2, '0');
  printDTerm("/$");
  printDec(cur_time.tm_mday, 2, '0');
  printDTerm("/$");
  printDec(year_wcen, 4, '0');

  printDTerm(" $");

  printDec(cur_time.tm_hour, 2, '0');
  printDTerm(":$");
  printDec(cur_time.tm_min, 2, '0');
  printDTerm(":$");
  printDec(cur_time.tm_sec, 2, '0');

  printDTerm("\r\n$");

  if (set_flag) {
    regs.h.ah = 0x2B;
    regs.x.cx = year_wcen;
    regs.h.dh = cur_time.tm_month;
    regs.h.dl = cur_time.tm_mday;

    intdos(&regs, &regs);

    regs.h.ah = 0x2D;
    regs.h.ch = cur_time.tm_hour;
    regs.h.cl = cur_time.tm_min;
    regs.h.dh = cur_time.tm_sec;
    regs.h.dl = 0;

    intdos(&regs, &regs);

    printDTerm("MS-DOS Time now set from FujiNet\r\n$");
  }

  return 0;
}

void check_uart()
{
  extern PORT far *port; // FIXME - this is in fujicom.c
  int uart;


  uart = port_identify_uart(port);
  switch (uart) {
  case UART_16550A:
    consolef("Serial port is 16550A w/FIFO\n");
    break;

  case UART_16550:
    consolef("Serial port is 16550\n");
    break;

  case UART_16450:
    consolef("Serial port is 16450\n");
    break;

  case UART_8250:
    consolef("Serial port is 8250\n");
    break;

  default:
    consolef("Unknown serial port\n");
    break;
  }

  return;
}
