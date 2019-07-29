#include "stdio.h"
#include "sys/select.h"
#include "sys/time.h"
#include "sys/types.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"

int main() {

    fd_set read_fds;
    struct timeval tv;
    int ret, fd;

    ret = mkfifo("test_fifo", 0666); // creatr a named fifo 

    if(ret != 0) {
        perror("mkfifo:");
    }

    fd = open("test_fifo", O_RDWR); //open the fifo in RDWR
    if(fd < 0) {
        perror("open fifo:");
        return -1;
    }

    while(1) {
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);
        FD_SET(fd, &read_fds);

        tv.tv_sec = 1;
        tv.tv_usec = 0;

    
        ret = select(FD_SETSIZE, &read_fds, NULL, NULL, &tv);

        if(ret == -1) {
            perror("select()"); // error
        }
        else if(ret > 0) {
            char buffer[100] = {0};
            if(FD_ISSET(0, &read_fds)) {
                read(0, buffer, sizeof(buffer));
                printf("stdin buffer: %s\n", buffer);
            }
            else if(FD_ISSET(fd, &read_fds)) {
                read(fd, buffer, sizeof(buffer));
                printf("fifo buffer: %s\n", buffer);
            }
            else if(ret == 0) {
                printf("time out!\n");
            }
        }
    }
    return 0;
}
