#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>	//sleep

struct passwd * getpwnam(const char *name)
{
	struct passwd *ptr;
//	setpwent();
	while((ptr=getpwent())!=NULL){
		if(strcmp(name,ptr->pw_name)==0)
			break;
	}
	endpwent();
	return(ptr);
}

/*
struct passwd  
{  
    char * pw_name; //Username, POSIX.1
    char * pw_passwd; //Password
    __uid_t pw_uid; //User ID, POSIX.1  
    __gid_t pw_gid; //Group ID, POSIX.1
    char * pw_gecos; //Real Name or Comment field  
    char * pw_dir; //Home directory, POSIX.1  
    char * pw_shell; //Shell Program, POSIX.1
};  
*/

int main()
{
	struct passwd *res = NULL;
	char reqstr[100];
	int scanfret=0;
	while(1)
	{
	sleep(1);
	//scanf("%s",reqstr);	//end encounter with space,tab or enter
	scanfret=scanf("%[^\n]",reqstr);	//end encounter with \n,recv space or tab
	//while((getchar())!='\n');
	//对于%[]可以用^+任意字符结束字符串的输入
	//gets(reqstr);	//end encounter with enter
	printf("scanfret = %d\n",scanfret);
	printf("~~%d\n",*((char *)reqstr));
	res=getpwnam(reqstr);
	if(NULL==res){
		printf("%s is not found\n",reqstr);
		return 0;
	}
	else{
		printf("your req is %s====>\n",reqstr);
		printf("username: %s\n",res->pw_name);
		printf("passwd: %s\n",res->pw_passwd);
		printf("uid: %d\n",res->pw_uid);
		printf("git: %d\n",res->pw_gid);
		printf("realname: %s\n",res->pw_gecos);
		printf("homedir: %s\n",res->pw_dir);
		printf("shellprog: %s\n",res->pw_shell);
	}
	}	
	return 0;
}
