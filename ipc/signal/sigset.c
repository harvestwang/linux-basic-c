#include "stdio.h"
#include "stdlib.h"
#include "signal.h"
#include "unistd.h"
#include "sys/types.h"

void MyFunc(int sigNo) {
    printf("if you want to quit, please try SIGQUIT.\n");
}

int main(int argc, char *argv[]) {
    printf("Waiting for signal SIGINT or SIGQUIT...\n");

    sigset_t set, pendset;
    struct sigaction action1, action2;

//initial the set 
    if (sigemptyset(&set) < 0) {
        perror("sigemptyset");
        exit(1);
    }

//add SIGINT and SIGQUIT to the set 
    if (sigaddset(&set, SIGINT) < 0) {
        perror("sigaddset");
        exit(1);
    }

    if (sigaddset(&set, SIGQUIT) < 0) {
        perror("sigaddset");
        exit(1);
    }

    if (sigismember(&set, SIGINT)) {
        action1.sa_handler = MyFunc;
        sigemptyset(&action1.sa_mask);
        action1.sa_flags = 0;
        sigaction(SIGINT, &action1, NULL);
    }

    if (sigismember(&set, SIGQUIT)) {
        action2.sa_handler = SIG_DFL;
        sigemptyset(&action2.sa_mask);
        action2.sa_flags = 0;
        sigaction(SIGQUIT, &action2, NULL);
    }

    if (sigprocmask(SIG_BLOCK, &set, NULL) < 0) {
        perror("sigprocmask");
        exit(1);
    }
    else {
        printf("Signal set was blcoked, press any key!\n");
        getchar();
    }
    
    if (sigprocmask(SIG_UNBLOCK, &set, NULL) < 0) {
        perror("sigprocmask");
        exit(1);
    }
    else {
        printf("Signal set is in unblock state!\n");
    }
    while(1);
    return 0;
}
