#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main (int argc, char * argv[]) {
    
    int ret = alarm(5);
    pause();
    printf("I am waiting for alarm signal.\n");
    // while(1);
    
    return 0;

}
