#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {

    unsigned short server_port = 10086;     // 服务器监听的端口号
    char *server_ip = "192.168.1.224";      // 服务器的ip地址

    if (argc > 1) {
        server_ip = argv[1];
    }

    if (argc > 2) {
        server_port = atoi(argv[2]);
    }

    int ret = 0;    // 系统调用返回值

    int client_sock;
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    ret = connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("connect");
        close(client_sock);
        exit(-1);
    }

    char send_buf[512] = {0};
    printf("send data to %s:%d.\n", server_ip, server_port);

    while (1) {
        printf("send: ");
        fgets(send_buf, sizeof(send_buf), stdin); // 从键盘读取输入内容
        send_buf[strlen(send_buf) - 1] = '\0';
    
        ret = send(client_sock, send_buf, strlen(send_buf), 0);
        if (ret < 0) {
            perror("send");
            continue;
        }

        if (strcmp(send_buf, "exit") == 0) {
            break;
        }
    }
    
    close(client_sock);

    // system("netstat -an | grep 10086");	// 查看连接状态
	
	// while(1);
 
	return 0;
}