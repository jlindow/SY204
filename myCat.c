/*   myCat Program  */
/* MIDN 3/C Lindow */
/*      SY204     */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#define BUF_LEN 1024

int main(int argc, char *argv[])
{
  getfiles(argc, argv);

  return(0);
}

int getfiles(int argc, char *argv[])
{
  int i;
  int fdr;
  int length;
  char buffer[BUF_LEN];

  for(i = 1; i < argc; i++)
  {

     fdr = open(*argv[i], O_RDWR);
     if(fdr < 0){
       perror("Error Occured: ");
     }

     length = read(fdr, buffer, BUF_LEN);
     if(length < 0){
        perror("Error Occured: ");
     }

     if( write(STDOUT_FILENO, fdr, length) < 0 ) {
        perror("Error Occured: ");
     }

     if(argc > 1){
        write(STDOUT_FILENO, "\n\n", 2);
     }

  }

  return(0);
}

