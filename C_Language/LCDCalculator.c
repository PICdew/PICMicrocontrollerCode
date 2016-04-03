/*******************************
 * Name Alex Limon:
 * Student ID #: 1000818599
 * Lab Day 02/08/2016:
 * CSE 3442/5442 - Embedded Systems 1
 * Lab 2: Simple I/O Circuit (GPIO)
 ********************************/

#include <p18cxxx.h>
#include <delays.h>

char COUNT;             // Counter available as local to functions
char RPGCNT;            // Used to display RPG changes


/*******************************
 * Strings for LCD Initialization and use
 * For stability reasons, each array must have 10 total elements (no less, no more)
 ********************************/
const char LCDstr[]  = {0x33,0x32,0x28,0x01,0x0c,0x06,0x00,0x00}; // LCD Initialization string (do not change)

//Never change element [0] or [9] of these char arrays
//You may only change the middle 8 elements for displaying on the LCD
char Str_1[] = {0x80,' ',' ',' ',' ',' ',' ',' ',' ',0};    // First line of LCD
char Str_2[] = {0xC0,' ',' ',' ',' ',' ',' ',' ',' ',0};   // Second line of LCD

const char Clear1[] = {0x80,' ',' ',' ',' ',' ',' ',' ',' ',0};  // Clear first line of LCD
const char Clear2[] = {0xC0,' ',' ',' ',' ',' ',' ',' ',' ',0};  // Clear second line of LCD


/*******************************
 * Function prototypes
 ********************************/
void Initial(void);
void InitLCD(void);
void DisplayC(const char *);
void Delay();
void CustomInitial();
void Addition(int A, int B);
void Subtraction(int A, int B);
void AndER(int A,int B);
void NOTER(int B);
void Multiply(int A,int B);

void main()
{
    int num1;
    int num2;
    Initial(); 	  	  //Initialize all settings required for general QwikFlash and LCD operation
    DisplayC(Clear1); //Clear the LCD one time at the beginning of your program  
	DisplayC(Clear2); 
    
	//Your personal PORT/TRIS/ADCON/etc settings or configurations can go here 
	//Or make your own function and call it
    
        Str_1[1]='A'; // these two variables are the default ones for any operation so I put them in there for good use
        Str_1[3]='B';
        
        while(1)
        {   
            num1 = PORTB & 15; // these bit masks are used to get the lower nibble  values in the ports to avoid garbage values
            num2=  PORTC & 15;
            
            
            
            while(PORTBbits.RB4==0 && PORTBbits.RB5==0) // this inner while loop will run for as long as we want to find the multiplication, which was asked by the TA to replace addition
            {   
                num1 = PORTB & 15; //another bit mask to ensure nothing weird will happen to values
                num2=  PORTC & 15;
                Multiply(num1,num2); // this function will do the string setup and operations
                DisplayC(Str_1); // both display functions are placed in order to display the resulting values after the function call
                DisplayC(Str_2);
                if(PORTBbits.RB4!=0 && PORTBbits.RB5!=0) break; // the break will allow this loop to not on on forever

            }
        
            
            while(PORTBbits.RB4==0 && PORTBbits.RB5==1) // similar to top while loop but this combination will operate with a subtract
            {
                num1 = PORTB & 15; //another read of values in case they changed
                num2=  PORTC & 15;
                Subtraction(num1,num2); // this function will pass the values and perform subtract
                DisplayC(Str_1); // display the new values
                DisplayC(Str_2);
                
                if(PORTBbits.RB4!=0 && PORTBbits.RB5!=1) break; // break if we are no long wanting to subtract
                
            }
            while(PORTBbits.RB4==1 && PORTBbits.RB5==0) // check to see if we want to do AND and if so initiate a loop
            {
                num1 = PORTB & 15; // regather new values wanted after displaying results
                num2=  PORTC & 15;
                
                AndER(num1,num2); // send to the function to AND
                DisplayC(Str_1); // display the results from the modified strings
                DisplayC(Str_2);
                
                if(PORTBbits.RB4!=1 && PORTBbits.RB5!=0) break; // break if the combination no longer matches the subtract
                
            }

             while(PORTBbits.RB4==1 && PORTBbits.RB5==1) //conditional while loop to check and loop if we want to NOT B
            {
                num1 = PORTB & 15; // regather the values every single iteration of the loop
                num2=  PORTC & 15;
                
                 NOTER(num2); // send to the function and NOT and place in new string
                DisplayC(Str_1); // display the resulting string
                DisplayC(Str_2);
                
                if(PORTBbits.RB4!=1 && PORTBbits.RB5!=1) break; // break if we don't want to NOT anymore
                
            }

            
            
            
            Delay(); // delay everytime we exit a while loop to allow the LCD to regather itself just in case
           
            }
}
/*******************************
 * Initial()
 *
 * This function performs all initializations of variables and registers.
 *
 * DO NOT CHANGE ANYTHING IN THIS FUNCTION
 ********************************/
