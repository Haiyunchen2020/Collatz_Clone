#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>



void collatz (int number);
int collatzWrapper(void *arg);

const int STACK_SIZE =1024 * 1024; // 1 MB

int main (int argc, char* argv[]) {
	char *stack, *stackTop;
	int wstatus;
	pid_t pid;    // process identifier
	int number;

	printf("argc = %d \n", argc);        
	for (int i=0; i < argc; i++) {
		printf("argv[%d]=%s\n", i , argv[i]);
	}
	
	if (argc <= 1) {
		printf("Usage: %s positive integer\n",argv[0]);
		exit (EXIT_SUCCESS);
	}

	number = atoi (argv[1]);
	//printf("number = %d\n", number);
	
	// do the following in the child process
	//collatz(number);
	// pid = fork ();
	stack = malloc(STACK_SIZE);
	if (stack == NULL){
		fprintf(stderr, "memory allacation failed\n");
		exit(EXIT_FAILURE);
	}
	stackTop=stack+STACK_SIZE;
	
	pid = clone(collatzWrapper,stackTop, SIGCHLD,argv[1]);

	if (pid > 0) {           // in the parent process
		waitpid (pid,NULL,0);
		printf ("In the parent\n");
		//sleep (5);
	}
	else {          // error 
		perror ("clone()");
		exit (EXIT_FAILURE);
	}
	exit (EXIT_SUCCESS);
}


/* call in this fashion: collatzWrapper("123"); */
int collatzWrapper (void *arg){
	int number;
	number = atoi(arg);
	
	printf("number = %d\n",number);
	collatz(number);
	
}



void collatz (int number) {
	long n = number; 
	while (n !=1) {
		printf ("%ld    ",n);
		if (n %2 ==0) {
			n=n/2;
		}
		else {
			n=3*n+1;
		}
	}
	printf("%ld\n",n);

}

