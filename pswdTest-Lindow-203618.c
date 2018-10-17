//*****************************************//
//**********MIDN 3/C Lindow***************//
//********SY204 Section 1121*************//
//****Password Complexity Program*******//
//*************************************//

//****************************//
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//***************************//


//Main Function//

int main()
{

  //Variable Declaration//
  int min_length, min_upper, min_lower, min_dec, min_spec;
  char pass1[100], pass2[100];


  //Read in password requirements//
  read_in(&min_length, &min_upper, &min_lower, &min_dec, &min_spec);


  //Read in a user's password//
  printf("\nPlease enter a password: ");
  scanf("%s", pass1);

  printf("\nPlease re-enter your password: ");
  scanf("%s", pass2);


  //Verify passwords are the same//
  if (strcmp(pass1, pass2) != 0)
  {
    printf("Passwords did not match. Program Terminating\n");
    exit(0);
  }


  //Verify password does not contain illegal characters//
  bad_character_check(pass1);


  //Verify password satisfies the minimum number of special characters//
  min_char_countr(pass1, min_spec);


  //Verify the password complexity//
  verify_complexity(pass1, &min_length, &min_upper, &min_lower, &min_dec);

return (0);
}

//**************************//

int read_in(int * min_length, int * min_upper, int * min_lower, int * min_dec, int * min_spec)
{
  
  //Read in password requirements from user//
  printf("\nEnter the minimum password length: ");
  scanf("%d", min_length);

  printf("\nEnter the minimum number of uppercase characters: "); 
  scanf("%d", min_upper);
     
  printf("\nEnter the minimum number of lowercase characters: ");
  scanf("%d", min_lower);
 
  printf("\nEnter the minimum number of decimal digit characters: ");
  scanf("%d", min_dec);
 
  printf("\nEnter the minimum number of special characters: ");
  scanf("%d", min_spec);
  
  return(0);

}
//**************************//

int verify_complexity(char*pass1, int*min_length,int*min_upper, int*min_lower, int*min_dec)
{
  int a, upper_count = 0, lower_count = 0, digit_count = 0;

  for(a = 0; a <= strlen(pass1); a = a + 1)
  {
    if(isupper(pass1[a]) > 0)
    {
      upper_count += 1;
    }
    if(islower(pass1[a]) > 0)
    {
      lower_count += 1;
    }
    if(isdigit(pass1[a]) > 0)
    {
      digit_count += 1;
    }
  }

  if (strlen(pass1) < *min_length)
  {
    printf("Password does not meet the minimum length requirement.\n");
  }

  else if (upper_count < *min_upper)
  {
    printf("Password does not meet the minimum upper case characters requirement.\n");
  }

  else if (lower_count < *min_lower)
  {
    printf("Password does not meet the minimum lower case characters requirement.\n");
  }

  else if (digit_count < *min_dec)
  {
    printf("Password does not meet the minimum decimal digit characters requirement.\n");
  }

  else
  {
   printf("\nPassword meets complexity requirement. Well done.\n");
  }

}

//*************************//
int bad_character_check(char*pass1)
{
  int a;

  for(a = 0; a < strlen(pass1); a ++)
  {
    if(isalpha(pass1[a]) ==  0)
    {
      if(isdigit(pass1[a]) ==  0)
      {
        if( (pass1[a] != '-') && (pass1[a] != '_') && (pass1[a] != '>') && (pass1[a] != '<') && (pass1[a] != '*') )
        {
           printf("Password contains the illegal character: %c \n", pass1[a]);
           exit(0);
        }
      }
    }
  }
 return(0);
}

//************************//

int min_char_countr(char*pass1, int min_spec)
{
  int a, count = 0;
  
  for( a = 0; a < strlen(pass1); a++)
  {
    if(isalpha(pass1[a]) == 0)
    {
      if(isdigit(pass1[a]) == 0)
      {
       count += 1;
      }
    }
  }
  if (count <  min_spec)
  {
    printf("\nPassword does not meet the minimum number of special characters\n");
  }

return(0);
}

//***********************//
