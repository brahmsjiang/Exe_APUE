/*************************************************************************
    > File Name: 14_12.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2018年02月23日 星期五 14时10分43秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include <errno.h>		/* for definition of errno */
#include <stdarg.h>		/* ISO C variable aruments */
#include <string.h>

#define MAXLINE 200
#define MAXARGS 31

static void err_doit(int errnoflag, int error, const char *fmt, va_list ap);

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
	exit(1);
}


/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void err_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char	buf[MAXLINE];

	vsnprintf(buf, MAXLINE, fmt, ap);
	if (errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s",
		  strerror(error));
	strcat(buf, "\n");
	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL);		/* flushes all stdio output streams */
}

//int _vsnprintf(char* str, size_t size, const char* format, va_list ap);
//int fflush(FILE * stream);
void print_vari_str(const char* args, ...)
{
	char buf[MAXLINE];
	int argno = 0;
	
	va_list ap;
	va_start(ap, args);
    while (args != 0 && argno < MAXARGS)
    {
    	printf("%s\n",args);
		strcat(buf,args);
		args = va_arg(ap, const char *);
    }
	printf("===>buf: %s\n",buf);

	//strcat(buf, "\n");
	//fputs(buf, stdout);
	//fflush(NULL);
	va_end(ap);
}

int main(int argc, char * argv[], char* envp[])
{
	int fdin,fdout;
	void *src,*dst;
	struct stat statbuf;

	if(argc!=3);
		print_vari_str("usage:","<fromfile>","<tofile>");
		//err_quit("usage: %s <fromfile> <tofile>",argv[0]);
	if((fdin=open(argv[1], O_RDONLY))<0)
	exit(0);
}

