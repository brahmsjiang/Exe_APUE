/*************************************************************************
    > File Name: 15_11.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2018年02月14日 星期三 10时13分21秒
 ************************************************************************/

#include<stdio.h>
#include<sys/shm.h>
#include<stdlib.h>	//exit

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE 0600	//user read/write

char arr[ARRAY_SIZE]={0};	//itialized data=data
char array[ARRAY_SIZE];	//unitialized data=bss


int main(int argc,char* argv[],char* envp[])
{
	int shmid;
	char *ptr,*shmptr;

	printf("arr[] from %lx to %lx\n",(unsigned long)&arr[0],(unsigned long)&arr[ARRAY_SIZE]);
	printf("array[] from %lx to %lx\n",(unsigned long)&array[0],(unsigned long)&array[ARRAY_SIZE]);
	printf("stack around %lx\n",(unsigned long)&shmid);

	if((ptr=(char*)malloc(MALLOC_SIZE))==NULL){
		perror("malloc");exit(-1);
	}
	printf("malloced from %lx to %lx\n",(unsigned long)ptr,(unsigned long)ptr+MALLOC_SIZE);

	if((shmid=shmget(IPC_PRIVATE,SHM_SIZE,SHM_MODE))<0){
		perror("shmget");exit(-1);
	}
	if((shmptr=(char*)shmat(shmid,0,0))==(void*)-1){
		perror("shmat");exit(-1);
	}
	printf("shared memory attached from %lx to %lx\n",(unsigned long)shmptr,(unsigned long)shmptr+SHM_SIZE);

	if(shmctl(shmid,IPC_RMID,0)<0){
		perror("shmctl");exit(-1);
	}


	exit(0);
}

