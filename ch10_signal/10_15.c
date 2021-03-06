#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 200

void pr_mask(const char* str);
static void sig_int(int signo);
void err_sys(const char *fmt, ...);
static void err_doit(int, int, const char*, va_list);


int main(void)
{

	sigset_t newmask,oldmask,waitmask;
	pr_mask("program start: ");
	if(signal(SIGINT,sig_int)==SIG_ERR)
		err_sys("signal(SIGINT) error");
	//if(signal(SIGUSR1,sig_int)==SIG_ERR)
	//	err_sys("signal(SIGUSR1) error");
	if(signal(SIGQUIT,sig_int)==SIG_ERR)
		err_sys("signal(QUIT) error");
	sigemptyset(&waitmask);
	sigaddset(&waitmask,SIGUSR1);	//waitmask=> USR1
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGINT);		//newmask=> INT

	//block SIGINT and save current signal mask.
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
		err_sys("SIG_BLOCK error");

	//critical region of code
	pr_mask("in critical region: ");
	
	//printf("block INT first,wait..\n");
	//pause();

	//pause,allow all signals except SIGUSR1
	printf("block USR1 then,wait..\n");
	if(sigsuspend(&waitmask)!=-1)
		err_sys("sigsuspend error");
	pr_mask("after return from sigsuspend: ");

	//reset signal mask which unblocks SIGINT
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
		err_sys("SIG_SETMASK error");

	//printf("unblock all,wait..\n");
	//pause();
	//and continue processing...
	pr_mask("program exit: ");
	exit(0);
}


static void sig_int(int signo)
{
	pr_mask("\nin sig_func: ");
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




