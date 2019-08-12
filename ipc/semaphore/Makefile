sem_fork: sem_fork.o sem_com.o 
	gcc sem_fork.o sem_com.o -o sem_fork 

sem_fork.o: sem_fork.c sem_com.h
	gcc -c sem_fork.c 

sem_com.o: sem_com.c sem_com.h 
	gcc -c sem_com.c

clean:
	rm -rf *.o sem_fork
