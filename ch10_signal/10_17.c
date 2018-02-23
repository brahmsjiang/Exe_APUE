
#include <stdio.h>
#include <signal.h>

static volatile sig_atomic_t sigflag;
static sigset_t newmask,oldmask,zeromask;

static void sig_usr(int signo)
{
	sigflag=1;
}

void TELL_WAIT(void)
{
	if(signal(SIGUSR1,sig_usr)==SIG_ERR)
		perror("signal(SIGUSR1)");
	if(signal(SIGUSR1,sig_usr)==SIG_ERR)
		perror("signal(SIGUSR2)");

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGUSR1);
	sigaddset(&newmask,SIGUSR2);
	//block SIGUSR1 and SIGUSR2,and save current signal mask.
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
		perror("SIG_BLOCK");
}

void TELL_PARENT(pid_t pid)
{
	kill(pid,SIGUSR2);
}

void WAIT_PARENT(void)
{
	while(sigflag==0)
		sigsuspend(&zeromask);
	sigflag=0;
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
		perror("SIG_SETMASK");
}

void TELL_CHILD(pid_t pid)
{
	kill(pid,SIGUSR1);
}

void WAIT_CHILD(void)
{
	while(sigflag==0)
		sigsuspend(&zeromask);
	sigflag=0;
	//reset signal mask to original value
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
		perror("SIG_SETMASK");
}















