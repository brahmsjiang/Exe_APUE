/*************************************************************************
    > File Name: 14_12.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2018年02月23日 星期五 14时10分43秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>		/* for definition of errno */
#include <stdarg.h>		/* ISO C variable aruments */
#include <string.h>
#include <sys/stat.h>	//stat
#include <unistd.h>		//seek,write,ftruncate

#define	FILEMODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
//S_IRUSR user read; S_IWUSR user write; S_IRGRP gruop read; S_IROTH other read
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
	char buf[MAXLINE]={0};
	int argno = 0;
	
	va_list ap;
	va_start(ap, args);
    while (args != 0 && argno < MAXARGS)
    {
		strcat(buf,args);
		args = va_arg(ap, const char *);
    }
	strcat(buf, "\n");
	fputs(buf, stdout);
	fflush(NULL);
	va_end(ap);
}

int main(int argc, char * argv[], char* envp[])
{
	int fdin,fdout;
	void *src,*dst;
	struct stat statbuf;

	if(argc!=3)
		//print_vari_str("usage:","<fromfile>","<tofile>");
		err_quit("usage: %s <fromfile> <tofile>",argv[0]);
	if((fdin=open(argv[1], O_RDONLY))<0)
		err_sys("can't open %s for reading",argv[1]);
	//O_CREATE need third para=>filemode; O_TRUNC if writable len cut to zero
	if((fdout=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,FILEMODE))<0)
		err_sys("can't open %s for writing",argv[2]);
	if(fstat(fdin,&statbuf)<0)
		err_sys("fstat error");
	printf("fdin st_size: %ld\n",statbuf.st_size);

	//set size of output file
//#if 0
	if(lseek(fdout,statbuf.st_size-1,SEEK_SET)==-1)
		err_sys("lseek error");
	if(write(fdout," ",1)!=1)
		err_sys("write error");
	if(fstat(fdout,&statbuf)<0)
		err_sys("fstat error");
	printf("fdout st_size: %ld\n",statbuf.st_size);
//#endif
	//void *mmap(void *addr, size_t len, int prot, int flag, int filedes, off_t off)
	if((src=mmap(0,statbuf.st_size,PROT_READ,MAP_SHARED,fdin,0))==MAP_FAILED)
		err_sys("mmap error for input");
	if((dst=mmap(0,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fdout,0))==MAP_FAILED)
		err_sys("mmap error for output");
	memcpy(dst,src,statbuf.st_size);	//does the file copy
	//int ftruncate(int fd,off_t length) => ret 0 ok;-1 failed
	ftruncate(fdout,statbuf.st_size+strlen("fuck!"));
	memcpy((char*)dst+statbuf.st_size,"fuck!",strlen("fuck!"));

	if(fstat(fdout,&statbuf)<0)
		err_sys("fstat error");
	printf("fdout st_size: %ld\n",statbuf.st_size);

	exit(0);
}





