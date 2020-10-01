#include<commandline.h>
#include<stdio.h>
#include<sys/utsname.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
int pid;
int exflag;
char  homedir[1000];
char cwd[1000];
struct utsname buf;
char *name;
char command[1001];
size_t len;
extern int pidch;
extern int bgpid[1001];
extern char bgproc[1001][20];
extern int bgnum;
extern char chprocname[20];
void printline()
{
	uname(&buf);
	name=getlogin();
	if(getcwd(cwd,sizeof(cwd))==NULL)
		printf("Cannot get Current Directory\n");
	printf("<%s @ %s : ",name,buf.sysname);
	int it1=0,flaghome=0;
	while(it1<strlen(homedir))
	{
		if(cwd[it1]!=homedir[it1])
		{
			flaghome=1;
			break;
		}
		it1++;
	}
	if (flaghome==1)
	{
		printf("%s > $ ",cwd);
	}
	else
	{
		printf("~%s > $ ",cwd+strlen(homedir));
	}
	return;
}
void sigfun(int sig)
{
	if(getpid()!=pid)
	{
		exit(0);
	}
	else
	{
		printf("\n");
		printline();
	}
	fflush(stdout);
}
void signotfun(int sig)
{
	if(pidch!=pid)
	{
		bgpid[bgnum]=pidch;
		strcpy(bgproc[bgnum],chprocname);
		bgnum++;
	//	printf("Caught ctrl + z on %d\n",pidch);
		pidch=getpid();
		kill(pidch,SIGTSTP);
	}
	else
	{
	//	printf("lololololo parent:%d child%d\n",pidch,getpid());
		printf("\n");
		printline();
	}
	fflush(stdout);
}
int main()
{
	exflag=0;
	pidch=getpid();
	getcwd(homedir,sizeof(homedir));
	pid = getpid();
	while(exflag==0)
	{
		signal(SIGINT,sigfun);
		signal(SIGTSTP,signotfun);
		printline();
		if(fgets(command,1000,stdin)==NULL)
			exit(0);
		seperator(command,homedir);
	}
	return 0;
}
