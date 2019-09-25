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
	pathArr[0] = "/usr/bin";
	int pathctr = 1;

	while(strcmp(str, "exit") != 0){
		printf("%s", "ash>> ");
		chars = getline(&str, &bufsize, stdin); 

		char *pstr = strtok(str, " \n");
		if(strcmp(pstr, "exit") == 0)
			exit(0);

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
			for(int i=0; i<pathctr; i++)
				printf("arr[%d] is %s", i, pathArr[i]);
		}else{
	
		int rc = fork();
		if (rc < 0) {
			//fork failed
			fprintf(stderr, "fork failed\n");
			exit(1);
		}else if(rc == 0){
				
			char *myargs[2];
			myargs[0] = strdup(str);
			myargs[1] = NULL;
			execvp(myargs[0], myargs);

		}else{
			printf("exit? %s", pstr);
			int rc_wait = wait(NULL);
		}
		}

	}
	return 0;

}
