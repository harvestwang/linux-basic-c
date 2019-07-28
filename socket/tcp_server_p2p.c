#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    unsigned short port = 10086; // 端口号
    char *server_ip = "192.168.1.224"; // 地址设为本机IP

    if (argc > 1) {
        server_ip = argv[1];
    }

    if (argc > 2) {
        port = atoi(argv[2]);
    }

    int ret; // 系统调用返回值

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

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    // server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    ret = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)); // 绑定端口号和IP地址
    if (ret < 0) {
        perror("bind");
        close(server_sock);
        exit(-1);
    }

    ret = listen(server_sock, 5);
    if (ret < 0) {
        perror("listen");
        close(server_sock);
        exit(-1);
    }

    while(1) {
        struct sockaddr_in client_addr;
        char client_ip[INET_ADDRSTRLEN] = "";
        unsigned short client_port = 0;
        socklen_t client_addr_len = sizeof(client_addr);

        int connect_sock;
        connect_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);
        if (connect_sock < 0) {
            perror("accept");
            continue;
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        client_port = ntohs(client_addr.sin_port);

        printf("client ip = %s, port = %d.\n", client_ip, client_port);

        char recv_buf[512] = "";
        int recv_len = recv(connect_sock, recv_buf, sizeof(recv_buf), 0);
        while (recv_len > 0)
        {
            printf("receive data length = %d.\n", recv_len);
            printf("receive data = %s.\n", recv_buf);
            recv_len = recv(connect_sock, recv_buf, sizeof(recv_buf), 0);
        }
        close(connect_sock);
        printf("client closed.\n");
    }
    close(server_sock);
    return 0;
}