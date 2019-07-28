#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


#include <pthread.h>


#define MAX_THREAD_NUM      32

/*
    程序功能：主线程每隔5秒产生一个线程，子线程每隔随机不超过10s时间取消，在另一个子线程中统计其他子线程个数并每隔2秒打印输出
    除去统计子线程数量的子线程，其他子线程数量最大不超过N个,N为用户输入个数，随程序运行时作为参数传递
 */


int thread_counter = 0;
int thread_status[MAX_THREAD_NUM];

void print_thread_number(void);
void thread_function(void* ptr);

// 静态初始化互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char * argv[]) {
    
    int user_defined_thread_num;

    if (argc > 1) {
        user_defined_thread_num = atoi(argv[1]);
    }
    
    int ret = 0, thread_id;
    
    pthread_t thread[MAX_THREAD_NUM];
    pthread_t thread_count;

    ret = pthread_create(&thread_count, NULL, (void *)&print_thread_number, NULL);
    if (ret < 0) {
        perror("[thread_count]: pthread_create.");
        exit(EXIT_FAILURE);
    }

    thread_id = 0;
    while (thread_counter < user_defined_thread_num) {
        
        ret = pthread_create(&thread[thread_id], NULL, (void *)&thread_function, (void *)(&thread_id));
        if (ret < 0) {
            perror("[thread] pthread_create.");
            exit(EXIT_FAILURE);
        }

        ret = pthread_mutex_lock(&mutex);
        if (ret < 0) {
           perror("pthread_mutex_lock");
           exit(EXIT_FAILURE);
        }

        thread_counter ++;
        thread_status[thread_id] = 1;
        
        pthread_mutex_unlock(&mutex);
        if (ret < 0) {
           perror("pthread_mutex_unlock");
           exit(EXIT_FAILURE);
        }

        sleep(5);
        thread_id ++;
    }
    
    printf("用户连接数已达上限.\n");

    int i;
    for (i = 0; i <= thread_id; i ++) {
        if(thread_status[i] != 0) {
            printf("thread [%d] is alive.\n", i);
        }
    }

    ret = pthread_join(thread[0], NULL);
    if (ret < 0) {
           perror("pthread_join");
           exit(EXIT_FAILURE);
        }

    while(thread_counter != 0);
    return 0;
}

void thread_function(void* ptr) {
    
    int ret;
    int id = *((int *)ptr);
    int delay_time = rand() % 10;

    printf("thread [%d] start.\n", id);
    sleep(delay_time);

    // 加锁
    ret = pthread_mutex_lock(&mutex);
    if (ret < 0) {
        perror("pthread_mutex_lock");
        exit(EXIT_FAILURE);
    }

    thread_counter --;
    thread_status[id] = 0;

    // 解锁
    ret = pthread_mutex_unlock(&mutex);
    if (ret < 0) {
        perror("pthread_mutex_unlock");
        exit(EXIT_FAILURE);
    }

    printf("thread [%d] delay time: %d.\n", id, delay_time);
    printf("thread [%d] exit.\n", id);
    pthread_exit(NULL);
}

void print_thread_number(void) {

    while (1) {
        printf("thread counter = %d.\n", thread_counter);
        sleep(2);
    }
    
}
