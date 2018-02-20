#include <signal.h>
#include <stdlib.h>
#include <unistd.h>	//sleep,alarm
#include <stdio.h>
#include <setjmp.h>

typedef unsigned int uint;

//unsigned int alarm(unsigned int seconds)	=>ret:0 or unslept time ever
//every proc has only one alarm
//SIGALRM will term proc defaultly

static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
	printf("at sig_alrm\n");
	//longjmp(env_alrm,1);
}

uint sleep1(uint nsecs)
{
	if(signal(SIGALRM,sig_alrm)==SIG_ERR)
		return(nsecs);
	//if(setjmp(env_alrm)==0){
		uint lastval = alarm(nsecs);	//start timer
		printf("sleep 4s..for SIGALRM\n");
	//	sleep(4);
		pause();	//next caught signal wakes up
	//}
	return(alarm(0));	//turn off timer,return unslept time
}

int main(int argc, char* argv[], char* envp[])
{
	printf("sleep 2s..\n");
	uint ret1 = sleep1(5);
	printf("ret val: %d\n",ret1);

}
