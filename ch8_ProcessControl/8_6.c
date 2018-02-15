
#include<stdio.h>	//setbuf,BUFSIZ=8192
#include<stdlib.h>	//exit
#include<unistd.h>	//fork

char outbuf[BUFSIZ];

void exe_setbuf()
{
	setbuf(stdout,outbuf);	//connect outbuf to stdout,setbuf()相当于调用setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZE)
	puts("this is a test of buffered output.");	//add ret automatically
	fprintf(stdout,"hello world");
	fflush(stdout);	//refresh stream
	puts(outbuf);	//content of outbuf 
}

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
	//exe_setbuf();
	//////////////////////////////////
	pid_t pid;
	if((pid=fork())<0){
		perror("fork");
	}else if(pid==0){
		charatatime("output from child\n");
	}else{
		charatatime("output from parent\n");
	}


	//printf("mainfunc will exit!\n");
	exit(0);//fflush IO
	//_exit(0);//wont refresh IO
}



