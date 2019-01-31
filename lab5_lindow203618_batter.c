/*
 * Name:       MIDN 3/C Lindow
 * Alpha:      203618
 * Course:     SY204 - OS Fundamentals
 * Assignment: Lab 5 - Baseball

 * Description: This program receives signals from a pitcher, and sends them to a fielder. Type -h for more detailed info.

 * Resources Used:
        - www.gnu.org
        - Mr. De Bels, NSA Visiting Professor
        - courses.cyber.usna.edu/SY204

 * Version: 1.1
    - modified printf() statements inside sighandler to write system calls
        -converted PID to a string in order to write (line 111). 

 */

/* ------- Required Includes ------- */

#include <stdio.h>     // Standard I/O: fprintf()
#include <stdlib.h>    // Standard Library: exit(0)
#include <errno.h>     // Error Checking: perror()
#include <getopt.h>    // Argument Parsing: getopt()
#include <signal.h>    // Signal Library: sigaction(2)
#include <unistd.h>    // UNIX Standard Library: getpid(2)
#include <sys/types.h> // System Type Library: pid_t
#include <string.h>    // itoa() string cast

/* ------- Global Constants & Variables ------- */
pid_t PID; // Process ID of FIELDER program
char batter; // -l or -r batter option

/* ------- Function Prototypes ------- */

void SigHand(int SigNum, siginfo_t *sigInfo, void *SysDependent); //Receive and Transmit Signals
char GetOption(int argc, char* argv[]);
pid_t getpid();

/* ---------------- Main Function --------------- */
/* Description: This program prints its current PID
   to the screen, and then receives signals being
   sent to that process ID from a pitching program.
   Depending upon command line argument options,
   and the signal sent from the pitcher, this program will
   send back to the fielder program, whose process ID is specified
   as an argument, either SIGUSR1 or SIGUSR2 */

int main(int argc, char* argv[])
{
  //Bad number of arguments given
    if (argc == 1 || argc > 3)
    {
      printf("Please type < ./batter -h > for usage information.\n");
      exit(0);
    }
  // Variables Declaration
    pid_t batter_pid; // process ID type

  //Get options
    batter = GetOption(argc, argv);

  //Get current process ID
    batter_pid = getpid();
    printf("BATTER(%d): Ready to bat!\n", batter_pid); // Call the pitcher program using this PID

  //Establish sigaction structure
    struct sigaction handler;
    handler.sa_sigaction = &SigHand; //Memory address of signal handler function
    handler.sa_flags = SA_SIGINFO | SA_RESTART; //restart interrupted system calls

  //Determine PID to send to
    if (argc == 2)
    {
      PID = (pid_t) atoi(argv[1]); // no batter option given, set PID to second command line argument
      batter = 'x';
    }

    if (argc == 3) // Batter option given as argv[1], set PID equal to third command line argument
    {
      PID = (pid_t) atoi(argv[2]);
    }


  //Call Signal Handler for SIGUSR1 & SIGUSR2
    while (1)
    {
      sigaction(SIGUSR1, &handler, NULL); // handler is name of sigaction structure (see above)
      sigaction(SIGUSR2, &handler, NULL);
    }

  //End of Main Function
    return(0);
}



/* ---------------- SIGNAL HANDLING FUNCTION ------------------ */
/* Description:  This function handles signals sent from a given
   pitching program, and sends them to a fielder program, with
   process ID 'PID'. The function returns nothing, and sends
   either SIGUSR1 or SIGUSR2, depending upon the received signal,
   declared as SigNum. */

void SigHand(int SigNum, siginfo_t*sigInfo, void *SysDependent)
{
  //get pitcher PID
    pid_t pitcher_pid = (sigInfo -> si_pid);

  //Convert PID to string
    char pid_string[5];
    sprintf(pid_string, "%d", PID);

  //Batter option given as right handed or no option
    if (batter  == 'r' || batter == 'x')
    {
      if (SigNum == SIGUSR1)
      {
        //Send SIGUSR1 to PID
          write(1, "Fly Ball to: ", 13);
          write(1, pid_string, 5);
          write(1, "\n", 1);
          kill(PID, SIGUSR1);

        //Send SIGUSR1 to process from which SIG was received
          kill(pitcher_pid, SIGUSR1);
      }

      if (SigNum == SIGUSR2)
      {
        //Send SIGUSR2 to PID
          write(1, "Ground Ball to: ", 16);
          write(1, pid_string, 5);
          write(1, "\n", 1);
          kill(PID, SIGUSR2);

        //Send SIGUSR1 to process from which SIG was received
          kill(pitcher_pid, SIGUSR1);
      }

    }

  //Batter option given as left handed
    if (batter == 'l')
    {
      if (SigNum == SIGUSR1)
      {
        //Send SIGUSR1 to PID
          write(1, "Ground Ball to: ", 15);
          write(1, pid_string, 5);
          write(1, "\n", 1);
          kill(PID, SIGUSR1);

        //Send SIGUSR1 to process from which SIG was received
          kill(pitcher_pid, SIGUSR1);
      }

      if (SigNum == SIGUSR2)
      {
        // Send SIGUSR2 to PID
          write(1, "Fly Ball to: ", 13);
          write(1, pid_string, 5);
          write(1, "\n", 1);
          kill(PID, SIGUSR2);

        //Send SIGUSR1 to process from which SIG was received
          kill(pitcher_pid, SIGUSR1);
      }

    }
}


/* ---------------------- Option Parsing Function -------------------- */
/* Description: Returns a character, either l, r, or x. If l is returned,
   a left handed batter was specified  by command argument, if r is returned,
   a right handed abtter was specified by command argument, and if x is returned   no command line option  argument was given */

char GetOption(int argc, char* argv[])
{
  //variable declaration
    int opt;

  /* Iterate through possible options of 'r' 'h' or 'l'.
     No options have mandatory arguments, but program
     will terminate after displaying a help message
     if -h is given */
    while ((opt = getopt(argc, argv, "rhl")) != -1)
    {
      //Display help message and terminate program
        if (opt == 'h')
        {

          printf("\nName:        Lab 5 - Baseball (Batter)\n\n");
          printf("Usage:       < $ ./batter (-l or -r) SPACE (some PID) > \n\n");
          printf("Example:     < $ ./batter -l 12345 >\n\n");
          printf("Description: This program takes the process ID of the fielder program\n             and a possible switch hitter option, either -l or -r.\n             The program will display the current PID which should be given to the pitching program as an argument.\n             The program will receive pitches from the pitcher, and simply bat either fly ball or ground ball to the fielder.\n             In the case of Nolan Ryan, the program will also send SIGUSR1 to the pitcher, for some reason unknown.\n\n");

          exit(0);
        }
     //Otherwise, simply return opt
        else
        {
          return(opt);
        }
    }

}
