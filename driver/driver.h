/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

				File: driver.h

*/

#ifndef DRIVER_H
#define DRIVER_H


// SERIAL PORT FILE
#define COMPORT "/dev/ttyUSB0"

// Number of boxes
#define X_SIZE 8
#define Y_SIZE 8

// Move repeats to reach next box
#define X_REPEATS 1
#define Y_REPEATS 1


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


/* Moves hardware in AXIS with direccion DIR
 * AXIS = <x,y>
 * DIR = <f,b>
 * 
 * Returns 0 if is not possible to move
 */
int step(char axis,char dir);


/* Moves hardware to 0,0
 * 
 * Returns 0 if is not possible to move
 */
int origin(void);


/* Moves hardware to X,Y
 * 
 * Returns 0 if is not possible to move
 */
int to(int x, int y);


#endif
