#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 200

volatile sig_atomic_t quitflag;	//equal to int

void pr_mask(const char* str);
static void sig_int(int signo);
void err_sys(const char *fmt, ...);
static void err_doit(int, int, const char*, va_list);

static void sig_int(int signo)
{
	if(signo==SIGINT)
		printf("\ninterrupt\n");
	else if(signo==SIGQUIT)
		quitflag = 1;
}

int main(void)
{
	sigset_t newmask,oldmask,zeromask;
	if(signal(SIGINT,sig_int)==SIG_ERR)
		err_sys("signal(SIGINT) error");
	if(signal(SIGQUIT,sig_int)==SIG_ERR)
		err_sys("signal(SIGQUIT) error");

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGQUIT);	//block quit then suspend to catch quit is a method of prevent from omitting quit

	//block SIGQUIT and save current signal mask.
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
		err_sys("SIG_BLOCK error");

	while(quitflag==0)
		sigsuspend(&zeromask);

	quitflag=0;
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
		err_sys("SIG_SETMASK error");
	exit(0);
}


void pr_mask(const char* str)
{
	sigset_t sigset;
	int errno_save;
	errno_save = errno;//we can be called by signal handlers
	if(sigprocmask(0,NULL,&sigset)<0)
		err_sys("sigprocmask error");
	printf("%s",str);
	if(sigismember(&sigset,SIGINT))	printf("SIGINT ");
	if(sigismember(&sigset,SIGQUIT)) printf("SIGQUIT ");
	if(sigismember(&sigset,SIGUSR1)) printf("SIGUSR1 ");
	if(sigismember(&sigset,SIGALRM)) printf("SIGALRM ");
	printf("\n");
	errno=errno_save;
}

void err_sys(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1,errno,fmt,ap);
	va_end(ap);
	exit(1);
}

//int vsnprintf(char* str,size_t size,const char* format,va_list ap)
static void err_doit(int errnoflag,int error,const char* fmt,va_list ap)
{
	char buf[MAXLINE];
	vsnprintf(buf,MAXLINE,fmt,ap);
	fputs(buf,stderr);
	fflush(NULL);
}




