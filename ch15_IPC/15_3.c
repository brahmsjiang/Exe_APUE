
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>	//pipe

static int pfd1[2],pfd2[2];

void TELL_WAIT(void)
{
	if(pipe(pfd1)<0 || pipe(pfd2)<0)
		perror("pipe");
}

void TELL_PARENT(pid_t pid)
{
	if(write(pfd2[1],"c",1)!=1)
		perror("wrtie");
}

void WAIT_PARENT(void)
{
	char c;
	if(read(pfd1[0],&c,1)!=1)
		perror("read");
	if(c!='p'){
		printf("WAIT_PARENT: incorrect data");
		exit(-1);
	}
}

void TELL_CHILD(pid_t pid)
{
	if(write(pfd1[1],"p",1)!=1)
		perror("write");
}

void WAIT_CHILD(void)
{
	char c;
	if(read(pfd2[0],&c,1)!=1)
		perror("read");
	if(c!='c'){
		printf("WAIT_CHILD: incorrect data");
	}
}






