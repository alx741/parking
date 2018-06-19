#pragma once

#define COMPORT "/dev/ttyUSB0"

/* Configure serial port with
 *
 * 9600 baud
 * 8 bit data
 * No parity
 * Non blocking reading
 * Special lines ignore
 * No control Terminal
 * Read enable
 * RAW input/output
 *
 * Puts Serial Descriptor in COM_FD, 0 if fail
 */
int serial_init(void);
