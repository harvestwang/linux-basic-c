#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sem_api.h"

int main() {

	
	key_t key;
	pid_t pid;
	int ret, sem_id;


	key = ftok(".", 'a');
	if (key == -1)	{
		perror("ftok");
		exit(-1);
	}

	sem_id = semget(key, 1, IPC_CREAT);
	if (sem_id == -1) {
		perror("semget");
		exit(-1);
	}

	ret = init_sem(sem_id, 0);
	if (ret == -1) {
		exit(-1);
	}

	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(-1);
	}

	else if (pid == 0) {
		printf("This is in child process, pid is %d.\n", getpid());
		sleep(1);
		sem_v(sem_id);
	}

	else {
		sem_p(sem_id);
		printf("This is in parent process, pid is %d.\n", getpid());
		sem_v(sem_id);
		del_sem(sem_id);
	}
	return 0;
}