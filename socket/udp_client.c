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
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    int ret;
    char send_buf[MAX_BUFFER_SIZE];

    while (1) {
        printf("send: ");

        fgets(send_buf, MAX_BUFFER_SIZE, stdin);
        send_buf[strlen(send_buf) - 1] = '\0';

        ret = sendto(server_sock, send_buf, strlen(send_buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            perror("sendto");
            close(server_sock);
        }
        
        if (strcmp(send_buf, "exit") == 0) {
            break;
        }

    }
    
    return 0;
}

