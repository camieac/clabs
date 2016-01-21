#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[])
{

	/*************Create Pipe*************/
  int p1[2];
  pid_t pid = 0;

  if (pipe(p1) == -1)
    printf("failed to create first pipe");
    
	/*************Fork Process **************
	@see http://en.wikipedia.org/wiki/Fork_%28system_call%29/
	*******************************************/
  if ((pid = fork()) < 0) 	//Assigns the process a PID number.
   printf("failed to fork first time");
    
	/************** Execute Process **********/
  if (pid == 0) //if we are in the child process
  {
    dup2(p1[1], STDOUT_FILENO); //Copy p1[1] file descriptor into standard out
    close(p1[0]);
    close(p1[1]);
    execlp("ps","ps",argv[1], (char *)0);
    printf("failed to exec 'ps'");
  }
  else //if we are in the parent process
  {
    dup2(p1[0], STDIN_FILENO);
    close(p1[0]);
    close(p1[1]);
    dup2(p1[1], STDOUT_FILENO); //copy pi[1] into standard out
    execlp("wc", "wc", (char *)0);
    printf("failed to exec 'wc'");
  }
  return 0;
}
