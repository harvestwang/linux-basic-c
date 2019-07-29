#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "sys/types.h"
#include "errno.h"
#include "string.h" 

#define MAX_DATA_LEN    256
#define DELAY_TIME      1


int main() {
    
    pid_t res;
    int pipe_fd[2];
    
    char buf[MAX_DATA_LEN];

    const char data[] = "Pipe Test Program";
    int res_read, res_write;
    
    memset(buf, 0, sizeof(buf));

    // 创建管道
    if (pipe(pipe_fd) < 0) {    
        printf("pipe create error.\n");
        exit(1);
    }

    res = fork();

    if (res < 0) {
        printf("subprocess create error");
        exit(1);
    }

    // 子进程中关闭管道的写端
    else if (res == 0) {
        printf("It is in subprocess.\n");
        close(pipe_fd[1]);
        sleep(DELAY_TIME * 2);
        res_read = read(pipe_fd[0], buf, MAX_DATA_LEN);
        if (res_read > 0) {
            printf("%d bytes read from the pipe is %s\n", res_read, buf);
        }

        close(pipe_fd[0]);
        exit(0);
    }

    // 父进程关闭读管道 
    else if (res > 0) {
        printf("This is in parent process.\n");
        close(pipe_fd[0]);
        sleep(DELAY_TIME);
        res_write = write(pipe_fd[1], data, strlen(data));

        if (res_write != -1) {
            printf("Parent wrote %d bytes : %s.\n", res_write, data);
        }

        close(pipe_fd[1]);

        waitpid(res, NULL, 0); // 等待子进程结束
        exit(0);
    }

    return 0;

}
