#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <poll.h>
//#include <fcntl.h>
#include <unistd.h>	//STDIN_FILENO

#define MAXFD 100

//int select(int maxfd,fd_set* rd,fd_set* wt,fd_set* except,struct timeval* time)
//ret=>0 is timeout;-1 is err
void sleep_us0(unsigned long usec)
{
	timeval waitt;
	waitt.tv_sec = usec/1000000;	//sec
	waitt.tv_usec = usec%1000000;	//microsec is usec

	//fd_set rset;
	//int fd;
	//FD_ZERO(&rset);
	//FD_SET(STDIN_FILENO,&rset);
	select(0,NULL,NULL,NULL,&waitt);	
}

//int poll(struct pollfd fdarr[],nfds_t nfds,int timeout)
//ret=>num of fd;0 is timeout;-1 is err
void sleep_us1(unsigned long usec)
{
	pollfd dummy;
	int timeout;
	if((timeout=usec/1000)<0)
		timeout = 1;
	poll(&dummy,0,timeout);	//timeout is millisecond		milesec = 1000 microsec = 1000*1000 nanosecond
}

int main(int argc, char* argv[], char* envp[])
{
	if(argc!=3)
	{
		printf("usage %s <timval1> <timeval2>\n",argv[0]);
		exit(1);
	}
	printf("normal sleep 3s start\n");
	sleep(3);
	printf("normal sleep 3s end\n");
	//unsigned long strtoul(const char*nptr,char**endptr,int base)
	unsigned long t1=strtoul(argv[1],0,0);
	unsigned long t2=strtoul(argv[2],0,0);
	printf("select sleep start\n");
	sleep_us0(t1);
	printf("select sleep end\n");
	printf("poll sleep start\n");
	sleep_us1(t2);
	printf("poll sleep end\n");
	exit(0);
}




