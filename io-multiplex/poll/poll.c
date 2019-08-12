#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "errno.h"
#include "poll.h"
#include "time.h"

#define MAX_BUFFER_SIZE			1024
#define IN_FILES				3
#define TIME_DELAY				60
#define MAX(a, b)				((a > b)?(a) : (b))


int main() {

	struct pollfd fds[IN_FILES];
	char buffer[MAX_BUFFER_SIZE];

	int i, ret, real_read, maxfd;

	fds[0].fd = 0;
	fds[1].fd = open("in1", O_RDONLY | O_NONBLOCK);
	if (fds[1].fd < 0) {
		perror("open in1");
		return -1;
	}

	fds[2].fd = open("in2", O_RDONLY | O_NONBLOCK);
	if (fds[1].fd < 0) {
		perror("open in1");
		return -1;
	}

	for (i = 0; i < IN_FILES; i ++) {
		fds[i].events = POLLIN;
	}

	while(fds[0].events || fds[1].events || fds[2].events) {
		
		if (poll(fds, IN_FILES, 0) < 0) {
			perror("poll");
			return -1;
		}

		for (i = 0; i < IN_FILES; i ++) {

			if (fds[i].revents) {

				memset(buffer, 0, MAX_BUFFER_SIZE);
				real_read = read(fds[i].fd, buffer, MAX_BUFFER_SIZE);
				if (real_read < 0) {
					if (errno != EAGAIN) {
						return -1;
					}
				}
				else if (!real_read) {
					printf("%d closed.\n", fds[i].fd);
					close(fds[i].fd);
					fds[i].events = 0;
				}
				else {

					if (i == 0) {

						if (buffer[0] == 'q' || buffer[0] == 'Q') {
							return 0;
						}
					}
					else {
						buffer[real_read] = '\0';
						printf("%s\n", buffer);
					}
				}
			}

		}

	}

	return 0;
}