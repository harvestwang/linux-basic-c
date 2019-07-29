#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

struct thread_data {

    int id;
    char message[20];
};

void print_message(void *ptr);

int main(int argc, char *argv[]) {

    void * ret;

    char test_str[20] = "this is thread";

    int ret_thread[3];
    pthread_t thread[3];
    struct thread_data td[3];

    
    int i, tmp;

    for (i = 0; i < 3; i ++) {

        td[i].id = i;
        // td[i].message = "Hello World.\n";
        memcpy(&(td[i].message), test_str, sizeof(test_str));

        printf("%s", td[i].message);
        ret_thread[i] = pthread_create(&thread[i], NULL, (void *)print_message, (void *)&(td[i]));

        if (ret_thread[i] < 0) {
            printf("thread[%d] 创建失败.\n", td[i].id);
        }
        else {
            printf("thread[%d] 创建成功.\n", td[i].id);
        }
    }
    
    for (i = 0; i < 3; i ++) {
        
        tmp = pthread_join(thread[i], &ret);
        printf("thread[%d] return value is %d.\n", i, (int)ret);
        
        if (tmp != 0) {
            printf("thread[%d] return value(tmp) is %d.\n", i, tmp);
            printf("cannot join thread[%d].\n", i);
        }
        printf("thread[%d] end.\n", i);
    }
    
    return 0;
}

void print_message( void *ptr ) {

    struct thread_data *td_ptr;

    int i, delay_time, exit_code;

    td_ptr = (struct thread_data *)ptr;

    for (i = 0; i < 3; i ++) {

        delay_time = rand() % 6;
        sleep(delay_time);

        // printf("sleep time = %d.\n", delay_time);
        printf("[%d]:%s, sleep time = %d.\n", td_ptr->id, td_ptr->message, delay_time);
    }

    exit_code = rand();
    printf("[%d]: the thread's id is %u, exit code is %d.\n", td_ptr->id, (unsigned int)pthread_self(), exit_code);

    pthread_exit((void *)exit_code);
}
