/**
 * #FUJINET Low Level Routines
 */

#include "fujicom.h"
#include "com.h"
#include "../sys/print.h" // debug

#include <env.h>
#include <stdlib.h>

#define TIMEOUT         100
#define SERIAL_BPS      9600

PORT fn_port;
PORT far *port;

void fujicom_init(void)
{
  int base, irq;
  long baud = SERIAL_BPS;
  int p = 1;


  if (getenv("FUJI_PORT"))
    p = atoi(getenv("FUJI_PORT"));
  if (getenv("FUJI_BAUD"))
    baud = atoi(getenv("FUJI_BAUD"));
  switch (p) {
  default:
  case 1:
    base = COM1_UART;
    irq = COM1_INTERRUPT;
    break;
  case 2:
    base = COM2_UART;
    irq = COM2_INTERRUPT;
    break;
  }
  port = port_open(&fn_port, base, irq);
  port_set(port, baud, 'N', 8, 1);
  port_disable_interrupts(port);

  return;
}

uint8_t fujicom_cksum(uint8_t far *buf, uint16_t len)
{
  uint16_t chk = 0;
  int i = 0;


  for (i = 0; i < len; i++)
    chk = ((chk + buf[i]) >> 8) + ((chk + buf[i]) & 0xFF);
  return (uint8_t) chk;
}


/**
 * @brief Internal function, send command, get response.
 *
 * @param c ptr to command frame to send
 * @return 'A'ck, or 'N'ak.
 */
char _fujicom_send_command(cmdFrame_t far *c)
{
  uint8_t *cc = (uint8_t *) c;


  /* Calculate checksum and place in frame */
  c->cksum = fujicom_cksum(cc, 4);

  /* Assert DTR to indicate start of command frame */
  port_set_dtr(port, 1);

  /* Write command frame */
  port_putbuf(port, cc, sizeof(cmdFrame_t));

  port_wait_for_tx_empty(port);
  /* Desert DTR to indicate end of command frame */
  port_set_dtr(port, 0);
  return port_getc_nobuf(port, TIMEOUT);
}

char fujicom_command(cmdFrame_t far *c)
{
  int reply;


  //port_disable_interrupts(port);
  _fujicom_send_command(c);
  reply = port_getc_nobuf(port, TIMEOUT);
  //port_enable_interrupts(port);
  return reply;
}

char fujicom_command_read(cmdFrame_t far *c, uint8_t far *buf, uint16_t len)
{
  int reply;
  uint16_t rlen;


  //port_disable_interrupts(port);
  reply = _fujicom_send_command(c);
  if (reply != 'A')
    goto done;

  /* Get COMPLETE/ERROR */
  reply = port_getc_nobuf(port, 15 * 1000);  
  if (reply == 'C') {
    /* Complete, get payload */
    rlen = port_getbuf(port, buf, len, TIMEOUT);
    if (rlen != len)
      consolef("FN Read failed: Exp:%i Got:%i\n", len, rlen);

    /* Get Checksum byte, we don't use it. */
    port_getc_nobuf(port, TIMEOUT);
    // FIXME - verify checksum and received length
  }
  else
    consolef("FN Read bogus reply: 0x%02x\n", reply);

 done:
  //port_enable_interrupts(port);
  return reply;
}

char fujicom_command_write(cmdFrame_t far *c, uint8_t far *buf, uint16_t len)
{
  int reply;
  uint8_t ck;


  //port_disable_interrupts(port);
  reply = _fujicom_send_command(c);
  if (reply != 'A')
    goto done;

  /* Write the payload */
  port_putbuf(port, buf, len);

  /* Write the checksum */
  ck = fujicom_cksum(buf, len);
  port_putc_nobuf(port, ck);

  /* Wait for ACK/NACK */
  reply = port_getc_nobuf(port, TIMEOUT);
  if (reply != 'A')
    goto done;

  /* Wait for COMPLETE/ERROR */
  reply = port_getc_nobuf(port, TIMEOUT);

 done:
  //port_enable_interrupts(port);
  return reply;
}

void fujicom_done(void)
{
  port_close(port);
  return;
}
