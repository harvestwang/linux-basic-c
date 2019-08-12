#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"

int main() {

	pid_t child_1, child_2, child;

	char *arg[] = {"env", NULL};
	char *envp[] = {"PATH=/tmp", "USER=debug", NULL};

	child_2 = fork();
	child_1 = fork();
	


	void print(void) {

		printf("Hello World.\n");
	}
	if (child_2 < 0) {
		printf("child_2 fork error.\n");
		exit(-1);
	}

	else if (child_2 == 0) {
		printf("[2] In child_2 process.\n");
		printf("[2] child_1's pid is %d\n", child_1);
		printf("[2] child_2's pid is %d\n", getpid());
		printf("[2] child_2's father's pid is %d\n", getppid());
		printf("[2] Test multiple process.\n");
		printf("\n");
		exit(0);
	}

	if (child_1 < 0) {
		printf("child_1 fork error.\n");
		exit(-1);
	}
	else if (child_1 == 0) {
		printf("[1] In child_1 process.\n");
		printf("[1] child_1's pid is %d\n", getpid());
		printf("[1] child_2's pid is %d\n", child_2);
		printf("[1] child_1's father's pid is %d\n", getppid());
		printf("[1] Test multiple process.\n");
		print();
		printf("\n");
		exit(0);
		// if (execve("/usr/bin/env", arg, envp) < 0) {
		// 	printf("execve error.\n");
		// }
	}

	else {
		
		printf("In father process.\n");
		printf("father's pid is %d\n", getpid());
		printf("child_1's pid is %d\n", child_1);
		printf("child_2's pid is %d\n", child_2);
		printf("\n");

		sleep(1);

	}

	return 0;
}