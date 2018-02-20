
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>	//exit,abort
#include<unistd.h>	//pause

static void sig_usr(int);

int main(int argc, char* argv[],char* envp[])
{
	if(signal(SIGUSR1,sig_usr)==SIG_ERR)
		perror("can't catch SIGUSR1");
	if(signal(SIGUSR2,sig_usr)==SIG_ERR)
		perror("can't catch SIGUSR2");
	for(;;)
		pause();

	exit(0);
}

static void sig_usr(int signo)
{
	if(signo==SIGUSR1)
		printf("received SIGUSR1\n");
	else if(signo==SIGUSR2)
		printf("received SIGSUR2\n");
	else{
		printf("received signal %d\n",signo);
		abort();
	}
}
