#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

typedef void (Sigfunc)(int);
static void sig_int(int signo);
static void sig_alrm(int signo);

Sigfunc *signal(int signo, Sigfunc *func)
{
	struct sigaction act,oact;
	act.sa_handler=func;
	sigemptyset(&act.sa_mask);	//sigempty sa_mask,"act.sa_mask=0"
	act.sa_flags=0;
	if(signo==SIGALRM){
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif		
	}
	else{
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif		
	}
	if(sigaction(signo,&act,&oact)<0)
		return(SIG_ERR);
	return(oact.sa_handler);
}


int main(int argc,char *argv[],char *envp[])
{
	if(signal(SIGINT,sig_int)==SIG_ERR)
		perror("signal(SIGINT) err");
	if(signal(SIGALRM,sig_alrm)==SIG_ERR)
		perror("signal(SIGALRM)	err");
	alarm(4);
	pause();
	exit(0);
}

static void sig_int(int signo)
{
	printf("caught SIGINT\n");
}

static void sig_alrm(int signo)
{
	printf("caught SIGALRM\n");
}




