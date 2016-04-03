/*******************************
 * Name:
 * Student ID#:
 * Lab Day:
 * CSE 3442/5442 - Embedded Systems 1
 * Lab 3: Simple I/O Circuit (GPIO)
 ********************************/

#include <p18cxxx.h>
#include<delays.h>

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
void Part1();
void Part2();
void Part3();
void Part4();

void main()
{
   
    Part1(); // lets have part1 run
    //Part2();
    //Part3();
    //Part4();
                  
       
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
        TRISB  = 0b11011100;            // Set I/O for PORTB
        TRISC  = 0b11010000;            // Set I/0 for PORTC
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
/*
 This part1 function will iterate through 255 possible characters on the LCD
 and display them
*/
void Part1()
{
    int i;
    int start=33;          
    int iterator=start;
    int end=255;
     
     
    Initial(); 	  	  //Initialize all settings required for general QwikFlash and LCD operation
	DisplayC(Clear1); //Clear the LCD one time at the beginning of your program  
	DisplayC(Clear2);
    while(1)    
    {
        DisplayC(Clear1); //clear out before displaying new line of characters
        DisplayC(Clear2);
        if(iterator==end) iterator=start;// constant start and end defined at beginning
        for(i=1;i<8;i++)
        {
           Str_1[i]=iterator;  // the iterator will change every 8 characters and each character is placed on the line
           iterator++;
           
        }
        for(i=1;i<8;i++)
        {
           Str_2[i]=iterator; // doing the same thing for the second line
           iterator++;
           
        }
        DisplayC(Str_1); // display all 16 characters with 8 in each line
        DisplayC(Str_2);
        

	
    }
 }

void Part2()
{
   
    int i;
    Initial(); 	  	  //Initialize all settings required for general QwikFlash and LCD operation
	DisplayC(Clear1); //Clear the LCD one time at the beginning of your program  
	DisplayC(Clear2);
   while(1)
   {
    DisplayC(Clear2);
    Str_1[1]=88;     // based on table found in LCD data sheet these are all decimal values of the formula needed to display
    Str_1[2]=61;
    Str_1[3]=224;
    Str_1[4]=42;   //left constantly defined in array
    Str_1[5]=246;
    Str_1[6]=226;
    Str_1[7]=42;
    Str_1[8]=89;
    DisplayC(Str_1); // display in one line
    for(i=0;i<100;i++) Delay(); // a delay in between the display of each formula given
    
    
    DisplayC(Clear1);
    
    Str_1[1]=89;   // second formula that is given same process as above
    Str_1[2]=61;
    Str_1[3]=115;
    Str_1[4]=105;
    Str_1[5]=110;
    Str_1[6]=40;
    Str_1[7]=244;
    Str_1[8]=42;
    
    Str_2[1]=116;
    Str_2[2]=41;
    DisplayC(Str_1); // two lines needed because there are 10 characters
    DisplayC(Str_2);
    Delay(); 
    Delay();
   }
}  
void Part3()
{   
    char tildeA[10];
    Initial(); 	  	  //Initialize all settings required for general QwikFlash and LCD operation
	DisplayC(Clear1); //Clear the LCD one time at the beginning of your program  
	DisplayC(Clear2);
    
    
    tildeA[0]=0x48;           //address of character 1 on LCD
    tildeA[1]=0x80 + 0b01000;  // each binary is the line of the matrix in the bitmap and adding 0x80 is an offset needed
    tildeA[2]=0x80 + 0b01000;  // for the very beginning of LCD
    tildeA[3]=0x80 + 0b11101;
    tildeA[4]=0x80 + 0b10101;
    tildeA[5]=0x80 + 0b10111;
    tildeA[6]=0x80 + 0b00010;
    tildeA[7]=0x80 + 0b00010;
    tildeA[8]=0x80 + 0b00000;
    tildeA[9]=0x00;
    
    DisplayC(tildeA); //programs the tilde on the LCD
    
    Str_1[4]=1; // put the character 1 on the display string, but in the middle
    Delay();
    while(1) DisplayC(Str_1); // keep displaying the tilde
    
    
    
    
    
    
    



}

void Part4()
{
    int i;
    char smiley[10];
    Initial(); 	  	  //Initialize all settings required for general QwikFlash and LCD operation
	DisplayC(Clear1); //Clear the LCD one time at the beginning of your program  
	DisplayC(Clear2);
    
    
    smiley[0]=0x50;
    smiley[1]=0x80 + 0b00000;  // same routine as last tilde but in the form of a smiley face
    smiley[2]=0x80 + 0b01010;
    smiley[3]=0x80 + 0b01010;
    smiley[4]=0x80 + 0b00000;
    smiley[5]=0x80 + 0b10001;
    smiley[6]=0x80 + 0b01010;
    smiley[7]=0x80 + 0b00100;
    smiley[8]=0x80 + 0b00000;
    smiley[9]=0x00;
    
    
    DisplayC(smiley); //program character 2 as the smiley face
    
    Delay(); // delay to allow LCD to process
    Str_1[4]=2; // smiley face on two lines
    Str_2[4]=2;
    while(1) //this while loop will have the smiley face appear to be bouncing
    { 
        DisplayC(Str_1); // display one line
        for(i=0;i<40;i++) Delay(); // delay before displaying the next line
        DisplayC(Clear1);
        DisplayC(Str_2); // display the next line
        for(i=0;i<40;i++) Delay(); // delay explained below
        DisplayC(Clear2);
    }
    
    /* the math behind the delay function is simple. the delay function provided gives us a .0250 second delay 
     in order to make this 1 second we multiply by 40 to make it 1 second.
     */
    
    
    
    
    
}