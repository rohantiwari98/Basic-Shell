#include<string.h>
#include<commandline.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include <unistd.h>
void seperator(char * command,char homedir[])
{
		char * token;
		char * save1;
		size_t len = strlen(command);
		command[len-1]=0;
		token=strtok_r(command,";", &save1);
		while(token!=NULL)
		{
		//	printf("token = %s>", token);
			tokenizer(token,homedir);
			token=strtok_r(NULL,";", &save1);
		}
}
void tokenizer(char * command, char homedir[])
{
	char * command1 = command;
	char * command2 = command;
	int flagbackground=0;
	char * args[1000];
	for(int i=0;i<1001;i++)
	{
		args[i]=NULL;
	}
	int i=0;
	char * token;
	char * token2;
	char * save2;
	char * save3;
	int pipes=0;
	int pipeflag=0;
	int inpfd=-10;
	int outfd=-10;
	char a=command[0];
	int i1=0;
	while (command[i1]!=0)
	{
		if(command[i1]=='|')
		{
			pipeflag=1;
			pipes++;
		}
		i1++;
	}
	int pfd[2][2];
	int fdin;
	pipe(pfd[0]);
	pipe(pfd[1]);
	int osc=0;
	int eachpipe=pipes;
	char * save4;
	char *token3;
	token3=strtok_r(command,"|",&save4);
	while (token3!=NULL)
	{
		pipe(pfd[(osc+1)%2]);
		pipetokenizer(osc,pfd,token3,eachpipe,pipes,homedir);
		token3=strtok_r(NULL,"|",&save4);
		eachpipe--;
		osc=(osc+1)%2;
	}
}
void pipetokenizer(int osc,int pfd[][2],char * command,int eachpipe,int pipes,char homedir[])
{
	int inpfd=-10;
	int outfd=-10;
	int flagbackground=0;
	int i=0;
	char * args[1000];
	for(int i=0;i<1001;i++)
	{
		args[i]=NULL;
	}
	char * save5;
	char * token;
	token=strtok_r(command," ",&save5);
	if(eachpipe<pipes)
	{
		inpfd=pfd[osc][0];
	}
	if(eachpipe>0)
	{
		outfd=pfd[(osc+1)%2][1];
	}
	//printf("eachpipe=%d pipes=%d inpfd=%d outfd=%d\n",eachpipe,pipes,inpfd,outfd);
	while(token!=NULL)
	{
		if(strcmp(token,"<")==0 && eachpipe==pipes)
		{
			token=strtok_r(NULL," ",&save5);
			inpfd = open(token,O_RDONLY);
		//	printf("Input Stream located\n");
		}
		else if((strcmp(token,">")==0 || strcmp(token,">>")==0) && eachpipe==0)
		{
			if(strcmp(token,">")==0)
			{
				token=strtok_r(NULL," ",&save5);
				//printf("%s\n",token );
				outfd = open(token,O_WRONLY | O_TRUNC| O_CREAT,0666);
			//	printf("Output Stream located\n");
			}
			else
			{
				token=strtok_r(NULL," ",&save5);
				outfd = open(token,O_WRONLY | O_APPEND | O_CREAT ,0666);
			}
		}
		else if(strcmp(token,"&")==0)
		{
			flagbackground=1;
		//	printf("Background detected\n");
		}
		else
		{
	//		printf("ArgPipe %d: %s \n",i,token);
			args[i]=token;
			i++;
		}
		token=strtok_r(NULL," ",&save5);
	}
	args[i]=(char *)0;
	if(strcmp(args[0],"quit")==0)
	{
		exit(0);
	}
	//printf("%d\n",flagbackground);
	if(strcmp(args[0],"cd")==0)
	{
		cd(inpfd,outfd,args,homedir);
	}
	else if(strcmp(args[0],"pwd")==0)
	{
		pwd(inpfd,outfd);
	}
	else if(strcmp(args[0],"echo")==0)
	{
		echo(inpfd,outfd,args);
	}
	else if(strcmp(args[0],"pinfo")==0)
	{
		pinfo(inpfd,outfd,args);
	}
	else if(strcmp(args[0],"jobs")==0)
	{
		bgjobs(inpfd,outfd);
	}
	else if(strcmp(args[0],"killallbg")==0)
	{
		killbgall();
	}
	else if(strcmp(args[0],"sendsig")==0)
	{
		sendsig(args);
	}
	else
	{
		//printf("Not built in\n");
		startProcess(inpfd,outfd,flagbackground,args);
	}
}
