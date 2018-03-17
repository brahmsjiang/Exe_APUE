/*************************************************************************
    > File Name: 12_4.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2018年03月17日 星期三 10时13分21秒
 ************************************************************************/

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define ARG_MAX 4096
static char envbuf[ARG_MAX];
extern char **environ;
//#include <stdlib.h>
//extern char **environ;
//这个变量是用来打印环境变量。

char* getenv(const char *name)
{
	int i, len;
	len=strlen(name);
	for(i=0;environ[i]!=NULL;i++){
		if((strncmp(name,environ[i],len)==0) && (environ[i][len]=='=')){
			strcpy(envbuf,&environ[i][len+1]);
			return(envbuf);
		}
	}
	return NULL;
}

int main(int argc,char* argv[],char* envp[])
{
	char tmpstr[ARG_MAX];
	printf("input envname you want to query: ");
	scanf("%s",tmpstr);
	printf("%s : %s\n",tmpstr,getenv(tmpstr));
	exit(0);
}

