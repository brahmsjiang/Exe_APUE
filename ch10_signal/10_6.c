
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

typedef unsigned int uint;

static void sig_int(int);
static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
	printf("at sig_alrm\n");
	longjmp(env_alrm,1);
}

uint sleep2(uint nsecs)
{
	if(signal(SIGALRM,sig_alrm)==SIG_ERR)
		return(nsecs);
	if(setjmp(env_alrm)==0){
		uint lastval = alarm(nsecs);	//start timer
		pause();	//next caught signal wakes up
	}
	return(alarm(0));	//turn off timer,return unslept time
}

int main(void)
{
	uint unslept;
	if(signal(SIGINT,sig_int)==SIG_ERR)
		perror("signal(SIGINT) err");
	unslept = sleep2(5);
	printf("sleep2 returned: %u\n",unslept);
	exit(0);
}

static void sig_int(int signo)
{
	int i,j;
	volatile int k;
	//tune these loops to run for more than 5 seconds
	//on whatever system this test program is run.
	printf("\nsig_int starting\n");
	for(i=0;i<300000;i++)
		for(j=0;j<4000;j++)
			k += i*j;
	printf("sig_int finished\n");
}




