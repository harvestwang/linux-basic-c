#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/shm.h"

#define SHM_SIZE		256

int main() {

	key_t key;
	pid_t pid;

	int shm_id;
	int ret;

	char flag[] = "WROTE";
	char shm_buf[SHM_SIZE];
	char * shm_addr;

	// key = ftok(".", 34);
	// if (key == -1) {
	// 	perror("ftok.");
	// 	exit(-1);
	// }
	// else
	// 	printf("key : %d\n", key);

	shm_id = shmget(IPC_PRIVATE, SHM_SIZE, 0666);
	if (shm_id == -1) {
		perror("shmget");
		exit(-1);
	}
	else
		printf("Create shared-memory: %d\n", shm_id);

	system("ipcs -m");

	pid = fork();

	if (pid == -1) {
		perror("fork");
		exit(-1);
	}

	else if (pid == 0) {

		printf("This is in child process.\n");

		shm_addr = shmat(shm_id, 0, 0);
		if (shm_addr == (char *)-1) {
			perror("Child: shmat");
			exit(-1);
		}
		else
			printf("Child: Attach shared-memory: %p\n", shm_addr);

		system("ipcs -m");

		while (strncmp(shm_addr, flag, strlen(flag))) {

			printf("Child: Wait for enable data...\n");
			sleep(5);
		}

		strcpy(shm_buf, shm_addr+strlen(flag));
		printf("Child: shared-memory: %s\n", shm_buf);

		ret = shmdt(shm_addr);
		if (ret == -1) {
			perror("Child: shmdt");
			exit(-1);
		}

		system("ipcs -m");

		ret = shmctl(shm_id, IPC_RMID, NULL);
		if (ret == -1) {
			perror("Child: shmctl");
			exit(-1);
		}
		else
			printf("Delete shared-memory.\n");
	}

	else {

		printf("This is in father process.\n");
		
		shm_addr = shmat(shm_id, 0, 0);
		if (shm_addr == (char *)-1) {
			perror("Parent: shmat");
			exit(-1);
		}
		else
			printf("Parent: Attach shared-memory: %p\n", shm_addr);

		system("ipcs -m");

		sleep(1);

		printf("\nInput something:\n");
		fgets(shm_buf, SHM_SIZE, stdin);
		
		strncpy(shm_addr + strlen(flag), shm_buf, strlen(shm_buf));
		strncpy(shm_addr, flag, strlen(flag));

		ret = shmdt(shm_addr);
		if (ret == -1) {
			perror("Parent: shmdt");
			exit(-1);
		}

		system("ipcs -m");

		waitpid(pid, NULL, 0);
		printf("Finished.\n");
	}


	return 0;
}

