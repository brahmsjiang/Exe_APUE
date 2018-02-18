
#include<stdio.h>
#include<signal.h>
#include<unistd.h>	//sleep
#include<stdlib.h>	//exit

static void sig_quit(int);

int main(void)
{
	sigset_t newmask,oldmask,pendmask;
	if(signal(SIGQUIT,sig_quit)==SIG_ERR)
		perror("can't catch	SIGQUIT");
	//block SIGQUIT and save current signal mask
	sigemptyset(&newmask);
	sigaddset(&newmask,	SIGQUIT);
	printf("start sigprocmask===>\n");
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
		perror("SIG_BLOCK error");
	printf("end sigprocmask<===\n");
	printf("sleep 5s...\n");
	sleep(5);

	if(sigpending(&pendmask)<0)
		perror("sigpenging error");
	if(sigismember(&pendmask,SIGQUIT))
		printf("\nSIGQUIT pending\n");

	printf("start sigprocmask===>\n");
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
		perror("SIG_SETMASK error");
	printf("end sigprocmask<===\n");
	printf("SIGQUIT unblocked\n");
	printf("sleep 5s...\n");
	sleep(5);

	exit(0);
}

static void sig_quit(int signo)
{
	printf("caught SIGQUIT\n");
	if(signal(SIGQUIT,SIG_DFL)==SIG_ERR)
		perror("can't reset SIGQUIT");
}
