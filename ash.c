#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
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
		while (fgets(str, 200, fp) != NULL){
			system(str);
		}
		fclose(fp);
		return 0;
	}

	while(strcmp(str, "exit") != 0){
		
		//begins shell prompt
		printf("%s", "ash>> ");
		chars = getline(&str, &bufsize, stdin);
		// splits input string by " " and "\n"
		char *pstr = strtok(str, " \n");

		while(pstr != NULL){

			//tracks how many paths have been tested with the command
			int pc = 0;
			char **args[5];
			char *myargs[10];
			char *myargs2[10];
			char *myargs3[10];
			char *myargs4[10];
			char *myargs5[10];

			args[0] = myargs;
			args[1] = myargs2;
			args[2] = myargs3;
			args[3] = myargs4;
			args[4] = myargs5;

			//clears buff array
			memset(buff, 0, sizeof buff);

			//copy first path in pathArr to buff
			strcpy(buff, pathArr[pc]);

			//stores commands
			char comm[255] = "";
			char outp[100] = "";

			//init is a boolean value that is set to false once the first argument is saved to comm (for path testing)
			int init = 1;
			int red = 0;

			//argC tracks each argument associated with the first command
			int argC = 1;
			int arrC = 0;
			
			//will become 1 if a pre-built command is issued and will then skip the fork
			int pb = 0;
			
			//pipe stuff
			int numPipes = 0;
			int isPipe = 0;
						
			//loops through command string and places command at the beginning and args into myargs array
			while(pstr != NULL){
				//if & is found, then execute command up to & and save execCtr to start after & in next loop through
				if(strcmp(pstr, "&") == 0){
					pstr = strtok(NULL, " \n");
					myargs[argC] = NULL;
					break;
				}
				if(strcmp(pstr, ">") == 0){
					red = 1;
					pstr = strtok(NULL, " \n");
					strcat(outp, pstr);
					pstr = strtok(NULL, " \n");
					break;
				}
				else if(strcmp(pstr, "|") == 0){
					args[arrC][argC] = NULL;
					arrC++;
					argC=0;

					numPipes++;
					pstr = strtok(NULL, " \n");
					isPipe = 1;
					while(pstr != NULL && strcmp(pstr, "&") != 0 && strcmp(pstr, ">") != 0){
						if(strcmp(pstr, "|") == 0){
							args[arrC][argC] = NULL;
							arrC++;
							argC=0;
							numPipes++;
							pstr = strtok(NULL, " \n");
						}
						args[arrC][argC] = pstr;
						argC++;
						pstr = strtok(NULL, " \n");

					}if(pstr != NULL && strcmp(pstr, ">") == 0){
						red = 1;
						pstr = strtok(NULL, " \n");
						strcat(outp, pstr);
					}

					args[arrC][argC] = NULL;
					if(pstr != NULL)
						pstr = strtok(NULL, " \n");
					if(pstr != NULL && strcmp(pstr, "&") == 0)
						pstr = strtok(NULL, " \n");
					break;
				}else if(strcmp(pstr, "exit") == 0){
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
					args[0][argC] = NULL;
				}else{
					args[arrC][argC] = pstr;
					argC++;
					args[arrC][argC] = NULL;
				}
				pstr = strtok(NULL, " \n");
			}
			
			//check if comm (or arg[0]) is a command on the path
			while(access(strcat(buff, comm), X_OK) != 0 && pc < pathctr){
				pc++;
				memset(buff, 0, sizeof buff);
				strcpy(buff, pathArr[pc]);
			}
			if(pc < pathctr)
				args[0][0] = strdup(buff);
			else
				printf("%s\n", "command is not on path");
			
			int rc = fork();
			if (rc < 0){
				//fork failed
			fprintf(stderr, "fork failed\n");
				exit(1);
			}

			if (rc == 0){


			//if a path was found for the current argument
				if (pc < pathctr && pb == 0 && isPipe == 0){
					if(red = 1){
						int fw = open(outp, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
						dup2(fw, 1);
						close(fw);
					}
					if(execvp(args[0][0], args[0]) == -1){
						char error_message[30] = "error\n";
						write(STDERR_FILENO, error_message, strlen(error_message));
						exit(1);
					}
				}
				exit(1);
			}else{ 
				if(pc < pathctr && isPipe == 1 && pb == 0){
					int fd[2];
					int fdd = 0;
					for(int i=0; i<numPipes+1; i++){
						if(i != numPipes){
							pipe(fd);
						}
						if((rc = fork()) == -1){
							perror("fork");
							exit(1);
						}else if(rc == 0){
							dup2(fdd, 0);
							if(i != numPipes){
								dup2(fd[1], 1);
							}else{
								if(red == 1){
									int fw = open(outp, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
									dup2(fw, 1);
									close(fw);
								}
							}
							close(fd[0]);
							if(execvp(args[i][0], args[i]) == -1){
								char error_message[30] = "error1\n";
								write(STDERR_FILENO, error_message, strlen(error_message));
								exit(1);
							}
						}else{
							int rc_wait = wait(NULL);
							close(fd[1]);
							fdd = fd[0];
						}
					}
				}
			}
			int rc_wait = wait(NULL);
		}
	}
	return 0;
}
