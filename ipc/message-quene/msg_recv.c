#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 512

struct message {
    int msg_type;
    char msg_text[BUFFER_SIZE];
};

int main() {
    int qid;
    key_t key;
    struct message msg;

    if ((key = ftok(".", 'a')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((qid = msgget(key, IPC_CREAT|0666)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Open quene %d.\n", qid);

    do {
        memset(msg.msg_text, 0, BUFFER_SIZE);

        if (msgrcv(qid, (void *)&msg, BUFFER_SIZE, 0, 0) < 0) {
            perror("msgrcv");
            return -1;
        }

        printf("The message from process %d : %s", msg.msg_type, msg.msg_text);

    } while (strncmp(msg.msg_text, "quit", 4));

    if ((msgctl(qid, IPC_RMID, NULL)) < 0) {
        perror("msgctl");
        return -1;
    }
    
    return 0;
}
