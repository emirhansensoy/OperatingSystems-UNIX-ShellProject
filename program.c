#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "program.h"


char *builtin_strings[] = {"cd", "showpid", "exit"};

int (*builtin_commands[])(char**) = {&sau_cd, &sau_showpid, &sau_exit};

int builtinCount()
{
	return sizeof(builtin_strings) / sizeof(char *);
}

int sau_cd(char **args)
{
	if (args[1] == NULL)
	{
		printf("Error: argument expected after 'cd'\n");
	}
	else
	{
		chdir(args[1]);
	}
	
	return 1;
}

int sau_showpid(char **args)
{
	pid_t pid;
	int i;
	for(i = 0; i < 5; i++)
        {
        	pid = fork();
        	if(pid == 0)
        	{
          		printf("%ld\n", (long)getpid());
          		exit(0);
        	}
    	}
    	
    	int j;
        for(j = 0; j < 5; j++)
        {
        	waitpid(pid, NULL, 0);
	}
	
	return 1;
}

int sau_exit(char **args)
{
	int status;
	while (!waitpid(-1, &status, WNOHANG)){}
	printf("exit\n");
	exit(0);
}

int commandHandler(char * args[])
{
	int i;

	if (args[0] == NULL)
	{
		return 1;
	}
	for (i = 0; i < builtinCount(); i++)
	{
		if (strcmp(args[0], builtin_strings[i]) == 0)
		{
			(*builtin_commands[i])(args);
			return 1;
		}
	}
	
	pid_t pid;
	pid = fork();
	
	if(pid == -1)
	{
		return -1;
	}
	else if (pid == 0)
	{
		if (execvp(args[0], args) == -1)	
		{
			printf("Error: command '%s' not found\n", args[0]);
			kill(getpid(), SIGTERM);
		} 
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
	
	return 1;
}

void prompt()
{
	char hostName[80] = "";
	gethostname(hostName, sizeof(hostName));
	printf(BLU "%s@%s:" RESET RED "%s > " RESET, getenv("LOGNAME"), hostName, getcwd(currentDirectory, 80));
}

int main(int argc, char **argv)
{
	char line[MAX_CHAR];
	char *tokens[MAX_WORD];
	int tokenCount;
	
	while(1)
	{
		prompt();
		
		//parsing
		memset(line, '\0', MAX_CHAR);
		fgets(line, MAX_CHAR, stdin);
		
		if((tokens[0] = strtok(line," \n\t")) == NULL) 
		{
			continue;
		}
		
		tokenCount = 1;
		
		while((tokens[tokenCount] = strtok(NULL, " \n\t")) != NULL) 
		{
			tokenCount++;
		}
		
		commandHandler(tokens);
	}
}
