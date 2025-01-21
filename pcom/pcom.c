/**
 * @brief   Polling COM port library
 * @author  Thomas Cherryhomes 
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <dos.h>
#include "pcom.h"

/**
 * @brief the current UART I/O base
 */
int pcom_base = 0x3F8;

/**
 * @brief the current interrupt number
 */
int pcom_int = 12;

/**
 * @brief set the pcom_base UART to desired serial parameters
 * @param speed Baud rate
 * @param parity Parity, e.g. 'N'one
 * @param data Data bits per character (e.g. 8)
 * @param stopbits # of stop bits per character (e.g. 1)
 */
void pcom_set(long speed, char parity, int data, int stopbits)
{
	unsigned char low_divisor = (char)(115200L / speed) & 0xFF;
	unsigned char high_divisor = (char)((115200L / speed) >> 8);
	unsigned char lcr_out = 0;

	/* Disable all interrupts to UART */
	outportb(pcom_base + IER, 0);
	inportb(pcom_base);

	/* Set baud rate */
	outportb(pcom_base + LCR, LCR_DLAB);
	outportb(pcom_base + DLL, low_divisor);
	outportb(pcom_base + DLM, high_divisor);

	/* Set parity, data bits, stop bits */
	outportb(pcom_base + LCR, 0);
	
	if ( parity == 'E')
		lcr_out = LCR_EVEN_PARITY;
	else if ( parity == 'O')
		lcr_out = LCR_ODD_PARITY;
	else
		lcr_out = LCR_NO_PARITY;

	if ( stopbits == 2)
		lcr_out |= LCR_2_STOP_BITS;

	if ( data == 6)
		lcr_out |= LCR_6_DATA_BITS;
	else if ( data == 7 )
		lcr_out |= LCR_7_DATA_BITS;
	else if ( data == 8 )
		lcr_out |= LCR_8_DATA_BITS;

	outportb(pcom_base + LCR, lcr_out);
}

/**
 * @brief Assert DTR signal
 * @param t Toggle value
 */
void pcom_set_dtr(unsigned char t)
{
	if (t)
		outportb(pcom_base + MCR, 
			 inportb(pcom_base + MCR) | 0x01);
	else
		outportb(pcom_base + MCR,
			 inportb(pcom_base + MCR) & 0xFE);
}

/**
 * @brief Read next byte from UART, wait if not available
 * @return Byte read.
 */
unsigned char pcom_getc(void)
{
	/* Wait until data ready */
	while(1)
	{
		if ((inportb(pcom_base + LSR) & 0x01) == 0x01)
			return (inportb(pcom_base + RBR) & 0xFF);
	}
}

/**
 * @brief Transmit a byte
 * @param b byte to transmit
 */
void pcom_putc(unsigned char b)
{
	/* Wait for transmitter ready */
	while (!inportb(pcom_base + LSR) & 0x20);

	/* Queue byte */
	outportb(pcom_base + THR, b);
	delay(1);
}

/**
 * @brief Send buffer of given len to serial port
 * @param buf Pointer to buffer
 * @param len number of bytes to send, must be len or less.
 */
void pcom_put(unsigned char *buf, unsigned short len)
{
	while (len)
	{
		pcom_putc(*buf);
		len--;
		buf++;
	}
}
