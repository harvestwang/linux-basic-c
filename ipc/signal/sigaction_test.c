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
    struct sigaction action;
    printf("Waiting for signal SIGINT or SIGQUIT...\n");

    action.sa_handler = MyFunc;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, 0);
    sigaction(SIGQUIT, &action, 0);
    pause();
    return 0;
}
