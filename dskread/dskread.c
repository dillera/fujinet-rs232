#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fujicom.h"
#include "com.h"

cmdFrame_t cmd;

unsigned char buf[512];

void dumpHex(uint8_t far *buffer, uint16_t count);

int main(int argc, char *argv[])
{
  unsigned int sector = atoi(argv[1]);
  unsigned int i=0;
    
  if (argc<2) {
    printf("DSKREAD <0-sector number>\n");
    exit(1);
  }

  cmd.device = DEVICEID_DISK;  /* Drive 1 */
  cmd.comnd = 'R'; /* Read */
  cmd.aux1 = sector & 0xFF;
  cmd.aux2 = sector >> 8;

  fujicom_init();
  
  printf("fujicom_command_read(%i) for sector %u\n",
	 fujicom_command_read(&cmd, buf, sizeof(buf)), sector);

  dumpHex(buf, sizeof(buf));

  return 0;
}

void dumpHex(uint8_t far *buffer, uint16_t count)
{
  int outer, inner;
  uint8_t c, is_err;


  for (outer = 0; outer < count; outer += 16) {
    printf("%08x  ", outer);
    for (inner = 0; inner < 16; inner++) {
      if (inner + outer < count) {
        c = buffer[inner + outer];
	printf("%02x ", c);
      }
      else
	printf("   ");
    }
    printf(" |");
    for (inner = 0; inner < 16 && inner + outer < count; inner++) {
      c = buffer[inner + outer];
      if (c >= ' ' && c <= 0x7f)
	putchar(c);
      else
	putchar('.');
    }
    printf("|\r\n");
  }

  return;
}
