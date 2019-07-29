#include "stdio.h"
#include "stdlib.h"
#include "signal.h"

void MyFunc(int sigNo) {
    if (sigNo == SIGINT) {
        printf("It is a SIGINT signal.\n");
    }
    else if (sigNo = SIGQUIT) {
        printf("It is a SIGQUIT signal.\n");
    }
}

int main(int argc, char *argv[]) {
    printf("Waiting for signal SIGINT or SIGQUIT...\n");

    signal(SIGINT, MyFunc);
    signal(SIGQUIT, MyFunc);
    pause();
    return 0;
}
