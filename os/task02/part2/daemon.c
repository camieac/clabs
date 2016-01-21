#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

int printLocalTime(){
	FILE * file;
	while(1){
		time_t curtime;
		time(&curtime);
		printf("Current time = %s", ctime(&curtime));
		file = fopen ("/tmp/logfile.txt", "a");
		fprintf(file, "%s", ctime(&curtime));
		fclose(file);
		sleep(5);
	}
	return 0;
}

int main(){
	
	pid_t pid, sid; //Process ID and Session ID
	
	/** The fork() function is used to create a new process by duplicating
	 * the existing process from which it is called. The existing process
	 * from which this function is called becomes the parent process and
	 * the newly created process becomes the child process. As already
	 * stated that child is a duplicate copy of the parent but there are
	 * some exceptions to it.
	 * @see http://www.thegeekstuff.com/2012/05/c-fork-function/
	 */ 
	pid = fork();	//fork off from the parent process
	
	if(pid < 0){	//check if PID not created
		exit(EXIT_FAILURE);
	}
	if(pid > 0){	//If the fork is succesful, exit from the parent process.
		exit(EXIT_SUCCESS);
	}
	
	umask(0);
	
	/** umask() sets the calling process's file mode creation mask (umask)
	 * to mask & 0777 (i.e., only the file permission bits of mask are used),
	 * and returns the previous value of the mask.
	 * 
	 * A child process created via fork(2) inherits its parent's umask.
	 * The umask is left unchanged by execve(2).
	 * @see http://linux.die.net/man/2/umask
	 */ 

	sid = setsid();	//Create a new session id for the child process
	if(sid < 0){	// If the SID failed to be created, exit.
		exit(EXIT_FAILURE);
	}
	if(chdir("/") < 0){	//Set the working directory to the root.
		exit(EXIT_FAILURE);
	}
	
	//Close all standard file descriptors.
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	//Run the local time function, will return on error.
	printLocalTime();
	return 0;
}

