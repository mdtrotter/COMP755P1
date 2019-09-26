#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	
	//stores input string
	char *str;
	size_t bufsize = 200;
	size_t chars;
	str = (char *)malloc(bufsize * sizeof(char));

	//stores path variables
	char *pathArr[20];
	pathArr[0] = "/usr/bin/";
	pathArr[1] = "/bin/";
	int pathctr = 2;
	
	//stores path variable to be executed
	char buff[255];

	while(strcmp(str, "exit") != 0){
		
		//begins shell prompt
		printf("%s", "ash>> ");
		chars = getline(&str, &bufsize, stdin); 
		
		// splits input string by " " and "\n"
		char *pstr = strtok(str, " \n");

		//exit function
		if(strcmp(pstr, "exit") == 0){
			exit(0);
		}

		//cd function
		else if(strcmp(pstr, "cd") == 0){

			pstr = strtok(NULL, " \n");

			chdir(pstr);
		
		//path function
		}else if(strcmp(pstr, "path") == 0){
			pstr = strtok(NULL, " \n");
			while(pstr != NULL){
				
				//for each argument, stores it into pathArr array
				pathArr[pathctr] = pstr;
				pathctr++;
				pstr = strtok(NULL, " \n");
			}
		}else{
	
		int rc = fork();
		if (rc < 0) {
			//fork failed
			fprintf(stderr, "fork failed\n");
			exit(1);
		}else if(rc == 0){
			
			//loops through pathArr and checks if shell command is in one of the saved paths
			int pc = 0;
			strcpy(buff, pathArr[pc]);
			while(access(strcat(buff, pstr), X_OK) != 0 && pc < pathctr){
				memset(buff, 0, sizeof buff);
				pc++;
				strcpy(buff, pathArr[pc]);
			}
			if (pc < pathctr){
				char *myargs[2];
				myargs[0] = strdup(buff);
				myargs[1] = NULL;
				execvp(myargs[0], myargs);
			}
		}else{
			int rc_wait = wait(NULL);
		}
		}

	}
	return 0;

}