void Initial()
{
        ADCON1 = 0b10001110;            // Enable PORTA & PORTE digital I/O pins
        TRISA  = 0b11100001;            // Set I/O for PORTA
        TRISB  = 0b11111111;            // THIS WAS MODIFIED TO CHANGE I/O to accomodate the pins and correctly burning the code
        TRISC  = 0b11111111;            // THIS WAS MODIFIED TO CHANGE I/O to accomodate the pins and correctly burning the code
        TRISD  = 0b00001111;            // Set I/O for PORTD
        TRISE  = 0b00000000;            // Set I/O for PORTE
        PORTA  = 0b00010000;            // Turn off all four LEDs driven from PORTA
        RPGCNT   = 0;                   // Clear counter to be displayed        
        InitLCD();                      // Initialize LCD
}

/*******************************
 * InitLCD()
 *
 * Initialize the Optrex 8x2 character LCD.
 * First wait for 0.1 second, to get past display's power-on reset time.
 *
 * DO NOT CHANGE ANYTHING IN THIS FUNCTION
 *******************************/
void InitLCD()
{
        char currentChar;
        char *tempPtr;
        COUNT = 10; 
                   
        while (COUNT)
        {         
		  Delay();	
          COUNT--;
        }

        PORTEbits.RE0 = 0;              // RS=0 for command
        tempPtr = LCDstr;

        while (*tempPtr)                // if the byte is not zero
        {
          currentChar = *tempPtr;
          PORTEbits.RE1 = 1;            // Drive E pin high
          PORTD = currentChar;          // Send upper nibble
          PORTEbits.RE1 = 0;            // Drive E pin low so LCD will accept nibble          
          Delay();	
	      currentChar <<= 4;            // Shift lower nibble to upper nibble
          PORTEbits.RE1 = 1;            // Drive E pin high again
          PORTD = currentChar;          // Write lower nibble
          PORTEbits.RE1 = 0;            // Drive E pin low so LCD will process byte        
          Delay();	
	      tempPtr++;                    // Increment pointerto next character
        }
}

/*******************************
 * DisplayC(const char *) 
 *
 * This function is called with the passing in of an array of a constant
 * display string.  It sends the bytes of the string to the LCD.  The first
 * byte sets the cursor position.  The remaining bytes are displayed, beginning
 * at that position.
 * This function expects a normal one-byte cursor-positioning code, 0xhh, or
 * an occasionally used two-byte cursor-positioning code of the form 0x00hh.
 *
 * DO NOT CHANGE ANYTHING IN THIS FUNCTION
 ********************************/
void DisplayC(const char * tempPtr)
{
	char currentChar;
        PORTEbits.RE0 = 0;              // Drive RS pin low for cursor-positioning code

        while (*tempPtr)                // if the byte is not zero
        {
          currentChar = *tempPtr;
          PORTEbits.RE1 = 1;            // Drive E pin high
          PORTD = currentChar;          // Send upper nibble
          PORTEbits.RE1 = 0;            // Drive E pin low so LCD will accept nibble
          currentChar <<= 4;            // Shift lower nibble to upper nibble
          PORTEbits.RE1 = 1;            // Drive E pin high again
          PORTD = currentChar;          // Write lower nibble
          PORTEbits.RE1 = 0;            // Drive E pin low so LCD will process byte
	      Delay();		
          PORTEbits.RE0 = 1;            // Drive RS pin high for displayable characters
          tempPtr++;                    // Increment pointerto next character
        }
}

/******************************************************************************
	int Delay()
	This function is called to create a .05 sec delay multiplied 
	by the integer given to it to make a specific delay time
******************************************************************************/
void Delay()
{   
	int delay = 2;
    int index = 0;

	for( index = 0; index < delay; index++ )
	{
		Delay100TCYx(1250);		// each delay cost .05 of a second
	}
	
}
//this function is gonna set input and output for the function

void Addition(int  A, int B) // addition function takes two integers
{
   
    
    int result = A+B; // will take the integer results and do a simple addition
    char char1; // initializing some characters to be placed later in the string
    char char2;
    Str_1[1]='A'; // below are the basic conventions for what an addition will loook like statically
    Str_1[2]='+';
    Str_1[3]='B';
    Str_1[4]=' '; // these blanks clear any previous used part of the LCD or string
    Str_1[5]=' ';
    
    
    Str_2[3]=' '; // same with these but from a different string
    Str_2[4]=' ';
    if(result>9) // this condtion will check if we have a two digit number result
    {
        char2= (result % 10)+ 48; // by doing this operation it will parse the integer's ones places and create into ASCII to be processed right in the string
        char1 =(result /10) +48; // this operation does the same but for the tenth place
        Str_2[1]= char1; // place the new ascii values in the string
        Str_2[2]= char2;
    }
    else // this conditional is just as important, it checks for a one digit number
    {
        char1 = result+48; // simple ASCII convertion from numbers 0-9
        Str_2[1]='0'; // the tenths place will always be 0 in a one digit number
        Str_2[2]= char1; // chunk it in the string
               
    }
    
}







