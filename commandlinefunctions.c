#include<stdio.h>
#include<sys/utsname.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
int bgpid[1001];
char bgproc[1001][20];
int bgnum=0;
int pidch;
char chprocname[20];
void tokenizer(char *,char *);
void cd(int inpfd,int outfd,char * args[],char homedir[]);
void pwd(int inpfd,int outfd);
void echo(int inpfd,int outfd,char * args[]);
void startProcess(int inpfd,int outfd,int flagbackground,char * args[]);
void bgjobs(int inpfd,int outfd);
void killbgall();
int Stringtoint(char a[]) ;
void cd(int inpfd,int outfd,char * args[],char homedir[])
{
	int flagin=0;
	int flagout=0;
	int stdincpy=dup(STDIN_FILENO);
	int stdoutcpy=dup(STDOUT_FILENO);
	if(inpfd!=-10)
	{
		if(inpfd==-1)
		{
			printf("Such file doesn't exist\n");
		}
		else
		{
			flagin=1;
			dup2(inpfd,STDIN_FILENO);
			close(inpfd);
		}
	}
	if(outfd!=-10)
	{
		flagout=1;
		dup2(outfd,STDOUT_FILENO);
		close(outfd);
	}
	if(args[1]==NULL)
	{
		chdir(homedir);
	}
	else
	{
		if(args[1][0]=='/')
		{
			if(chdir(args[1])!=0)
			{
				printf("Error in changing to directory\n" );
			//	finderrcd();
			}
		}
		else if (args[1][0]=='~')
		{
			char * fullpath;
			fullpath=malloc(strlen(args[1])-1+strlen(homedir)+strlen("/")+1);
			fullpath[0]='\0';
			strcat(fullpath,homedir);
			strcat(fullpath,"/");
			strcat(fullpath,args[1]+1);
			if(chdir(fullpath)!=0)
			{
				printf("Error in changing directory\n");
			//	finderrcd();
			}
		}
		else
		{
			char * fullpath;
			fullpath=malloc(strlen(args[1])+strlen(homedir)+strlen("/")+1);
			fullpath[0]='\0';
			char cwd[500];
			getcwd(cwd,sizeof(cwd));
			strcat(fullpath,cwd);
			strcat(fullpath,"/");
			strcat(fullpath,args[1]);
			if(chdir(fullpath)!=0)
			{
				printf("Error in changing directory\n");
			//	finderrcd();
			}
		}
	}
	if(flagin==1)
	{
		dup2(stdincpy,STDIN_FILENO);
		close(stdincpy);
	}
	if(flagout==1)
	{
		dup2(stdoutcpy,STDOUT_FILENO);
		close(stdoutcpy);
	}
}
void pwd(int inpfd,int outfd)
{
	int flagin=0;
	int flagout=0;
	int stdincpy=dup(STDIN_FILENO);
	int stdoutcpy=dup(STDOUT_FILENO);
	if(inpfd!=-10)
	{
		if(inpfd==-1)
		{
			printf("Such file doesn't exist\n");
		}
		else
		{
			flagin=1;
			dup2(inpfd,STDIN_FILENO);
			close(inpfd);
		}
	}
	if(outfd!=-10)
	{
		flagout=1;
		dup2(outfd,STDOUT_FILENO);
		close(outfd);
	}
	char cwd[500];
	getcwd(cwd,sizeof(cwd));
	printf("%s\n",cwd);
	if(flagin==1)
	{
		dup2(stdincpy,STDIN_FILENO);
		close(stdincpy);
	}
	if(flagout==1)
	{
		dup2(stdoutcpy,STDOUT_FILENO);
		close(stdoutcpy);
	}
}
void echo(int inpfd,int outfd,char * args[])
{
	int flagin=0;
	int flagout=0;
	int stdincpy=dup(STDIN_FILENO);
	int stdoutcpy=dup(STDOUT_FILENO);
	if(inpfd!=-10)
	{
		if(inpfd==-1)
		{
			printf("Such file doesn't exist\n");
		}
		else
		{
			flagin=1;
			dup2(inpfd,STDIN_FILENO);
			close(inpfd);
		}
	}
	if(outfd!=-10)
	{
		flagout=1;
		dup2(outfd,STDOUT_FILENO);
		close(outfd);
	}
	int i=1;
	while(args[i]!=NULL)
	{
		printf("%s",args[i]);
		i++;
	}
	printf("\n");
	if(flagin==1)
	{
		dup2(stdincpy,STDIN_FILENO);
		close(stdincpy);
	}
	if(flagout==1)
	{
		dup2(stdoutcpy,STDOUT_FILENO);
		close(stdoutcpy);
	}
}

