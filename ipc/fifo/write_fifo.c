#include "stdio.h"
#include "string.h"
#include "sys/select.h"
#include "sys/time.h"
#include "sys/types.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"

int main() {

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
        char *str = "this is for test";
        write(fd, str, strlen(str));
        printf("after write to fifo\n");
        sleep(5);        
    }
    return 0;
}
