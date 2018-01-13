#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

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

typedef enum Axis_ {LEFT, RIGHT} Axis;

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

    if (argv[1][0] == 'i')
    {
        step(LEFT);
    }
    else if (argv[1][0] == 'd')
    {
        step(RIGHT);
    }
    else if (argv[1][0] == 'l')
    {
        light();
    }

    return 0;
}



int rotate(Axis axis, int angle)
{
    int steps = angle / 10;

    if (COM_FD < 0)
    {
        return 0;
    }
    printf("Rotando\n");
    switch (axis)
    {
        case LEFT:
            while (steps > 0)
            {
                step(LEFT);
                steps--;
            }
            break;

        case RIGHT:
            while (steps > 0)
            {
                step(RIGHT);
                steps--;
            }
            break;
    }
}


int step(Axis axis)
{
    if (COM_FD < 0)
    {
        return 0;
    }
    switch (axis)
    {
        case LEFT:
            write(COM_FD, "l;", 2);
            wait_hw();
            break;
        case RIGHT:
            write(COM_FD, "r;", 2);
            wait_hw();
            break;
    }
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