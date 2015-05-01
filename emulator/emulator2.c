#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);

    grantpt(fd);
    unlockpt(fd);

    printf("created virtual port %s\n", ptsname(fd));

    while(1)
    {
        int r = rand() % 6;

        char msg[100];

        int b = read(fd, msg, 100);

        if (b > 0)
        {
            write(fd, msg, b);
            for (int i = 0; i < b; i++)
            {
                printf("0x%02x ", msg[i]);
                fflush(stdout);
            }
        }
    }
}

// EOF <stefan@scheler.com>

