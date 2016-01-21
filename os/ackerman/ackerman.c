#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/******************************************
 * https://en.wikipedia.org/wiki/Ackermann_function
 *
 * This example implements the Ackermann function as
 * described in the link above.
 * Author: Cameron A. Craig
 *****************************************/

int ackermann(int m, int n){
  if(m == 0) {
		n+= 1;
		return n;
	}
	if(m > 0 && n == 0) ackermann(m-1, 1);
	if(m > 0 && n > 0) ackermann(m-1, ackermann(m, n-1));

}
int main(int argc, char * argv[]){

	int m,n;
	if(argc == 3) {
		m = atoi(argv[1]);
		n = atoi(argv[2]);

	}else{
		m = 0;
		n = 0;
	}
	printf("Running ackermann function with m = %i and n = %i\n", m, n);
	int r = ackermann(m, n);
	printf("Result: %i \n", r);
	return 0;
}
