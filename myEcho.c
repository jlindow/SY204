/* 
    Author: MIDN Jacob Lindow
         "my Echo" 
         
    This program functions
   similar to the standard 
   terminal command 'echo'
   and will echo user input
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char * argv[])
{
 int i = 0;
 int errno = 0;
  
  for(i = 1; i < argc; i++) {
    if(i == (argc - 1)) {
      if( write(STDOUT_FILENO, argv[i], strlen(argv[i])) == -1) {
         perror("Error Occured: ");
         break; }    
      write(STDOUT_FILENO, "\n", 1); 
    } else {
       if(write(STDOUT_FILENO, argv[i], size) == -1) {
          perror("Error Occured: ");
          break; } 
       write(STDOUT_FILENO, " ", 1); }
    }
    
  return(0);
}



