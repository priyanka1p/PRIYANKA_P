#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	pid_t pid=fork();
	
	if(pid==0)
	{
		sleep(3);
		printf("My parent PID: %d \n", getppid());
		printf("i am CHILD having ID: %d \n", getpid());  	
	}
	else
	{
		
		printf("My Child PID: %d \n", pid);
		printf("i am PARENT having ID: %d \n", getpid());
		wait(NULL);
	}
	printf("      NEWWWWWWWWWWWWWWWWWW         \n");
	return 0;
}
