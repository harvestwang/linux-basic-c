#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_USER_NUM        5 // 最大允许连接数
#define MAX_BUFFER_SIZE     512

/*
    线程传输参数
    sock: connect建立的socket，用于传输数据
    client_addr: 用户的地址信息
 */
struct thread_arguments {
    int sock;
    struct sockaddr_in client_addr;
};

/*
    用户信息
    port: 用户端口号
    ip_addr: 用户IP地址
    message: 用户传输内容
 */
struct client_infomation {

    unsigned short port;
    char ip_addr[INET_ADDRSTRLEN];
    char message[MAX_BUFFER_SIZE];
};

void thread_function(void *ptr); 

int main(int argc, char *argv[]) {

    unsigned short port = 10086; // 端口号
    char *server_ip = "192.168.1.224"; // 地址设为本机IP

    if (argc > 1) {
        server_ip = argv[1];
    }

    if (argc > 2) {
        port = atoi(argv[2]); // 将字符串转换为数字
    }

    int ret; // 用于接收系统调用返回值

    /*
        AF_INET 表示 IPv4
        SOCK_STREAM 表示 TCP数据流
        0 表示 默认协议
     */ 
    int server_sock;
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(-1);
    }

    // 设置服务端地址信息
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    // inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    // server_addr.sin_addr.s_addr = htonl(INADDR_ANY);


    // 设置端口复用
    int opt = 1;
	ret = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
    if (ret < 0) {
        perror("setsockopt");
        close(server_sock);
        exit(-1);
    }

    // 绑定端口号和IP地址
    ret = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind");
        close(server_sock);
        exit(-1);
    }

    // 监听server socket
    ret = listen(server_sock, 5);
    if (ret < 0) {
        perror("listen");
        close(server_sock);
        exit(-1);
    }

    while(1) {

        struct thread_arguments thread_arg[MAX_USER_NUM];
        struct sockaddr_in client_addr[MAX_USER_NUM];
        socklen_t sockaddr_in_len = sizeof(struct sockaddr_in);
        int connect_sock[MAX_USER_NUM];
        
        pthread_t thread[MAX_USER_NUM];
        int ret_thread[MAX_USER_NUM];

        int i = 0;
        while(i < MAX_USER_NUM)
        {
            connect_sock[i] = accept(server_sock, (struct sockaddr *)&(client_addr[i]), &sockaddr_in_len);

            
            if (connect_sock[i] < 0) { 
                perror("accept.\n");
                continue;   // 连接不成功，用户数不变，继续监听
            }
            
            thread_arg[i].sock = connect_sock[i];
            // thread_arg[i].client_addr = (struct sockaddr *)&client_addr[i];
            memcpy(&thread_arg[i].client_addr, &client_addr[i], sizeof(client_addr[i]));

            ret_thread[i] = pthread_create(&thread[i], NULL, (void *)&thread_function, (void *)&thread_arg[i]);
            if (ret_thread[i] < 0) {
                perror("pthread_create。\n");
                printf("用户 [%d] 接收线程创建失败", i);
            }

            i ++;   // 连接成功，用户数加1
        }
        printf("超过最大连接数.\n");
    }
    close(server_sock);
    return 0;
}

void thread_function(void *ptr) {

    struct thread_arguments *arg_ptr = (struct thread_arguments *)ptr;

    struct client_infomation client_info;
    struct sockaddr_in client_addr;

    int sock = arg_ptr->sock;
    memcpy(&client_addr, &(arg_ptr->client_addr), sizeof(struct sockaddr_in));
    
    
    client_info.port = ntohs(client_addr.sin_port); // 获取用户端口号
    inet_ntop(AF_INET, &client_addr.sin_addr, client_info.ip_addr, INET_ADDRSTRLEN); // 获取用户IP地址

    int recv_len;
    recv_len = recv(sock, client_info.message, MAX_BUFFER_SIZE, 0);
    while (recv_len > 0) {
        // printf("receive data from %s:%d, length is %d.\n", client_info.ip_addr, client_info.port, recv_len);
        printf("[%s:%d]: %s.\n", client_info.ip_addr, client_info.port, client_info.message);

        if (strcmp(client_info.message, "exit") == 0) {
            printf("client %s:%d closed.\n", client_info.ip_addr, client_info.port);
            break;
        }
        memset(client_info.message, 0, MAX_BUFFER_SIZE);
        recv_len = recv(sock, client_info.message, MAX_BUFFER_SIZE, 0);

    }

    close(sock);
    pthread_exit(NULL);
}
