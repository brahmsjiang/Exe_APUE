
#include<stdio.h>
#include<signal.h>	//sig
#include<stdlib.h>	//exit
#include<errno.h>	//errno

void pr_mask(const char* str)
{
	sigset_t sigset;
	int errno_save;

	errno_save=errno;
	if(sigprocmask(0,NULL,&sigset)<0)
		perror("sigprocmask err");
	printf("%s:	%lx",str,sigset);
	
	if(sigismember(&sigset,SIGINT))		printf("SIGINT");
	if(sigismember(&sigset,SIGQUIT))	printf("SIGQUIT");
	if(sigismember(&sigset,SIGUSR1))	printf("SIGUSR1");
	if(sigismember(&sigset,SIGALRM))	printf("SIGALRM");

	printf("\n");
	errno=errno_save;
}

int main()
{
	pr_mask("fuck");
	exit(0);
}
