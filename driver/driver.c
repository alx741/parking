#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include "command.h"

#define COMPORT "/dev/ttyUSB0"

// COM PORT file descriptor
int COM_FD = 0;


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
 * Puts Serial Descriptor in COM_FD
 * returns 0 if fail, 1 if success
 */
int serial_init(void);

/* Returns 1 if hardware is redy to recive a command
 *
 * Its a blocking function, returns only when hardware is ready
 * */
int wait_hw(void);

int main(int argc, char *argv[])
{
    if (geteuid() != 0)
    {
        printf("Ejecutar como root: \"sudo ./driver <...>\"");
        return 1;
    }

    if (argc < 2)
    {
        printf("Uso: sudo ./driver [i|d|I|D|l|L]\n\n \
i - un paso a la izquierda\n \
d - un paso a la derecha\n \
I - giro completo a la izquierda\n \
D - giro completo a la derecha\n \
l - apagar luces\n \
L - encender luces\n \
f - bajar brazo frontal\n \
F - levantar brazo frontales\n \
b - bajar brazo posterior\n \
B - levantar brazo posterior\n \
                ");
        return 1;
    }

    serial_init();

    command_t command = {0};

    if (COM_FD < 0) { return 0; }

    switch (argv[1][0])
    {
        case 'd':
            command.command = STEP_RIGHT;
            break;

        case 'i':
            command.command = STEP_LEFT;
            break;

        case 'D':
            command.command = ROTATE_RIGHT;
            break;

        case 'I':
            command.command = ROTATE_LEFT;
            break;

        case 'l':
            command.command = LIGHTS_OFF;
            break;

        case 'L':
            command.command = LIGHTS_ON;
            break;

        case 'f':
            command.command = FRONT_ARM_DOWN;
            break;

        case 'F':
            command.command = FRONT_ARM_UP;
            break;

        case 't':
            command.command = BACK_ARM_DOWN;
            break;

        case 'T':
            command.command = BACK_ARM_UP;
            break;
    }

    write(COM_FD, &command, 1);
    wait_hw();
    return 0;
}


int serial_init(void)
{
    if (COM_FD > 0)
    {
        return 0;
    }

    //Open serial port file
    int fd = open(COMPORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        COM_FD = -1;
        return 0;
    }

    //Configure serial port
    struct termios config;
    if (tcgetattr(fd, &config) != 0)
    {
        COM_FD = -1;
        return 0;
    }

    cfsetispeed(&config, B9600);
    cfsetospeed(&config, B9600);

    config.c_cflag |= (CLOCAL | CREAD | CS8);
    config.c_cflag &= ~(PARENB | PARODD);
    config.c_iflag = 0;
    config.c_oflag = 0;
    config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    config.c_cc[VTIME] = 5;
    config.c_cc[VMIN] = 0;

    if (tcsetattr(fd, TCSANOW, &config) != 0)
    {
        COM_FD = -1;
        return 0;
    }

    COM_FD = fd;
    return 1;
}


int wait_hw(void)
{
    char a = 0;

    if (COM_FD < 0)
    {
        return 0;
    }

    while (a != '>')
    {
        read(COM_FD, &a, 1);
    }
    a = 0;
    return 1;
}
