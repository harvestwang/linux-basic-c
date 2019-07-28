#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/* 初始化读写锁 */
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

/* 全局资源 */
int global_num = 10;

void err_exit(const char *err_msg)
{
    printf("error:%s\n", err_msg);
    exit(1);
}

/* 读锁线程函数 */
void *thread_read_lock(void *arg)
{
    char *pthr_name = (char *)arg;

     while (1)
    {
         /* 读加锁 */
        pthread_rwlock_rdlock(&rwlock);

        printf("线程%s进入临界区，global_num = %d\n", pthr_name, global_num);
        sleep(1);
        printf("线程%s离开临界区...\n", pthr_name);

        /* 读解锁 */
        pthread_rwlock_unlock(&rwlock);

        sleep(1);
    }

    return NULL;
}

/* 写锁线程函数 */
void *thread_write_lock(void *arg)
{
    char *pthr_name = (char *)arg;

    while (1)
    {
        /* 写加锁 */ 
        pthread_rwlock_wrlock(&rwlock);
        /* 写操作 */
        global_num++;
        printf("线程%s进入临界区，global_num = %d\n", pthr_name, global_num);
        sleep(1);
        printf("线程%s离开临界区...\n", pthr_name);

        /* 写解锁 */
        pthread_rwlock_unlock(&rwlock);

        sleep(2);
    }

 return NULL;
}

int main(void)
{
 pthread_t tid_read_1, tid_read_2, tid_write_1, tid_write_2;

 /* 创建4个线程，2个读，2个写 */
 if (pthread_create(&tid_read_1, NULL, thread_read_lock, "read_1") != 0)
     err_exit("create tid_read_1");

 if (pthread_create(&tid_read_2, NULL, thread_read_lock, "read_2") != 0)
     err_exit("create tid_read_2");

 if (pthread_create(&tid_write_1, NULL, thread_write_lock, "write_1") != 0)
     err_exit("create tid_write_1");

 if (pthread_create(&tid_write_2, NULL, thread_write_lock, "write_2") != 0)
     err_exit("create tid_write_2");

 /* 随便等待一个线程，防止main结束 */
 if (pthread_join(tid_read_1, NULL) != 0)
     err_exit("pthread_join()");

 return 0;
}
