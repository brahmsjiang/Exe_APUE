
#include <stdio.h>
#include <stdlib.h> //exit
#include <sys/wait.h> //wait
#include <unistd.h>
#include <signal.h>

static void sig_cld(int);

void newproc(char* str)
{
	pid_t pid;
	if((pid=fork())<0){
		perror("fork error");
	}
	else if(pid==0){	//child
		printf("In %s child start,sleep..\n",str);
		sleep(2);
		printf("In %s child end.\n",str);
		return;
	}
	else{
		printf("In %s parent start,pause..\n",str);
		pause();	//parent
		printf("In %s parent end.\n",str);
		return;
	}
}

int setsigact(int signo,bool resetsig)
{
	if(resetsig){
		struct sigaction act,oact;
		act.sa_handler = sig_cld;
		sigemptyset(&act.sa_mask);
		act.sa_flags=0;
		act.sa_flags|=SA_RESETHAND;
		act.sa_flags|=SA_RESTART;
		if(sigaction(signo,&act,&oact)<0)
			return -1;
	}
	return 0;
}

int main(int argc,char *argv[],char *envp[])
{
	if(setsigact(SIGCHLD,true)<0)
	//if(signal(SIGCHLD,sig_cld)==SIG_ERR)
		perror("signal error");
	newproc("main1");
	sleep(2);
	newproc("main2");
}

static void sig_cld(int signo)
{
	printf("In handler\n");
	pid_t pid;
	int status;
	printf("SIGCLD recv\n");
	if(setsigact(SIGCHLD,false)<0)
		perror("signal error");
	if((pid=wait(&status))<0)
		perror("wait error");
	printf("pid=%d\n",pid);
}


