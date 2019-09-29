#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//Builtin command functions exit, cd and path
int ashexit(char **args);
int ashcd(char **args);
int ashpath(char **args);

//Builtin commands from bash
char *builtin_command[] = {"exit","cd"};

//Builtin scripts
int (*builtin_script[]) (char **) = {&ashexit, &ashcd};

//Number of builtins
int ash_count_of_builtins() {return sizeof(builtin_command)/sizeof(char *);}

//Builtin script exit
int ashexit(char **args) { return kill(getpid(),SIGKILL);}

//Builtin script cd
int ashcd(char **args) {

   if (args[1] == NULL) {
      fprintf(stderr, "cd error\n");}
   else {
   if (chdir(args[1]) != 0) {
      perror("ash");

   }
   }
   return 1;
}

int runbuiltins(char **args)
{
   int i;

   if (args[0] == NULL) {
      //No command
      return 1;
   }

   for (i=0; i< ash_count_of_builtins(); i++) {
      if (strcmp(args[0], builtin_command[i]) == 0) {
         return (*builtin_script[i])(args);
      }
   }
   return 2;
}

void setup_cmdline()
{
   char buffer[100];
   char * cwd;
 
//Get the current working directory
   if ((cwd = getcwd(buffer,100)) != NULL)
   {printf(">ash$ ");

   }else{
     perror("getcwd error");

   }
}   

int main(int argc, char **argv[]){

char *input_line = NULL; //input to command line
pid_t rc; //return code
pid_t rc_wait; //return code
ssize_t buffersize; //buffer for getline
char **ap;
int state;
int buffersize2;
int place;
char *token;
int loopcode = 0;
char copy[100];
int linelen;
const char pipe = '|';
const char amp = '&';
char *temp;

//Add while loop while !exit()
while (1) {

//Ash command line prompt
setup_cmdline();

//Read in command line input. 
//Need to handle batch mode i.e. not ash> but ./ash batch.txt
buffersize = 0;
linelen = getline(&input_line, &buffersize, stdin);

//Parse a string containing tokens delimited by white space into an argument vector
buffersize2 = 64;
place = 0;
ap = malloc(buffersize2 * sizeof(char*));

if (!ap) {
   char error_message[30] = "An error has occurred\n";
   write(STDERR_FILENO,error_message,strlen(error_message));
   exit(1);
}

token = strtok(input_line, " \t\r\n\a");
while (token != NULL) {
   ap[place] = token;
   place++;

  if (place >= buffersize2) {
     buffersize2 += 64;
     ap = realloc(ap, buffersize2 * sizeof(char*));
     if (!ap){
       char error_message[30] = "An error has occurred\n";
       write(STDERR_FILENO,error_message,strlen(error_message));
       exit(1);
     }
   }
  token = strtok(NULL, " \t\r\n\a");
}
ap[place] = NULL;

loopcode = runbuiltins(ap);

//This is a not one of the builtins, go to the binaries

if (loopcode != 1) {

   int last;
   int count = 0;
   int found = 0;
   char temp;

//   printf("%d\n",found);
   for(count =0;count <linelen-1; count++){
      temp =input_line[count];
    /*  printf("%s\n",input_line);*/
      if(temp == '&' || temp == '|' || temp == '>')
        {/*printf("%c\n",temp);*/
         found = 1;
         system(input_line);
}
      }

if (found == 0)
{

rc = fork();
if (rc == 0) {
   //child process
   if (execvp(ap[0], ap) == -1){
         char error_message[30] = "error\n";
         write(STDERR_FILENO,error_message,strlen(error_message));
   }
   exit(1);}
else if (rc <0) {//fork failed; exit
       char error_message[30] = "error - fork\n";
       write(STDERR_FILENO,error_message,strlen(error_message));}
else {
  //parent process
  do {
     rc_wait = waitpid(rc,&state, WUNTRACED);
     } while (!WIFEXITED(state) && !WIFSIGNALED(state));
}
}

}

   } //End  the main while loop

exit(0);
//free(input_line);
//free(ap);
return 0;
}
