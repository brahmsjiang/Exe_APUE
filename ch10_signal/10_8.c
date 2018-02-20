
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>

#define MAXLINE 100

static void sig_alrm(int);
static jmp_buf env_alrm;

int main(void)
{
	int n;
	char line[MAXLINE];

	if(signal(SIGALRM,sig_alrm)==SIG_ERR)
		perror("signal(SIGALRM) error");
	if(setjmp(env_alrm)!=0){
		printf("read timeout\n");
		exit(0);
	}
	alarm(5);
	if((n=read(STDIN_FILENO,line,MAXLINE))<0)
		perror("read error");
	alarm(0);

	write(STDOUT_FILENO,line,n);
	exit(0);
}

static void sig_alrm(int signo)
{
	longjmp(env_alrm,1);
}
