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

void sense(int id)
{
    command_t command = {0};
    if (id == 1)
    {
        command.command = FRONT_ENTRY_SENSE;
    }
    else if (id == 2)
    {
        command.command = BACK_ENTRY_SENSE;
    }

    write(COM_FD, &command, 1);
    char response;
    int timeout = 0;
    while (response != 't' && response != 'f')
    {
        read(COM_FD, &response, 1);
        if (timeout++ >= 10000)
        {
            printf("false\n");
            return;
        }
    }
    if (response == 't') { printf("true\n"); }
    else { printf("false\n"); }
    wait_hw();
    return;
}

int main(int argc, char *argv[])
{
    if (geteuid() != 0)
    {
        printf("Ejecutar como root: \"sudo ./driver <...>\"");
        return 1;
    }

    if (argc < 2)
    {
        printf("Uso: sudo ./driver [i|d|I|D|l|L|f|F|p|P|s|S|r|R]\n\n \
[i|d] - un paso a la izquierda|derecha\n \
[I|D] - giro completo a la izquierda|derecha\n \
[l|L] - apagar|encender luces\n \
[f|F] - bajar|levantar brazo frontal\n \
[p|P] - bajar|levantar brazo posterior\n \
[s|S] - leer sensor frontal|posterior (true = carro en la entrada)\n \
[r|R] - desactivar|activar reacción automática de brazos\n \
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

        case 'p':
            command.command = BACK_ARM_DOWN;
            break;

        case 'P':
            command.command = BACK_ARM_UP;
            break;

        case 's':
            sense(1);
            break;

        case 'S':
            sense(2);
            break;

        case 'r':
            command.command = DISABLE_AUTO_ENTRY_REACT;
            break;

        case 'R':
            command.command = ENABLE_AUTO_ENTRY_REACT;
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
