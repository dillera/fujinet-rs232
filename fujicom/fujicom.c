/**
 * #FUJINET Low Level Routines
 */

#include "fujicom.h"
#include "com.h"
#include "../sys/print.h" // debug

#include <env.h>
#include <stdlib.h>

#define TIMEOUT         100
#define MAX_RETRIES	5
#ifndef SERIAL_BPS
#define SERIAL_BPS      9600
#endif /* SERIAL_BPS */

PORT fn_port;
PORT far *port;

void fujicom_init(void)
{
  int base, irq;
  long bps = SERIAL_BPS;
  int comp = 1;


  if (getenv("FUJI_PORT"))
    comp = atoi(getenv("FUJI_PORT"));
  if (getenv("FUJI_BPS"))
    bps = atol(getenv("FUJI_BPS"));
  consolef("Port: %i  BPS: %li\n", comp, (int32_t) bps);

  switch (comp) {
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
  port_set(port, bps, 'N', 8, 1);
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
char _fujicom_send_command(cmdFrame_t far *cmd)
{
  uint8_t *cc = (uint8_t *) cmd;


  /* Calculate checksum and place in frame */
  cmd->cksum = fujicom_cksum(cc, 4);

  /* Assert DTR to indicate start of command frame */
  port_set_dtr(port, 1);

  /* Write command frame */
  port_putbuf(port, cc, sizeof(cmdFrame_t));

  port_wait_for_tx_empty(port);
  /* Desert DTR to indicate end of command frame */
  port_set_dtr(port, 0);
  return port_getc_nobuf(port, TIMEOUT);
}

char fujicom_command(cmdFrame_t far *cmd)
{
  int reply;


  //port_disable_interrupts(port);
  _fujicom_send_command(cmd);
  reply = port_getc_nobuf(port, TIMEOUT);
  //port_enable_interrupts(port);
  return reply;
}

char fujicom_command_read(cmdFrame_t far *cmd, uint8_t far *buf, uint16_t len)
{
  int reply;
  uint16_t rlen;
  int retries;
  uint8_t ck1, ck2;

  //port_disable_interrupts(port);

  for (retries = 0; retries < MAX_RETRIES; retries++) {
    if (retries)
      consolef("FN retry: %i\n", retries);

    reply = _fujicom_send_command(cmd);
    if (reply == 'N')
      break;

    if (reply != 'A') {
      consolef("FN send command bad: 0x%02x\n", reply);
      continue;
    }

    /* Get COMPLETE/ERROR */
    reply = port_getc_nobuf(port, 15 * 1000);
    if (reply != 'C') {
      consolef("FN complete fail: 0x%02x\n", reply);
      continue;
    }

    /* Complete, get payload */
    rlen = port_getbuf(port, buf, len, TIMEOUT);
    if (rlen != len) {
      consolef("FN Read failed: Exp:%i Got:%i  Cmd: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
	       len, rlen,
	       cmd->device, cmd->comnd, cmd->aux1, cmd->aux2, cmd->cksum);
      reply = 'E';
      continue;
    }

    /* Get Checksum byte, verify it. */
    ck1 = port_getc_nobuf(port, TIMEOUT);
    ck2 = fujicom_cksum(buf,len);

    if (ck1 != ck2) {
      consolef("FN checksum error\n");
      reply = 'E';
      continue;
    }

    /* No errors, we're done! */
    break;
  }

  //port_enable_interrupts(port);
  return reply;
}

char fujicom_command_write(cmdFrame_t far *cmd, uint8_t far *buf, uint16_t len)
{
  int reply;
  uint8_t ck;
  int retries = MAX_RETRIES;

  //port_disable_interrupts(port);

  while (retries--) {
    reply = _fujicom_send_command(cmd);
    if (reply == 'N')
      break;

    if (reply != 'A')
      continue;

    /* Write the payload */
    port_putbuf(port, buf, len);

    /* Write the checksum */
    ck = fujicom_cksum(buf, len);
    port_putc_nobuf(port, ck);

    /* Wait for ACK/NACK */
    reply = port_getc_nobuf(port, TIMEOUT);
    if (reply != 'A')
      continue;

    /* Wait for COMPLETE/ERROR */
    reply = port_getc_nobuf(port, TIMEOUT);
    if (reply != 'C')
      continue;

    break;
  }

  //port_enable_interrupts(port);
  return reply;
}

void fujicom_done(void)
{
  port_close(port);
  return;
}
