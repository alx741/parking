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

typedef enum Axis_ {LEFT, RIGHT, RLEFT, RRIGHT} Axis;

int step(Axis axis);
int light(void);
int rotate(Axis axis, int angle);


int main(int argc, char *argv[])
{
    if (geteuid() != 0)
    {
        printf("Ejecutar como root: \"sudo driver ...\"");
        return 1;
    }

    if (argc < 2)
    {
        printf("Uso: sudo ./driver [i|d]\n\n"
                "i - girar a la izquierda\nd - girar a la derecha\n");
        return 1;
    }

    serial_init();

    switch (argv[1][0])
    {
        case 'd':
            step(RIGHT);
            break;

        case 'i':
            step(LEFT);
            break;

        case 'D':
            step(RRIGHT);
            break;

        case 'I':
            step(RLEFT);
            break;

        case 'l':
            step(RLEFT);
            break;
    }

    return 0;
}



int step(Axis axis)
{
    command_t command = {0};

    if (COM_FD < 0) { return 0; }

    switch (axis)
    {
        case LEFT:
            command.command = STEP_LEFT;
            break;
        case RIGHT:
            command.command = STEP_RIGHT;
            break;
        case RLEFT:
            command.command = ROTATE_LEFT;
            break;
        case RRIGHT:
            command.command = ROTATE_RIGHT;
            break;
    }

    printf("%#01X", command);
    write(COM_FD, &command, 1);
    wait_hw();
}

int light()
{
    if (COM_FD < 0)
    {
        return 0;
    }

    write(COM_FD, "p;", 2);
    wait_hw();
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
