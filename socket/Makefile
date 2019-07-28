.SUFFIXES:.c .o

CC=gcc

SRCS1=tcp_server.c
OBJS1=$(SRCS1:.c=.o)
EXEC1=tcp_server

SRCS2=tcp_client.c
OBJS2=$(SRCS2:.c=.o)
EXEC2=tcp_client

all: $(OBJS1) $(OBJS2)
	$(CC) -o $(EXEC1) $(OBJS1) -lpthread
	$(CC) -o $(EXEC2) $(OBJS2) -lpthread
	@echo "-----------------------------OK-----------------------"

.c.o:
	$(CC) -Wall -g -c -o $@ $< -lpthread

clean:
	rm -rf $(EXEC1) $(OBJS1)
	rm -rf $(EXEC2) $(OBJS2)
