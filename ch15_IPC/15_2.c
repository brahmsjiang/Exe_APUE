
#include<stdio.h>
#include<wait.h>
#include<unistd.h>	//fork,pipe,write,close
#include<stdlib.h>	//exit
#include<string.h>	//strlen
#include<fcntl.h>



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
		printf("parent::start!!\n");
		close(fd[0]);	//close read end
		while(fgets(line,MAXLINE,fp)!=NULL){	//fgets
			n=strlen(line);
			if(write(fd[1],line,n)!=n){
				perror("write");exit(-2);
			}
		}

		printf("parent::waiting child...\n");
		if(wait(&status)<0){	//waitpid
			perror("waitpid");
		}

		printf("parent::ret,will exit\n");
		exit(0);
	}
	else{
		printf("child::start!!\n");
		close(fd[1]);	//close write end
		/*read file directory
		int fd0;
		fd0=open("test.txt",O_RDONLY,0666);
		while(1){
			n=read(fd0,line,1);
			printf("//child::read line is %s\n",line);
			printf("child::read byte is %d//\n",n);
			if(n==0){
				printf("child::read to end\n");
				break;
			}		
		}
		*/

		/*read pipe[0]
		while(1){
			int flags = fcntl(fd[0], F_GETFL);
    		fcntl(fd[0],F_SETFL,flags | O_NONBLOCK);
			n=read(fd[0],line,1);
			printf("===>child::read line is %s\n",line);
			printf("child::read byte is %d<===\n",n);
			if(n==0 || n==-1){
				printf("child::read to end or err\n");
				break;
			}
		}
		*/

		//int flags = fcntl(fd[0], F_GETFL);
		//fcntl(fd[0],F_SETFL,flags | O_NONBLOCK);
		fcntl(fd[0],F_SETFL,fcntl(fd[0],F_GETFL,0)|O_NONBLOCK);

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
		printf("child::argv0:%s	pager:%s\n",argv0,pager);
		//execl
		if(execl(pager,argv0,(char*)0)<0)
			perror("execl");exit(-4);
	}
	exit(0);
}