void startProcess(int inpfd,int outfd,int flagbackground,char * args[])
{
	int pid;
	pid=fork();
	if(pid==0)
	{
		if(inpfd!=-10)
		{
			if(inpfd==-1)
			{
				printf("No such file found.\n");
			}
			else
			{
				dup2(inpfd,STDIN_FILENO);
				//close(inpfd);
			}
		}
		if(outfd!=-10)
		{
			dup2(outfd,STDOUT_FILENO);
			//close(outfd);
		}
		if(execvp(args[0],args)==-1)
		{
			printf("No such command found!\n");
			exit(0);
		}
	}
	else
	{
		if(outfd!=-10)
		{
			close(outfd);
		}
		if(inpfd!=-10)
		{
			close(inpfd);
		}
		if(flagbackground==0)
		{
			strcpy(chprocname,args[0]);
			pidch=pid;
			waitpid(pid,NULL,WUNTRACED);
		}
		if(flagbackground==1)
		{
			bgpid[bgnum]=pid;
			strcpy(bgproc[bgnum],args[0]);
			bgnum++;
		}
		printf("The PID of the process is %d and parent %d\n",pid,getpid());
	}
}
void pinfo(int inpfd,int outfd,char * args[])
{
	int flagin=0;
	int flagout=0;
	int stdincpy=dup(STDIN_FILENO);
	int stdoutcpy=dup(STDOUT_FILENO);
	if(inpfd!=-10)
	{
		if(inpfd==-1)
		{
			printf("Such file doesn't exist\n");
		}
		else
		{
			flagin=1;
			dup2(inpfd,STDIN_FILENO);
			close(inpfd);
		}
	}
	if(outfd!=-10)
	{
		flagout=1;
		dup2(outfd,STDOUT_FILENO);
		close(outfd);
	}
	//printf("I'm in pinfo\n");
	FILE * fp;
	char fname[500];
	char contents[1000];
	strcpy(fname,"/proc");
	strcat(fname,"/");
	strcat(fname,args[1]);
	strcat(fname,"/");
	strcat(fname,"status");
	//printf("%s\n",fname);
	if((fp=fopen(fname,"r"))==NULL)
	{
		printf("No such pid\n");
	}
	while(fscanf(fp,"%s",contents)!=EOF)
	{
		if(contents[strlen(contents)-1]==':')
			printf("\n%s",contents);
		else
			printf("%s",contents);
	}
	fclose(fp);
	printf("%s\n",contents);
	if(flagin==1)
	{
		dup2(stdincpy,STDIN_FILENO);
		close(stdincpy);
	}
	if(flagout==1)
	{
		dup2(stdoutcpy,STDOUT_FILENO);
		close(stdoutcpy);
	}
}
void bgjobs(int inpfd,int outfd)
{
	int flagin=0;
	int flagout=0;
	int stdincpy=dup(STDIN_FILENO);
	int stdoutcpy=dup(STDOUT_FILENO);
	if(inpfd!=-10)
	{
		if(inpfd==-1)
		{
			printf("Such file doesn't exist\n");
		}
		else
		{
			flagin=1;
			dup2(inpfd,STDIN_FILENO);
			close(inpfd);
		}
	}
	if(outfd!=-10)
	{
		flagout=1;
		dup2(outfd,STDOUT_FILENO);
		close(outfd);
	}
	int i,j=0;
	for(i=0;i<bgnum;i++)
	{
		if(waitpid(bgpid[i],NULL,WNOHANG)==0)
		{
			j++;
			printf("[%d] %s [%d]\n",j,bgproc[i],bgpid[i]);
		}
	}
	if(flagin==1)
	{
		dup2(stdincpy,STDIN_FILENO);
		close(stdincpy);
	}
	if(flagout==1)
	{
		dup2(stdoutcpy,STDOUT_FILENO);
		close(stdoutcpy);
	}
}

void killbgall()
{
	int i,j=0;
	for(i=0;i<bgnum;i++)
	{
		if(waitpid(bgpid[i],NULL,WNOHANG)==0)
		{
			kill(bgpid[i],SIGKILL);
		}
	}

}

void sendsig(char * args[])
{
	int i,j=0;
	int lab=Stringtoint(args[1]);
	int sigsend=Stringtoint(args[2]);
	//printf("lab:%d sigsend:%d\n",lab,sigsend);
	for(i=0;i<bgnum;i++)
	{
		if(waitpid(bgpid[i],NULL,WNOHANG)==0)
		{
			j++;
			if(j==lab)
			{
				//printf("lab:%d sigsend:%d name:%s\n",lab,sigsend,bgproc[i]);
				if(kill(bgpid[i],sigsend)==-1)
				{
					printf("Failed to send signal\n");
				}
			}
		}
	}
}

int Stringtoint(char a[])
{
  int c, sign, offset, n;
  if (a[0] == '-')
  {
      sign = -1;
  }
  if (sign == -1)
  {
    offset = 1;
  }
  else
  {
    offset = 0;
  }
  n = 0;
  for (c = offset; a[c] != '\0'; c++)
  {
    n = n * 10 + a[c] - '0';
  }
  if (sign == -1)
  {
    n = -n;
  }
  return n;
}