void Subtraction(int A, int B) // similar concept of grabbing the integers from the port B/C
{
    
    
    char char1;
    char char2;
    int result = A-B; // perform the subtract
    Str_1[1]='A'; // these next few lines create the setup for the LCD where we have static situations
    Str_1[2]='-';
    Str_1[3]='B';
    Str_1[4]=' ';
    Str_1[5]=' ';
  
    Str_2[3]=' ';
    Str_2[4]=' ';
    if(result > 9) // same concept as addition, looking for a two digit number
    {
        char2= (result % 10)+ 48; // ones place parsing
        char1 =(result /10) +48; // tenths place
        Str_2[1]= char1; // insert in string
        Str_2[2]= char2;
    }
    else if(result> 0 && result<10) // finding the single digit number
    {
        char1 = result+48;
        Str_2[1]='0';
        Str_2[2]= char1;
        
    
    }
    else if( result > -10 && result < 0 ) // this will be different from ADD, we are looking for a single digit number that is negative
    {
        result *=-1; // lets not mess up ASCII convertions here
        char1 = result+48;
        Str_2[1]='-'; // show the world its negative
        Str_2[2]= char1;
        
        
        
    }
    else // this is the case we have a two digit negative number
    {
        result *=-1; // make sure when we do the math we have a positive to display
        char2= (result % 10)+ 48;// this will get the ones place
        char1 =(result /10) +48; // tens place
        Str_2[1]='-'; // show its negative
        Str_2[2]= char1; // place in resulting string
        Str_2[3]= char2;
        
    
    }
    
}

void AndER(int A,int B) // just a simple AND function that takes in two integers
{
     
    
    int result = A & B; // perform the AND operations
    int counta=0;
    Str_1[1]='A'; // setting up the LCD display for the case when we have an AND
    Str_1[2]='&';
    Str_1[3]='B';
    Str_1[4]=' ';
    Str_1[5]=' ';
    Str_2[3]=' ';
    Str_2[4]=' ';
    while(counta<4) // we will loop 4 times to create a nibble from scratch
    {
        if(result%2==0) Str_2[4-counta]='0'; // if the number is divisble by 2 then its a 0
        else Str_2[4-counta]='1'; // if the current number isnt divisible then its a 1
        result /=2; // KEEP Dividing by 2 to check the divisibility of the next number using the algorithm
        counta+=1; // keeping track that so we dont pass a nibble
        
        
        
    }
    
    
}

void NOTER(int B) // we will not the B integer
{
    
    
    int counta=0; // simple LCD setup below, nothing new
    Str_1[1]='N';
    Str_1[2]='O';
    Str_1[3]='T';
    Str_1[4]='(';
    Str_1[5]='B';
    Str_1[6]=')';
    while( counta<4 ) // this loop will make sure we dont pass a nibble
    {
        if(B%2==0) Str_2[4-counta]='1'; // same type of binary conversion but this time we will do the opposite, hence the NOT
        else Str_2[4-counta]='0';
        B /=2; // keep up the division
        counta+=1;
        
        
        
    }
} 
    
    
 void Multiply(int A, int B) // EXTRA asked to do in LAB
  {
    int result= A*B; // perform the actual operation
    int char2;
    int char1;
    int char3;
    Str_1[1]='A'; //setting up the string for LCD display
    Str_1[2]='*';
    Str_1[3]='B';
    Str_1[4]=' ';
    Str_1[5]=' ';
    Str_1[6]=' ';
    Str_2[3]=' ';
    Str_2[4]=' ';
    
    
    if(result>99) // this is checking for a 3 digit number result and converting them from int to ASCII char
    {
        char1= (result / 100)+ 48; // this gets the hundreth place
        result = result % 100; // we will mod to get the lower two digits
        
        char3= (result % 10)+ 48; // now get its ones place
        char2 =(result /10) +48; // divide to get the tens place
        Str_2[1]= char1;
        Str_2[2]= char2;
        Str_2[3]= char3;
    
    
    }
    
    else if(result<100 && result >9) // are we dealing with a two digit numba??
    {
        char2= (result % 10)+ 48; // same process in converting to ascii as in the addition
        char1 =(result /10) +48;
        Str_2[1]= char1;
        Str_2[2]= char2;
    }
    else // one digit result simple as it gets
    {
        char1 = result+48;
        Str_2[1]='0';
        Str_2[2]= char1;
               
    }
    
      
      
  }  
    

