/*************************************************************************
    > File Name: 12_4.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2018年03月17日 星期三 10时13分21秒
 ************************************************************************/

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#define ARG_MAX 4096
extern char **environ;

pthread_mutex_t env_mutex;
static pthread_once_t init_done=PTHREAD_ONCE_INIT;

static void thread_init(void)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&env_mutex,&attr);
	pthread_mutexattr_destroy(&attr);
}

int getenv_r(const char *name,char *buf,int buflen)
{
	int i, len, olen;
	pthread_once(&init_done,thread_init);
	len=strlen(name);
	pthread_mutex_lock(&env_mutex);
	for(i=0;environ[i]!=NULL;i++){
		if((strncmp(name,environ[i],len)==0) && (environ[i][len]=='=')){
			olen=strlen(&environ[i][len+1]);
			if(olen>=buflen){
				pthread_mutex_unlock(&env_mutex);
				return(ENOSPC);
			}
			strcpy(buf,&environ[i][len+1]);
			pthread_mutex_unlock(&env_mutex);
			return(0);
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return(ENOENT);
}


int main(int argc,char* argv[],char* envp[])
{
	char tmpstr[ARG_MAX], resstr[ARG_MAX];
	printf("input envname you want to query: ");
	scanf("%s",tmpstr);
	int resval=getenv_r(tmpstr,resstr,100);
	printf("res: %d, %s : %s\n",resval,tmpstr,resstr);
	exit(0);
}

