
#include <stdio.h>
#include <stdlib.h> //exit
#include <sys/wait.h> //wait
#include <unistd.h>
#include <signal.h>

static void sig_cld(int);

int main(int argc,char *argv[],char *envp[])
{
	pid_t pid;
	if(signal(SIGCHLD,sig_cld)==SIG_ERR)
		perror("signal error");
	if((pid==fork())<0){
		perror("fork error");
	}
	else if(pid==0){	//child
		sleep(2);
		_exit(0);
	}
	pause();	//parent
	exit(0);
}

static void sig_cld(int signo)
{
	pid_t pid;
	int status;
	printf("SIGCLD received\n");
	if(signal(SIGCLD,sig_cld)==SIG_ERR)
		perror("signal error");
	if((pid=wait(&status))<0)
		perror("wait error");
	printf("pid=%d\n",pid);
}
