/* 
  Name:    MIDN 3/C Lindow
  Alpha: 203618
  Assignment:  Lab 0x02 - ls_hiding
  Course:  SY204 - OS Fundamentals
  Section: 1121
  Instructor: Mr. Mark De Bels

  Description: This program hides a designated file from the user, copies the contents of the file to a newly created file.  
 
*/

// Required Includes //
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Global Constants //
#define BUF_LEN 1024


// Function Prototypes // 
int readnwrite(int fd, int fd2);
void getoption(int argc, char*argv[]);

// Main Function //
int main(int argc, char *argv[])
{
  //Declaration of required variables //
  int intPause = 0; // Variable to force pausing point within program
  int fd; // File descriptor
  int i; // For use in for and while loops
  char buffer[BUF_LEN]; // Sring to read into
  int fd2; //File descriptor


  //Check for no user argument
  if(argc < 2)
  {
    printf("This program hides a file from view and copies its contents to a new file named <newfile.txt> To use, type < ./hide filename > where filename is the name of the file you would like to hide.\n");
    exit(0);
  }

  //Check for -h option argument. 
  getoption(argc, argv);
  
  // Use Descriptor I/O to open specified file from command line argument and read one byte
  fd = open(argv[1], O_RDONLY);
  read(fd, buffer, 1);

  /* 
     * Begin Hard Coded Pause Point: 1
  */

  fprintf(stdout, "Pause Point: File appears in ls listing\n");
  scanf("%d", &intPause); // Read in integer as forced pause point
  // 1. Use CTRL - Z to pause the program. Program is paused at the scanf(3) call above. 2
  // 2. Use ls -a to see the file within the ls listing. 
  // 3. Use fg 1 (assumes this program is the only paused program)
  // 4. Enter an integer to proceed. Enter data via Standard Input to complete the scanf(3) call 

  /* 
   * End Hard Coded Pause Point: 1
   */


  // REMOVE i-NODE ENTRY FOR THE SPECIFIED FILE
  if( unlink(argv[1]) < 0) 
  { 
    perror("Error Occured");
  }

  /* 
     *Begin Hard Coded Pause Point: 2
  */

  fprintf(stdout, "Pause Point: File does not appear in ls listing but is still open\n");
  scanf("%d", &intPause); // Read in integer as forced pause point
  // 1. Use CTRL - Z to pause the program. The program is paused at the scanf(3) call above. 
  // 2. Use ls - a to see the file is not in the ls listing 
  // 3. Use fg 1 (assume the program is the only paused program) 
  // 4. Enter an integer to proceed. Enter data via standard input to complete the scanf(3) call. 

  /* 
   * End Hard Code Pause Point: 2
   */

  // Reset buffer offset to beginning of file 
  lseek(fd, 0, SEEK_SET);

  fd2 = open("newfile.txt", O_CREAT | O_WRONLY, 0744); // open a new file for read, write, and execute
  readnwrite(fd, fd2); // read the contents of the original specified file (file descriptor fd), and write to the new file (fd2). 
  close (fd); // close the files
  close (fd2); 

  // Return to Caller 
  return (0); 

}

// This function reads the first argument, and writes it to the second argument.
int readnwrite(int fd, int fd2)
{
  int length = 1; 
  char buffer[BUF_LEN];

  while(length > 0)
  {
    //Read in contents of fd to bufer
    length = read(fd, buffer, BUF_LEN);

    //Write to temp file
    if( write (fd2, buffer, length) < 0)
    {
      perror("Error occured: ");
    }

  }
  return(0);
}

void getoption(int argc, char *argv[]) 
{
  int opt; 
  
  while( (opt = getopt(argc, argv, ":h")) != -1)
  {
    if (opt == 'h') 
    {
      printf("This program will remove a file from the I-node table, and write its contents to a new file named <newfile.txt>. To use, enter < ./hide filename> where filename is the name of the file you would like to hide.\n");
      exit(0);
     }
   }
}
