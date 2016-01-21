#include <time.h>
#include <stdio.h>

int main(){
	FILE * fp;
	while(1){
		time_t curtime;
		time(&curtime);
		printf("Current time = %s", ctime(&curtime));
		fp = fopen ("/tmp/logfile.txt", "a");
		fprintf(fp, "%s", ctime(&curtime));
		fclose(fp);
		sleep(5);
	}
	return (0);
}
