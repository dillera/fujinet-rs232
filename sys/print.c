#include "print.h"

void printHex(uint16_t val, uint16_t width, char leading)
{
  uint16_t digits, tval;
  const char hex_digits[] = "0123456789ABCDEF";


  for (tval = val, digits = 0; tval; tval >>= 4, digits++)
    ;
  if (!digits)
    digits = 1;

  for (; digits < width; width--)
    printChar(leading);

  while (digits) {
    digits--;
    printChar(hex_digits[(val >> 4 * digits) & 0xf]);
  }

  return;
}

void printHex32(uint32_t val, uint16_t width, char leading)
{
  uint16_t digits;
  uint32_t tval;
  const char hex_digits[] = "0123456789ABCDEF";


  for (tval = val, digits = 0; tval; tval >>= 4, digits++)
    ;
  if (!digits)
    digits = 1;

  for (; digits < width; width--)
    printChar(leading);

  while (digits) {
    digits--;
    printChar(hex_digits[(val >> 4 * digits) & 0xf]);
  }

  return;
}

void printDec(uint16_t val, uint16_t width, char leading)
{
  uint16_t digits, tval, tens;


  for (tval = val, digits = 0, tens = 1; tval; tval /= 10, digits++, tens *= 10)
    ;
  if (!digits)
    digits = 1;
  tens /= 10;

  for (; digits < width; width--)
    printChar(leading);

  while (digits) {
    digits--;
    printChar('0' + (val / tens) % 10);
    tens /= 10;
  }

  return;
}

void dumpHex(uint8_t far *buffer, uint16_t count)
{
  int outer, inner;
  uint8_t c, is_err;


  for (outer = 0; outer < count; outer += 16) {
    for (inner = 0; inner < 16; inner++) {
      if (inner + outer < count) {
        c = buffer[inner + outer];
	printHex(c, 2, '0');
	printChar(' ');
      }
      else
	printDTerm("   $");
    }
    printDTerm(" |$");
    for (inner = 0; inner < 16 && inner + outer < count; inner++) {
      c = buffer[inner + outer];
      if (c >= ' ' && c <= 0x7f)
	printChar(c);
      else
	printChar('.');
    }
    printDTerm("|\r\n$");
  }

  return;
}
