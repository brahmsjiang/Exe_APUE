
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>	//alarm

#define MICE_DEV "/dev/input/mice"

static volatile sig_atomic_t sigflag;
static volatile sig_atomic_t alrm_flag,intr_flag;
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

void handle_alrm()
{
	printf("handle alrm!\n");
}

void handle_intr()
{
	printf("handle intr!\n");
}

void sig_func(int signo)
{
	if(SIGINT==signo)
		intr_flag=1;
	else if(SIGALRM==signo)
		alrm_flag=1;
	else;
}

int main(int argc,char* argv[],char* envp[])
{
	if(signal(SIGINT,sig_func)==SIG_ERR)
		perror("signal(SIGINT) err");
	if(signal(SIGALRM,sig_func)==SIG_ERR)
		perror("signal(SIGALRM) err");

	fd_set rfds;
	int retval,fd1;
	if(fd1=open(MICE_DEV,O_RDONLY)<0)
		perror("read err");
	FD_ZERO(&rfds);
	FD_SET(0,&rfds);
	FD_SET(fd1,&rfds);

	alarm(3);
	while((retval=select(fd1+1,&rfds,NULL,NULL,NULL))<0){
		if(errno==EINTR){	//intr by a sig
			printf("errno==EINTR\n");
			if(alrm_flag)
				handle_alrm();
			else if(intr_flag)
				handle_intr();
		}
		else{

		}
	}
	if(retval>0){
		if(FD_ISSET(fd1,&rfds)){
			printf("Data is available from mouse now\n");
		}
	}

	exit(0);
}













