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
			
			int pc = 0;
			char *myargs[10];
			//loops through pathArr and checks if shell command is in one of the saved paths
			memset(buff, 0, sizeof buff);
			strcpy(buff, pathArr[pc]);
			char comm[255] = "";
			//char *comm;
			//comm = (char *)malloc(bufsize * sizeof(char *));
			int init = 1;
			int argC = 1;

			while(pstr != NULL){
				if(init == 1){
					strcat(comm, pstr);
					init = 0;
				}else{
					myargs[argC] = pstr;
					argC++;
				}
				pstr = strtok(NULL, " \n");
			
			}
			//comm[(sizeof(comm) - 1)] = '\0';

			printf("%s\n", comm);
			printf("%s\n", myargs[1]);
			while(access(strcat(buff, comm), X_OK) != 0 && pc < pathctr){
				printf("%d\n", pc);
				pc++;
				memset(buff, 0, sizeof buff);
				strcpy(buff, pathArr[pc]);
			}

			printf("%s\n", buff);
			printf("%d\n", argC);
			if (pc < pathctr){
				//char *myargs[2];
				myargs[0] = strdup(buff);
				myargs[argC] = NULL;
				execv(myargs[0], myargs);
			}
		}else{
			int rc_wait = wait(NULL);
		}
		}

	}
	return 0;

}
