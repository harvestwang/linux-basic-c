#include "sem_com.h"

int init_sem(int sem_id, int init_value) {
    union semun sem_union;
    sem_union.val = init_value;

    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
        perror("Initialize semphore");
        return -1;
    }
        
    return 0;
}

int del_sem(int sem_id) {
    union semun sem_union;

    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        perror("Delete semphore.");
        return -1;
    }
    return 0;
}

int sem_p(int sem_id) {
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_buf, 1) == -1) {
        perror("sem_p operation");
        return -1;
    }

    return 0;
}


int sem_v(int sem_id) {
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_buf, 1) == -1) {
        perror("sem_p operation");
        return -1;
    }

    return 0;
}
