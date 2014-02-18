#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

int main(void)
{
    srand(time(NULL));

    int fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);

    grantpt(fd);
    unlockpt(fd);

    printf("created virtual port %s\n", ptsname(fd));
    
    const char *str[] = {
        "foo",
        "bar",
        "test test test",
        "\teins\tzwei\tdrei\tvier\tfuenf",
        "\tpeter\t1\t2\thallo\thubert",
        "01234567890123456789012345678901234567890123456789012345678901234567890123456789"
    };

    unsigned long c = 0;

    int fd_log = open("superterm.log", O_RDONLY);
    sleep(10);
    while(1)
    {
        int r = rand() % 6;
        
        char msg[100];
//        sprintf(msg, "%08lx: %s\r\n", c++, str[r]);
        if (read(fd_log, msg, 100) != 0)
        {
            printf("{%s}\n", msg);
            write(fd, msg, strlen(msg));
        }
        usleep(100000);
    }

    close(fd_log);
}

// EOF <stefan@scheler.com>

