
#include<stdio.h>
#include<wait.h>
#include<unistd.h>	//fork,pipe,write,close
#include<stdlib.h>	//exit
#include<string.h>	//strlen


#define DEF_PAGER "/bin/more"
#define MAXLINE	4096


int main(int argc, char* argv[])
{
	int n;
	int status;
	int fd[2];
	pid_t pid;
	char *pager,*argv0;
	char line[MAXLINE];
	FILE *fp;

	if(argc!=2){
		printf("usage: a.out <pathname>\n");exit(-1);
	}
	if((fp=fopen(argv[1],"r"))==NULL){	//IO stream
		perror("fopen");exit(-1);
	}
	if(pipe(fd)<0){
		perror("pipe");exit(-1);
	}
	if((pid=fork())<0){
		perror("fork");exit(-1);
	}
	else if(pid>0){
		printf("parent start!!\n");
		close(fd[0]);	//close read end
		while(fgets(line,MAXLINE,fp)!=NULL){	//fgets
			n=strlen(line);
			if(write(fd[1],line,n)!=n){
				perror("write");exit(-2);
			}
		}
		sleep(10);
#if 0
		printf("waiting child...\n");
		if(wait(&status)<0){	//waitpid
			perror("waitpid");
		}
#endif
		printf("ret to parent,will exit\n");
		exit(0);
	}
	else{
		printf("child start!!\n");
		close(fd[1]);	//close write end
		if(fd[0]!=STDIN_FILENO){
			if(dup2(fd[0],STDIN_FILENO)!=STDIN_FILENO){
				perror("dup2");exit(-3);
			}
			close(fd[0]);	//dont need this after dup2
		}
		if((pager=getenv("PAGER"))==NULL)
			pager=DEF_PAGER;
		if((argv0=strrchr(pager,'/'))!=NULL)
			argv0++;
		else
			argv0=pager;
		printf("argv0:%s	pager:%s\n",argv0,pager);
		//execl
		if(execl(pager,argv0,(char*)0)<0)
			perror("execl");exit(-4);
	}
	exit(0);
}




