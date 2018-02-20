
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

#define MAXLINE 100

static void sig_alrm(int);

int main(void)
{
	int n;
	char line[MAXLINE];

	if(signal(SIGALRM,sig_alrm)==SIG_ERR)
		perror("signal(SIGALRM) error");
	alarm(4);
	if((n=read(STDIN_FILENO,line,MAXLINE))<0)
		perror("read error");
	alarm(0);
	write(STDOUT_FILENO,line,n);
	exit(0);
}

static void sig_alrm(int signo)
{
	printf("sig_alrm timeout\n");
}
