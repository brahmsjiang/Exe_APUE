
#include<stdio.h>	//setbuf,BUFSIZ=8192
#include<stdlib.h>	//exit
#include<unistd.h>	//fork

extern void TELL_WAIT();
extern void WAIT_PARENT();
extern void TELL_CHILD();

static void charatatime(char* str)
{
	char* ptr;
	int c;
	setbuf(stdout,NULL);	//set stdout without buffered
	for(ptr=str;(c=*ptr++)!=0;)
		putc(c,stdout);
}



int main(int argc,char* argv[],char* envp[])
{
	pid_t pid;
	TELL_WAIT();

	if((pid=fork())<0){
		perror("fork");
	}else if(pid==0){
		charatatime("output from child\n");
		WAIT_PARENT();
	}else{
		charatatime("output from parent\n");
		TELL_CHILD(pid);
	}

	exit(0);//fflush IO
}



