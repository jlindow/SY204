/************************************************************************************
 *   Name: MIDN 3/C Lindow                                                          *
 *   Alpha: 203618                                                                  *
 *   Instructor: Mr. De Bels                                                        *
 *   Course: SY204 - OS Fundamentals                                                *
 *                                                                                  *
 *   Assignment: Lab 3 - Untouchables                                               *
 *   Description: This program appends a file without modifying its timestamps      *
 *                                                                                  *
 ************************************************************************************/



/* ---------------------------------- Required Includes ---------------------------------------- */

  #include <stdio.h>
  #include <stdlib.h>
  #include <fcntl.h>
  #include <sys/stat.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <string.h>
  #include <ctype.h>
  #include <errno.h>
  #include <getopt.h>

/* ---------------------------------------------------------------------------------------------- */



/* ---------------------------------- Function Prototypes --------------------------------------- */

    void getoption(int argc, char* argv[]);

/* ---------------------------------------------------------------------------------------------- */




/* ------------------------------------- Main Function ------------------------------------------ */

int main(int argc, char* argv[])                                             
{   

  /************************** Declare Required Variables **************************/                                               
       int fd;                                                                      
  /********************************************************************************/


  /* _____________________________________________________________________________ */
  /* _____________________________________________________________________________ */



  /**************************** Check for no user argument *************************/
       if(argc < 2) 
           {
               printf("To use this program, type <./untouch filename.txt\n >");
               exit(0);
           }  
  /*********************************************************************************/


  /* _____________________________________________________________________________ */
  /* _____________________________________________________________________________ */



  /************************** Check for -h argument ********************************/ 
      getoption(argc, argv);    
  /*********************************************************************************/


  /* _____________________________________________________________________________ */
  /* _____________________________________________________________________________ */



  /***************************  Open or Create File ********************************/
  
      struct timespec before[2]; //initialize before[] as a timespec structure. 
      struct stat file;
        
      fd = open(argv[1], O_APPEND | O_WRONLY); //Open argv[1] to write only.
      

      if(fd < 0) // If file does not exist, create the file.
        {
            fd = open(argv[1], O_CREAT | O_APPEND | O_WRONLY, 0600);
            stat("..", &file); // get file time data from current working directory
        }

      else
        {
            stat(argv[1], &file); //get time data for specified file
        }

      
  /*********************************************************************************/


  /* _____________________________________________________________________________ */
  /* _____________________________________________________________________________ */



  /************************ Load Structure with Current Times **********************/

      before[0].tv_sec = file.st_atime; //access time to the second
      before[0].tv_nsec = file.st_atim.tv_nsec; // access time to the nanosecond

      before[1].tv_sec = file.st_mtime; // access time to the second
      before[1].tv_nsec = file.st_mtim.tv_nsec; //access time to the nanosecond


  /*********************************************************************************/


  /* _____________________________________________________________________________ */
  /* _____________________________________________________________________________ */



  /******************************* Append the File *********************************/

      if( write(fd, "Lindow was here\n", 16) < 0)
         {
            perror("Writing Error Occured");
            exit(0);
         }

  /*********************************************************************************/


  /* _____________________________________________________________________________ */
  /* _____________________________________________________________________________ */
  


  /***************************** Change access time ********************************/

      futimens(fd, before); // change file timestamps for file descriptor 'fd'
      close(fd);
  /********************************************************************************/

}



/* ------------------------------------------------------------------------------------------- */

void getoption(int argc, char* argv[])
{
  int opt; 
  while( (opt = getopt(argc, argv, ":h")) != -1)
  {
    if(opt == 'h') 
    {
      printf("This program will append a file without modifying the access or modification timestamps. To use, type <./untouch filename.txt>\n");
      exit(0); 
    }

  }

}

/* ------------------------------------------------------------------------------------------- */


// END OF PROGRAM

