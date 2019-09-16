#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	
	char str[100];
	
	while(strcmp(str, "exit") != 0){
		int rc = fork();
		if (rc < 0) {
			//fork failed
			fprintf(stderr, "fork failed\n");
			exit(1);
		} else if(rc == 0){

			printf("ash>> ");
			scanf("%s", str);
			
			char *myargs[3];
			myargs[0] = strdup("exit");
			myargs[1] = NULL;
			execvp(myargs[0], myargs);

		}else{
	
			int rc_wait = wait(NULL);

		}

	}

	return 0;

}
