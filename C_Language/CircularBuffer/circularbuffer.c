/*
* Created by: Alex Limon Naranjo
* Class: Section: CSE 3441-001
* Assignment: Homework #3
*/

#include <stdio.h>
#include "circularbuffer.h" 
/*Refer to header file for function and global variable definitions*/

/*Actual buffer itself*/
unsigned char buffer[MAXBUF];

/* Global variable initializations to 0
 * tail pointer is where we start writing
 * head pointer is where we read from
 * buffer size starts at 0
*/
int headptr = 0;
int tailptr = 0;
int buffersize = 0;


int main ( void )
{
  int menuoption = 0; //Used for user selection
  int esc = 1; //Used as condition to break while loop
  unsigned char newchar; //Character we will insert
  __int16_t delret; //check return statement for when we read
  
  while(esc)
  {
      
/*
* The following print statements will display
* what the user should input and then it will
* follow by prompting user input using scanf.
* The menu option is controlling the switch statement.
* 
* Each case statement calls the desired function
* with appropriate exception handling for negative
* return values.
*/
      
    printf("\nBuffer Functions: \n");
    printf("\n");
    printf("1. Insert a character\n");
    printf("2. Remove a character\n");
    printf("3. Display number of characters\n");
    printf("4. Quit.\n");
    printf("\n");
    printf(" :");
    scanf("%d", &menuoption);
    
    switch(menuoption)
    {
      case 1:
        printf("\n");
        printf("Enter your character: ");
        scanf(" %c",&newchar);
        if( insertChar(newchar) == -1) printf("\nBuffer full.\n");
        break; 
      
      case 2:
        if( (delret=deleteChar()) == -1) printf("\nBuffer empty\n");
        else printf("\nCharacter Read: %c\n",delret);
        break;
      
      case 3:
        printf("\nThe buffer size is: %d\n", numChar());
        break;
      case 4:
        esc = 0;
        break;
    }
  }
}


/*The insertChar function takes the character the user inputs
 *and places it in the buffer based on the pointer.
 *The pointer mods with the buffersize to go
 *to the beginning if we are at the end the array.
 *Returns -1 if the buffer is full
*/

char insertChar( unsigned char newchar )
{
  if( buffersize < MAXBUF )
  { 
    buffer[tailptr] = newchar;
    tailptr++;
    tailptr %= MAXBUF;
    buffersize++;
    return 0;
  }
  else
  {
    return -1;
  }
}

/*The deleteChar() function removes the last character
 *in the buffer (AKA reads it).
 *Advances pointer based on the last char read
 *and mods with maxbuf to create circular effect
 *Returns the character read or
 *returns -1 if the buffer is empty
 */

__int16_t deleteChar()
{
  __int16_t readchar;
  
  if( buffersize > 0 )
  {
    readchar = buffer[headptr];
    headptr++;
    headptr %= MAXBUF;
    buffersize--;
    return readchar;
  }
  else
  {
    return -1;
  }
}

/*The numChar() function simply encapsulates the buffersize global variable 
 * and can return its value at any time
*/
int numChar()
{
  return buffersize;
}

