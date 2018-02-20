#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXLINE 100

typedef void (Sigfunc)(int);
static void sig_int(int signo);
static void sig_alrm(int signo);

Sigfunc *signal_intr(int signo, Sigfunc *func)
{
	struct sigaction act,oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
//#ifdef SA_INTERRUPT
	act.sa_flags |= SA_RESTART;
//#endif
	if(sigaction(signo,&act,&oact)<0)
		return(SIG_ERR);
	return(oact.sa_handler);
}



int main(int argc,char *argv[],char *envp[])
{
	char buf[MAXLINE];
	int n;
	//if(signal_intr(SIGINT,sig_int)==SIG_ERR)
	//	perror("signal(SIGINT) err");
	if(signal_intr(SIGALRM,sig_alrm)==SIG_ERR)
		perror("signal(SIGALRM)	err");
	alarm(4);
	if((n=read(STDIN_FILENO,buf,MAXLINE))<0)
		perror("read err");
	write(STDOUT_FILENO,buf,n);
	//pause();
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




