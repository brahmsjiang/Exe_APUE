/*************************************************************************
    > File Name: 15_12.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2018年02月14日 星期三 10时13分21秒
 ************************************************************************/

#include<stdio.h>
#include<sys/shm.h>
#include<stdlib.h>	//exit
#include<fcntl.h>	//open,RDWR
#include<unistd.h>	//fork
#include<sys/mman.h>	//mmap,MAP_SHARED,PROT_READ,PROT_WRITE

#define NLOOPS 1000
#define SIZE 	sizeof(long) //size of shared memory area
#define err_sys(str) {\
	perror(str);	\
	exit(-1);	\
}

static int update(long *ptr)
{
	return ((*ptr)++); //ret value before increment
}

int main(int argc,char* argv[],char* envp[])
{
	int fd,i,counter;
	pid_t pid;
	void *area;

	if((fd=open("/dev/zero",O_RDWR))<0)	err_sys("open");
	if((area=mmap(0,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)	err_sys("mmap");
	close(fd);	//close /dev/zero now that it's mapped

	TELL_WAIT();

	if((pid=fork())<0)	err_sys("fork")
	else if(pid>0){
		printf("parent start!!");
		for(i=0;i<NLOOPS;i+=2){
			if((counter=update((long*)area))!=i){
				printf("parent: expected %d, got %d",i,counter);
				exit(-2);
			}
			TELL_CHILD(pid);
			WAIT_CHILD();
		}
	}
	else{
		printf("child start!!");
		for(i=1;i<NLOOPS+1;i+=2){
			WAIT_PARENT();

			if((counter=update((long*)area))!=i){
				printf("child: expected %d, got %d",i,counter);
				exit(-2);
			}
			TELL_PARENT(getpid());
		}
	}
	exit(0);
}

