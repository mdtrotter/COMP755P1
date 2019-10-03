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

	if(argc > 1){
		FILE *fp;
		char str[200];
		char* filename = argv[1];

		fp = fopen(filename, "r");
		if(fp == NULL){
			printf("Could not open file %s", filename);
			return 1;
		}
		while (fgets(str, 200, fp) != NULL)
			system(str);
		fclose(fp);
		return 0;
	}

	while(strcmp(str, "exit") != 0){
		
		//begins shell prompt
		printf("%s", "ash>> ");
		chars = getline(&str, &bufsize, stdin); 
		if(/*strchr(str, '|') != NULL ||*/ strchr(str, '>') != NULL){
			system(str);
		}else{
			// splits input string by " " and "\n"
			char *pstr = strtok(str, " \n");

			//exit function
			/*if(strcmp(pstr, "exit") == 0){
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
			
			}else{*/
		while(pstr != NULL){
						

			/*int rc = fork();
			if (rc < 0) {
				//fork failed
				fprintf(stderr, "fork failed\n");
				exit(1);
			}else if(rc == 0){
			*/

			//tracks how many paths have been tested with the command
			int pc = 0;
			char *myargs[10];

			char *myargs2[10];

			//clears buff array
			memset(buff, 0, sizeof buff);

			//copy first path in pathArr to buff
			strcpy(buff, pathArr[pc]);

			//stores commands
			char comm[255] = "";

			//init is a boolean value that is set to false once the first argument is saved to comm (for path testing)
			int init = 1;

			//argC tracks each argument associated with the first command
			int argC = 1;
			
			//will become 1 if a pre-built command is issued and will then skip the fork
			int pb = 0;
			
			//pipe stuff
			int isPipe = 0;
			int fd[2];
			pipe(fd);
			
			//loops through command string and places command at the beginning and args into myargs array
			while(pstr != NULL){
				//if & is found, then execute command up to & and save execCtr to start after & in next loop through
				if(strcmp(pstr, "&") == 0){
					pstr = strtok(NULL, " \n");
					break;
				}
				else if(strcmp(pstr, "|") == 0){
					pstr = strtok(NULL, " \n");
					isPipe = 1;
					int arg2C = 0;
					while(pstr != NULL && strcmp(pstr, "&") != 0){
						myargs2[arg2C] = pstr;
						arg2C++;
						pstr = strtok(NULL, " \n");
					}
					myargs2[arg2C] = NULL;
				}
				else if(strcmp(pstr, "exit") == 0){
					exit(0);
				}
				else if(strcmp(pstr, "cd") == 0){
					pstr = strtok(NULL, " \n");
					pb = 1;
					chdir(pstr);
				}
				else if(strcmp(pstr, "path") == 0){
					pstr = strtok(NULL, " \n");
					while((pstr != NULL) || strcmp(pstr, "&") != 0){
						pathArr[pathctr] = pstr;
						pathctr++;
						pstr = strtok(NULL, " \n");
					}
					pb = 1;
				}
				else if(init == 1){
					strcat(comm, pstr);
					init = 0;
				}else{	
					myargs[argC] = pstr;
					argC++;
				}
				pstr = strtok(NULL, " \n");
				printf("pstr is now %s\n", pstr);
			}
			
			//check if comm (or arg[0]) is a command on the path
			printf("%s\n", comm);
			while(access(strcat(buff, comm), X_OK) != 0 && pc < pathctr){
				printf("%d\n", pc);
				pc++;
				memset(buff, 0, sizeof buff);
				strcpy(buff, pathArr[pc]);
			}
			
			//check if comm after pipe is a command on the path
			/*if(pc < pathctr){
				comm = arg2
				while(access(strcat(
			}*/

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
				if (pc < pathctr && pb == 0 && isPipe == 0){
					//char *myargs[2];
					myargs[0] = strdup(buff);
					myargs[argC] = NULL;
					if(execvp(myargs[0], myargs) == -1){
						char error_message[30] = "error\n";
						write(STDERR_FILENO, error_message, strlen(error_message));
						exit(1);
					}
				}else if(isPipe == 1){
					dup2(fd[1], STDOUT_FILENO);
					close(fd[0]);
					close(fd[1]);
					if(execvp(myargs[0], myargs) == -1){
						char error_message[30] = "error\n";
						write(STDERR_FILENO, error_message, strlen(error_message));
						exit(1);
					}
				}
				exit(1);
			
			}else{
				if(pc < pathctr && isPipe == 1){
				
					rc = fork();

					if(rc == 0){
						dup2(fd[0], STDIN_FILENO);
						close(fd[1]);
						close(fd[0]);
						//close(fd[READ_END]);
						if(execvp(myargs2[0], myargs2) == -1){
							char error_message[30] = "error\n";
							write(STDERR_FILENO, error_message, strlen(error_message));
							exit(1);
						}
					}else{
						close(fd[0]);
						close(fd[1]);
						int rc_wait = wait(NULL);
					}
				
				}else{
					int rc_wait = wait(NULL);
				}
			}
		}
		}
	}
	
	return 0;
}
