#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	
	char *str;
	size_t bufsize = 200;
	size_t chars;
	str = (char *)malloc(bufsize * sizeof(char));

	char *pathArr[20];
	pathArr[0] = "/usr/bin/";
	pathArr[1] = "/bin/";
	int pathctr = 2;
	
	char buff[255];

	while(strcmp(str, "exit") != 0){
		printf("%s", "ash>> ");
		chars = getline(&str, &bufsize, stdin); 

		char *pstr = strtok(str, " \n");
		if(strcmp(pstr, "exit") == 0){
			exit(0);
		}
		else if(strcmp(pstr, "cd") == 0){

			pstr = strtok(NULL, " \n");
			printf("chdir %s", pstr);

			chdir(pstr);

		}else if(strcmp(pstr, "path") == 0){
			pstr = strtok(NULL, " \n");
			while(pstr != NULL){
				
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
			strcpy(buff, pathArr[pc]);
			printf("%s", strcat(buff, pstr));
			while(access(strcat(buff, pstr), X_OK) != 0 && pc < pathctr){
				memset(buff, 0, sizeof buff);
				pc++;
				strcpy(buff, pathArr[pc]);
				printf("%d", pc);
			}
			printf("%d", pc);
			if (pc < pathctr){
				char *myargs[2];
				printf("%s", buff);
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
