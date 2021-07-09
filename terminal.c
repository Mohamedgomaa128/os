#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <unistd.h>


void logg(){
	//create logging file and write in it
	int v = open("logs.txt", O_CREAT | O_RDWR);
	lseek(v, 0, SEEK_END);
	write(v, "Child process was terminated\n", 29);
}

char** parse (char* str){
	// parsing input string

	// removing \n char form the string
	int len = strlen(str);
	if (len > 0 && str[len - 1] == '\n')
		str[len - 1] = '\0';


	// allocating slots for substrings of command
	char** ret  = malloc (sizeof(char *) * 100);
	ret[0] = strtok(str, " ");


	//parsing by removing the spaces in the string
	int i = 0;
	while (ret[i]){
		i++;
		ret[i] = strtok(NULL, " ");
	}

	return ret;
}

int main(){
	signal(SIGCHLD, &logg);

	while (1){
		char line[1000];

		fgets(line, sizeof(line), stdin);

		//handle exit command
		if (strstr(line, "exit"))
			exit(9);

		//if have & sign
		int bolCont = strstr(line, "&");

		if (bolCont){
			int len = strlen(line);
			for (int i = 0; i < len; i++)
				if (line[i] == '&')
					line[i] = ' ';;
		}

		char** commands = parse(line);


		int f = fork();

		if (f == 0){

			int val = execvp(commands[0], commands);

			//if invalid command
			if (val == -1)
				printf("cannot execute command\n");

			exit(0);
		}
		else {
			int w = -10;
			int val = - 100;


			//if does't contain &
			if (!bolCont)
				val = waitpid(f, &w, WUNTRACED);
			else
				signal(SIGCHLD, SIG_IGN); //to handle zombies

			//printf("%d %d \n", w, val);
		}

	}
}

