#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

// #define testSignal

void my_func(int sig_no) {

	switch(sig_no) {

		case SIGINT:
			printf("I have got SIGINT.\n");
			break;
		case SIGQUIT:
			printf("I have got SIGQUIT.\n");
			break;
		default:
			printf("I have got nothing.\n");
	};
};

int main() {

	printf("Waiting for signal SIGINT or SIGQUIT......\n");

#ifdef testSignal

	printf("This is testing signal.\n");
	signal(SIGINT, my_func);
	signal(SIGQUIT, my_func);

#else

	printf("This is testing sigaction.\n");
	struct sigaction action;
	

	action.sa_handler = my_func;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	sigaction(SIGINT, &action, 0);
	sigaction(SIGQUIT, &action, 0);
#endif

	pause();
	return  0;
}
