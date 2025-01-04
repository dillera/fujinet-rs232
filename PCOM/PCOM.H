/**
 * @brief   Polling COM port library
 * @author  Thomas Cherryhomes 
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef PCOM_H
#define PCOM_H

/**
 * @brief 8250 registers relative to base address
 */
#define RBR              0    /* Receive buffer register */
#define THR              0    /* Transmit holding reg.   */
#define IER              1    /* Interrupt Enable reg.   */
#define IER_RX_DATA      1    /* Enable RX interrupt bit */
#define IER_THRE         2    /* Enable TX interrupt bit */
#define IIR              2    /* Interrupt ID register   */
#define IIR_MODEM_STATUS 0    /* Modem stat. interrupt ID*/
#define IIR_TRANSMIT     2    /* Transmit interrupt ID   */
#define IIR_RECEIVE      4    /* Receive interrupt ID    */
#define IIR_LINE_STATUS  6    /* Line stat. interrupt ID */
#define LCR              3    /* Line control register   */
#define LCR_DLAB         0x80 /* Divisor access bit      */
#define LCR_EVEN_PARITY  0x8  /* Set parity 'E' bits     */
#define LCR_ODD_PARITY   0x18 /* Set parity 'O' bits     */
#define LCR_NO_PARITY    0    /* Set parity 'N' bits     */
#define LCR_1_STOP_BIT   0    /* Bits to set 1 stop bit  */
#define LCR_2_STOP_BITS  4    /* Bits to set 2 stop bits */
#define LCR_5_DATA_BITS  0    /* Bits to set 5 data bits */
#define LCR_6_DATA_BITS  1    /* Bits to set 6 data bits */
#define LCR_7_DATA_BITS  2    /* Bits to set 7 data bits */
#define LCR_8_DATA_BITS  3    /* Bits to set 8 data bits */
#define MCR              4    /* Modem control register  */
#define MCR_DTR          1    /* Bit to turn on DTR      */
#define MCR_RTS          2    /* Bit to turn on RTS      */
#define MCR_OUT1         4    /* Bit to turn on OUT1     */
#define MCR_OUT2         8    /* Bit to turn on OUT2     */
#define LSR              5    /* Line Status register    */
#define MSR              6    /* Modem Status register   */
#define DLL              0    /* Divisor latch LSB       */
#define DLM              1    /* Divisor latch MSB       */

/**
 * @brief the current UART I/O base
 */
extern int pcom_base;

/**
 * @brief the current interrupt number
 */
extern int pcom_int;

/**
 * @brief set the pcom_base UART to desired serial parameters
 * @param speed Baud rate
 * @param parity Parity, e.g. 'N'one
 * @param data Data bits per character (e.g. 8)
 * @param stopbits # of stop bits per character (e.g. 1)
 */
void pcom_set(long speed, char parity, int data, int stopbits);

/**
 * @brief Assert DTR signal
 * @param t Toggle value
 */
void pcom_set_dtr(unsigned char t);

/**
 * @brief Read next byte from UART, wait if not available
 * @return Byte read.
 */
unsigned char pcom_getc(void);

/**
 * @brief Transmit a byte
 * @param b byte to transmit
 */
void pcom_putc(unsigned char b);

/**
 * @brief Send buffer of given len to serial port
 * @param buf Pointer to buffer
 * @param len number of bytes to send, must be len or less.
 */
void pcom_put(unsigned char *buf, unsigned short len);

#endif /* PCOM_H */
