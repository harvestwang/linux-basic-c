#include "sem_api.h"


int init_sem(int sem_id, int value) {
	
	int ret;
	union semun sem_union;


	sem_union.val = value;

	ret = semctl(sem_id, 0, SETVAL, sem_union);
	if (ret == -1) {
		perror("Initialize semaphore");
		return -1;
	} 
	return 0;
}

int del_sem(int sem_id) {

	int ret;
	union semun sem_union;


	ret = semctl(sem_id, 0, IPC_RMID, sem_union);
	if (ret == -1) {
		perror("Delete semaphore");
		return -1;
	} 
	return 0;
}

int sem_p(int sem_id) {

	int ret;
	struct  sembuf sem_buffer;
	

	sem_buffer.sem_num = 0;
	sem_buffer.sem_op = -1;
	sem_buffer.sem_flg = SEM_UNDO;

	ret = semop(sem_id, &sem_buffer, 1);
	if (ret == -1) {
		perror("P operation");
		return -1;
	}
	return 0;
}

int sem_v(int sem_id) {

	int ret;
	struct sembuf sem_buffer;
	

	sem_buffer.sem_num = 0;
	sem_buffer.sem_op = 1;
	sem_buffer.sem_flg = SEM_UNDO;

	ret = semop(sem_id, &sem_buffer, 1);
	if (ret == -1) {
		perror("V operation");
		return -1;
	}
	return 0;
}