#include <signal.h>
#include <stdlib.h>
#include <unistd.h>	//sleep,alarm
#include <stdio.h>

typedef unsigned int uint;

//unsigned int alarm(unsigned int seconds)	=>ret:0 or unslept time ever
//every proc has only one alarm
//SIGALRM will term proc defaultly

static void sig_alrm(int signo)
{
	//nothing to do
	printf("at sig_alrm\n");
}

uint sleep1(uint nsecs)
{
	if(signal(SIGALRM,sig_alrm)==SIG_ERR)
		return(nsecs);
	uint lastval = alarm(nsecs);	//start timer
	printf("sleep 10s..for SIGALRM\n");
	sleep(10);
	pause();	//next caught signal wakes up
	return(alarm(0));	//turn off timer,return unslept time
}

int main(int argc, char* argv[], char* envp[])
{
	printf("alarm 4s..\n");
	alarm(4);
	printf("sleep 2s..\n");
	uint ret1 = sleep1(2);
	printf("ret val: %d\n",ret1);

}
