
#include <pwd.h>	//getpwnam
#include <stdio.h>
#include <stdlib.h>	//exit
#include <signal.h>
#include <unistd.h>	//alarm,pause
#include <string.h>	//str
#include <fcntl.h>	//filectl

#define TESTFILE "test.txt"
static char line[100];
static int fd;

static void my_alarm2(int signo)
{
	if(read(fd,line,1)!=1)
		perror("read err");
	printf("in handler2,readres is %s\n",line);
	alarm(1);
}

static void my_alarm1(int signo)
{
	struct passwd *rootptr;
	if((rootptr=getpwnam("root"))==NULL)
		perror("getpwnam(root) err");
	else
		printf("in handler1,getpwnam res is %s\n",rootptr->pw_name);
	alarm(1);
}

int main(int argc, char* argv[], char* envp[])
{
//#if 0
	struct passwd *ptr;
	signal(SIGALRM,my_alarm1);
	alarm(1);
	for(;;){
		sleep(1);
		if((ptr=getpwnam("sar"))==NULL)
			perror("getpwnam(sar) err");
		if(strcmp(ptr->pw_name,"sar")!=0)
			printf("return value corrupted!,pw_name = %s\n",ptr->pw_name);
		else
			printf("in main,getpwnam res is %s\n",ptr->pw_name);

	}
//#endif
	/////////////////////////////////////////////////
#if 0
	if((fd=open(TESTFILE,O_RDONLY))==-1)
		perror("open err");
	signal(SIGALRM,my_alarm2);
	alarm(1);
	while(1){
		sleep(1);
		if(read(fd,line,1)!=1)
			perror("read err");
		printf("in main,readres is %s\n",line);

	}
#endif
	exit(0);
}






