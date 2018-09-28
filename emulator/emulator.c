#define _XOPEN_SOURCE 501
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    srand(time(NULL));
#if 1
    int fd = open("/dev/tnt0", O_RDWR | O_NOCTTY);

//    grantpt(fd);
  //  unlockpt(fd);
#endif
    if (argc < 2)
    {
        return 1;
    }

   // printf("created virtual port %s\n", ptsname(fd));
    
    unsigned long c = 0;

    int fd_log = open(argv[1], O_RDONLY);

    if (fd_log == -1)
    {
        return 2;
    }


    printf("starting playback of %s in 10 seconds...\n", argv[1]);    
    
    sleep(10);
    while(1)
    {
        int r = rand() % 6;
        
        char msg[100];

        if (read(fd_log, msg, 100) != 0)
        {
            printf("{%s}\n", msg);
            write(fd, msg, strlen(msg));
        }
        else
        {
            lseek(fd_log, 0, SEEK_SET);
        }

        usleep(500000);
    }

    close(fd_log);
}

// EOF <stefan@scheler.com>

