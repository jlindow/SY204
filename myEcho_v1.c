/* Attempt #2 at myEcho */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char * argv[])
{
 int size; 
 int i;
 int errno;


 // int size = strlen(argv[1]);
  
  for(i = 1; i < argc; i++)
  {

    size = strlen(argv[i]);


    if(i == (argc - 1))
    {
      if( write(STDOUT_FILENO, argv[i], size) == -1){
         perror("Error Occured: ");
         break;
        }
      write(STDOUT_FILENO, "\n", 1);
    }



    else
    
    {
      
       if(write(STDOUT_FILENO, argv[i], size) == -1){
          perror("Error Occured: ");
          break;
          }

       write(STDOUT_FILENO, " ", 1);
    }

  }


  return(0);
}



