/*
	Author: MIDN Jacob A. Lindow

	Rudimentary Program to demonstrate 
	Basic usage of Pointers and functions
        in the language C. 
*/ 

#include<stdio.h>

// This program show how to use a function to compute area

// Function prototype
int area(int b, int h, int *answer);

int main()
{

// Declare variables
int b, h;
int *answer;		// Made this a pointer

printf("Please enter the base of the triangle:\n");
scanf("%d", &b);	// Scans in base and stores it in b - & refers to address of b

printf("Please enter the height of ther triangle:\n");
scanf("%d", &h);	// Scans in height and stores it in h - & refers to address of h

area(b,h,answer);

printf("\nThe area of your triangle is: %d\n\n", *answer);

return 0;
}

// This function computes the area
int area(int b, int h, int *answer)
{

*answer = 0.5 * b * h;
return(0);

}

