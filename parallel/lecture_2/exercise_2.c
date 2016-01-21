#include <string.h> //Required for strlen
#include <stdio.h>	//Required for printf

void reverse(char *str) {
  char  *end_ptr;
  int tmp;
  /*Return if no string given*/
  if( str == NULL || !(*str) )
    return;
  /* *str points at the first character of str*/
  /* *end_ptr points to the last character in the string*/
  end_ptr = str + strlen(str) - 1;

  /* Reverse the characters */
  while( end_ptr > str ) {
    tmp = *str; //Set tmp to what str points at
    *str = *end_ptr; //Set str pointer to where end pointer points
    *end_ptr = tmp; //Set end pointer location to tmp
    str++; //Move str pointer right by 1
    end_ptr--; //Move end pointer left by 1
  }
}

int main(){
	  char str[] = "cat";
	  reverse(str);
	  printf("%s\n",str);//Print the result
    return 0;
  }
