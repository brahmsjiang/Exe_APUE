
#include<stdio.h>
#include<wait.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc,char* argv[])
{
	pid_t pid;
	int sts;
	if((pid=fork())<0){
		perror("fork");
		exit(-1);
	}
	else if(pid==0){
		printf("child...\n");
		execl("/bin/more","more","test.txt",(char*)0);
	}
	else{
		printf("parent,wait child...\n");
		wait(&sts);
		printf("parent,ret\n");
	}
	return 0;

}
