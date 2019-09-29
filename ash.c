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

	//stores counter for commands executed already
	int execCtr=0;

	while(strcmp(str, "exit") != 0){
		
		//begins shell prompt
		printf("%s", "ash>> ");
		chars = getline(&str, &bufsize, stdin); 
		if(strchr(str, '|') != NULL || strchr(str, '>') != NULL){
			system(str);
		}else{
			// splits input string by " " and "\n"
			char *pstr = strtok(str, " \n");

			//exit function
			if(strcmp(pstr, "exit") == 0){
			exit(0);

			//cd function
			}else if(strcmp(pstr, "cd") == 0){

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
		while(pstr != NULL){
						

			/*int rc = fork();
			if (rc < 0) {
				//fork failed
				fprintf(stderr, "fork failed\n");
				exit(1);
			}else if(rc == 0){
			*/
			int pc = 0;
			char *myargs[10];
			//loops through pathArr and checks if shell command is in one of the saved paths
			
			//clears buff array
			memset(buff, 0, sizeof buff);

			//copy first path in pathArr to buff
			strcpy(buff, pathArr[pc]);

			//stores commands
			char comm[255] = "";

			//init is a boolean value that is set to false once the first argument is saved to comm
			int init = 1;

			//argC tracks each argument associated with the first command
			int argC = 1;
			
			//loops through command string and places command at the beginning and args into myargs array
			while(pstr != NULL){
				//if & is found, then execute command up to & and save execCtr to start after & in next loop through
				if(strcmp(pstr, "&") == 0){
					pstr = strtok(NULL, " \n");
					break;
				}
				if(init == 1){
					strcat(comm, pstr);
					init = 0;
					execCtr++;
				}else{	
					myargs[argC] = pstr;
					argC++;
					execCtr++;
				}
				pstr = strtok(NULL, " \n");
				printf("pstr is now %s\n", pstr);
			}
			
			printf("%s\n", comm);
			while(access(strcat(buff, comm), X_OK) != 0 && pc < pathctr){
				printf("%d\n", pc);
				pc++;
				memset(buff, 0, sizeof buff);
				strcpy(buff, pathArr[pc]);
			}

			printf("%s\n", buff);
			printf("%d\n", argC);
			
			int rc = fork();
			if (rc < 0){
				//fork failed
				fprintf(stderr, "fork failed\n");
				exit(1);
			}

			if (rc == 0){


			//if a path was found for the current argument
				if (pc < pathctr){
					//char *myargs[2];
					myargs[0] = strdup(buff);
					myargs[argC] = NULL;
					if(execvp(myargs[0], myargs) == -1){
						char error_message[30] = "error\n";
						write(STDERR_FILENO, error_message, strlen(error_message));
					}
					exit(1);
				}
				exit(1);
			
			}else{
				int rc_wait = wait(NULL);
			}
		}
		}
	}
	}
	return 0;
}
