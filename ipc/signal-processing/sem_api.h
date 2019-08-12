#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int init_em(int sem_id, int value);
int del_sem(int sem_id);
int sem_p(int sem_id);
int sem_v(int sem_id);
