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

	while(strcmp(str, "exit") != 0){
		
		printf("%s", "ash>> ");
		chars = getline(&str, &bufsize, stdin);
		
		char *pstr = strtok(str, " \n");
		
		if(strcmp(pstr, "exit") == 0)
			exit(0);

		if(strcmp(pstr, "cd") == 0){

			while(pstr != NULL){
				
				printf("%s is current str", pstr);
				pstr = strtok(NULL, " \n");
				chdir(pstr);

			}
		}
		
		//create check if 'built-in' command used, then program will not fork. Else, command run in child does not execute in parent.
		int rc = fork();
		if (rc < 0) {
			//fork failed
			fprintf(stderr, "fork failed\n");
			exit(1);
		} else if(rc == 0){

			char *myargs[2];
			myargs[0] = strdup(str);
			myargs[1] = NULL;
			execvp(myargs[0], myargs);

		}else{
	
			int rc_wait = wait(NULL);

		}

	}

	return 0;

}
