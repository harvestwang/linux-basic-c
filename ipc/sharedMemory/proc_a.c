#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "sem_api.h"

#define BUFFER_SIZE		512

int main() {

	key_t key_sema, key_semb, key_shm;
	int sema_id, semb_id, shm_id, ret;

	char *shm_addr;
	char buffer[BUFFER_SIZE];

	/*  
		create semaphore key    
	*/
	key_sema = ftok(".", 'a');
	if (key_sema < 0) {
		perror("[a] ftok");
		exit(-1);
	}
	else {
		printf("[a] semaphore key is %d\n", key_sema);
	}

	key_semb = ftok(".", 'b');
	if (key_semb < 0) {
		perror("[b] ftok");
		exit(-1);
	}
	else {
		printf("[b] semaphore key is %d\n", key_semb);
	}

	/*
		create shared-memory key
	*/
	key_shm = ftok(".", 's');
	if (key_shm < 0) {
		perror("ftok");
		exit(-1);
	}
	else {
		printf("shared-memory key is %d\n", key_shm);
	}

	/*
		create semaphore id
	*/
	sema_id = semget(key_sema, 1, IPC_CREAT);
	if (sema_id < 0) {
		perror("[a] semget");
		exit(-1);
	}
	else {
		printf("[a] semaphore id is %d.\n", sema_id);
	}

	semb_id = semget(key_semb, 1, IPC_CREAT);
	if (semb_id < 0) {
		perror("[b] semget");
		exit(-1);
	}
	else {
		printf("[b] semaphore id is %d.\n", semb_id);
	}

	/*
		create shared-memory id
	*/
	shm_id = shmget(key_shm, BUFFER_SIZE, 0666|IPC_CREAT);
	if (shm_id < 0) {
		perror("semget");
		exit(-1);
	}
	else {
		printf("shared-memory id is %d.\n", shm_id);
	}

	ret = init_sem(sema_id, 0);
	if (ret == -1) {
		printf("[a] initialize error\n");
		exit(-1);
	}

	ret = init_sem(semb_id, 0);
	if (ret == -1) {
		printf("[a] initialize error\n");
		exit(-1);
	}

	shm_addr = shmat(shm_id, 0, 0);
	if (shm_addr == (char *)(-1)) {
		perror("shmat");
		exit(-1);
	}

	sem_v(sema_id);

	while (1) {

		sem_p(sema_id);

		memset(buffer, BUFFER_SIZE, 0);

		printf("please input something: ");
		fgets(buffer, BUFFER_SIZE, stdin);

		strncpy(shm_addr, buffer, strlen(buffer));
		sem_v(semb_id);

		if ( !strncmp(buffer, "quit", 4)) {
			
			ret = shmdt(shm_addr);
			if (ret < 0) {
				perror("shmdt");
				exit(-1);
			}
			
			break;

		}
	}
	return 0;
}