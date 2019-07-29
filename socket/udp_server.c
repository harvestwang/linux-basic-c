#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE         512

int main(int argc, char *argv[]) {


    unsigned short port = 10086;
    char *server_ip = "127.0.0.1";

    if (argc > 1) {
        server_ip = argv[1];
    }

    if (argc > 2) {
        port = atoi(argv[2]);
    }

    int server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    // server_addr.sin_addr.s_addr = inet_addr(server_ip);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    int ret;

    int opt = 1;
    ret = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (ret < 0) {
        perror("setsockopt");
        close(server_sock);
        exit(-1);
    }

    ret = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind");
        close(server_sock);
        exit(-1);
    }

    char recv_buf[MAX_BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t sock_len = sizeof(client_addr);

    char client_ip[INET_ADDRSTRLEN];
    unsigned short client_port;

    while (1) {
        
        memset(recv_buf, 0, MAX_BUFFER_SIZE);
        ret = recvfrom(server_sock, recv_buf, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &sock_len);
        if (ret < 0) {
            perror("recvfrom");
            close(server_sock);
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        client_port = ntohs(client_addr.sin_port);

        recv_buf[strlen(recv_buf)] = '\0';

        printf("[%s:%d]: %s.\n", client_ip, client_port, recv_buf);
        
        if (strcmp(recv_buf, "exit") == 0) {
            break;
        }

    }
    
    return 0;
}

