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
		
		char *pstr = strtok(str, " ");
		
		if(strcmp(pstr, "exit\n") == 0)
			exit(0);

		while(pstr != NULL){
		
			printf("%s ", pstr);
			pstr = strtok(NULL, " ");

		}

		//handles exit functionality
		if(strcmp(str, "exit") == 0)
			exit(0);

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
