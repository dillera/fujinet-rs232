#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fujicom.h"
#include "com.h"

cmdFrame_t cmd;

unsigned char buf[512];

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

  for (i = 0; i < sizeof(buf); i++) {
    printf("%02x ",buf[i]);
  }

  return 0;
}
