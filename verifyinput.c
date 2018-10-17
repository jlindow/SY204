/* Verify Input Function */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

int verifyinput(char input[]);
char * translate(char character);

int main()
{
  char input[51];
  char message[6];
  int i;
  char character;
  int return_value;

  printf("Please enter a string of letters, numbers, or spaces: ");
  fgets(input, 51, stdin);

  while (1){
  return_value = verifyinput(input);

  if(return_value == 0) {
    printf("Invalid input. Try again\n");
    fgets(input, 51, stdin); }

  if(return_value == 1) {
    printf("Message successfully entered\n");
    break; }
  }

  for(i = 0; i < (strlen(input) - 1); i++) {
     character = input[i];
     char morse_code[6];

     strcpy(morse_code, translate(character));
     printf("%s\n", morse_code);
    }



  return(0);
}

int verifyinput(char input[])
{
  int i;
  for(i = 0; i < (strlen(input) - 1); i++)
  {
    if(isalnum(input[i]) == 0) { // i.e. NOT a letter or number

      if(input[i] != ' ') { // NOT A SPACE

        if(input[i] != "\0") { // NOT a null - byte

           return(0); } } }
  }

  return(1);
}


char* translate(char character)
{
    char morse_code[6];
    char letter;
    int i;

    char letters[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    char mletters[26][5] = {"01","1000","1010", "100", "0", "0010", "110", "0000", "00", "0111", "101", "0100", "11", "10", "111", "0110", "1101", "010", "000", "1", "001", "0001", "011", "1001", "1011", "1100"};
    char mnumbers[10][6] = {"11111", "01111", "00111", "00011", "00001", "00000", "10000", "11000", "11100", "11110"};

    if(isalpha(character) > 0)
    {
      letter = toupper(character);

      for(i = 0; i < 26; i++)
      {
        if(letter == letters[i]) {
          strcpy(morse_code, mletters[i]);
          char* morse_tmp = strdup(morse_code);
          return(morse_tmp);
          }
      }

    }

     if(isdigit(character) > 0) {
       int i = character - '0';
       strcpy(morse_code, mnumbers[i]);
       char* morse_tmp = strdup(morse_code);
       return(morse_tmp);
     }

    if(character == ' ')
    {
     strcpy(morse_code, " ");
     char* morse_tmp = strdup(morse_code);
     return(morse_tmp);
    }

}
