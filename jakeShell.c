/* 
    Name: MIDN 3/C Lindow
    Alpha: 203618
    
    Source: The majority of this code was written by the SY204 instructors at USNA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include <sys/stat.h>


#define STANDARD_PATH "/bin:/usr/bin:/usr/local/bin"
#define PROMPT "jakeShell: "
#define MAX_BUF 1000

#define NOPIPE 0
#define YESPIPE 1
#define DEFAULT 0
#define LEFTSIDE 1
#define RIGHTSIDE 2

void runCommand(struct command_t command, int *fdPipe, int pipeExists, int pipeSide);

int main(int argc, char *argv[]) {
   char *existingPath = NULL;
   char commandLine[MAX_BUF];
   struct command_t command; // for parsing the command line
   pid_t pid;
   int i, numRead;
   int fdPipe[2];

   // no output buffering on stdout
   setbuf(stdout,NULL);

   // secure the PATH; if no PATH set, then set it now
   existingPath = getenv("PATH");
   if (existingPath == NULL) {
      if (setenv("PATH",STANDARD_PATH,0) < 0) errExit("setenv",EXIT);
   }

   // loop until ctrl-C quits
   while (1) {
      // prompt for a command
      if (write(STDOUT_FILENO,PROMPT,strlen(PROMPT)) == -1) errExit("write",EXIT);
      // read in the entire line (no more than MAX_BUF in length though)
      if ((numRead = read(STDIN_FILENO,commandLine,MAX_BUF)) == -1) errExit("read",EXIT);
      commandLine[numRead-1] = '\0';

      for (i = 0; /* no test */ ; i++) { // for running multiple commands on one line
         // get the next command from the current command line
         command = parseCommandLine(commandLine);
         if (command.argc == 0) break; // no command to execute -- try again

         // check if this command starts a pipe
         if (command.pipe2next) { // yes -- two commands needed
            int returned = pipe(fdPipe); 
            if(returned == -1) return 1; //error check
        
            // fork a child process to run the command on the left side of the pipe
            if ((pid = fork()) == -1) errExit("fork",EXIT);
            if (pid == 0) { // child runs the current command
            runCommand(command, fdPipe, 1, 1);

            }

            // get the next command from the current command line
            command = parseCommandLine(commandLine);
            if (command.argc == 0) errExit("no command for pipe",EXIT); // no command found!

            // fork second process for command on right side of the pipe
            if ((pid = fork()) == -1) errExit("fork",EXIT);
            if (pid == 0) {
            runCommand(command, fdPipe, 1, 1);
  
            }
            // parent waits for both commands, and closes pipe ends
            if (close(fdPipe[0]) == -1) errExit("close",EXIT);
            if (close(fdPipe[1]) == -1) errExit("close",EXIT);
            wait(NULL);
            wait(NULL);
         } else { // no pipe found
            // fork a child process to run the current command
            if ((pid = fork()) == -1) errExit("fork",EXIT);
            if (pid == 0) { // child runs the current command
               runCommand(command,fdPipe,NOPIPE,DEFAULT);
            }
            // parent waits for one command
            wait(NULL);
         }
      }
   }

   return EXIT_SUCCESS;
}


void runCommand(struct command_t command, int *fdPipe, int pipeExists, int pipeSide) {
   int fdIn, fdOut, flags, mode; // for I/O redirection

   if (pipeExists) {
      if (pipeSide != RIGHTSIDE) { // left side of pipe (close the read end)

         close(fdPipe[1]); //close left side of pipe

         //connect stdout to write end of the pipe, and then close the
         //fd to the write end
         fdOut = fdPipe[0];  
         close(fdPipe[0]); 
          

      } else { // right side of pipe (close the write end)
         //close the right end of the pipe
         close(fdPipe[0]);
      
         // connect stdin to read end of the pipe, and then close
         // the fd to the read end
         fdIn = fdPipe[1]; 
         close(fdPipe[1]);

      }
   }

   if (command.redirectIN) { // redirect input from command.infile
      flags = O_RDONLY | O_CLOEXEC;
      // open the infile with the correct flags
      int fd = open(command.infile, flags);
      if (fd < 0) perror("Error opening"); 

      // make stdin a dup of the input file descriptor just opened
      int returned = dup2(fd, fdIn);
      if (returned == -1) perror("Error dup'ing");

   }

   if (command.redirectOUT) { // redirect output to command.outfile
      flags = O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC;

      // set the file permissions mode to rw-rw-rw-
      chmod(command.outfile, 0666);

      // open the outfile with the correct flags and mode
      int fd = open(command.outfile, flags); 
      if (fd < 0) perror("Error opening"); 

      // make stdout a dup of the output file descriptor just opened
      int returned = dup2(fd, fdOut); 
      if  ( returned == -1 ) perror("Error opening"); 

   }

   // exec the command with I/O redirection
   execvp(command.cmd,command.argv);
   fprintf(stderr,"execvp failed on command %s\n",command.argv[0]);
   exit(EXIT_FAILURE); // child should exit now
}
