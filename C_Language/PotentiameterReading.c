/*******************************
 * Name:Alex Limon	
 * Student ID#:1000818599
 * Lab Day: 3441-001 Monday LAB
 * CSE 3442/5442 - Embedded Systems 1
 * Lab 4: Simple I/O Circuit (GPIO)
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
void main()
{
        Initial(); 	  	  
	DisplayC(Clear1); 
	DisplayC(Clear2); 
    
    //Part1();
    Part2(); // by default this will compile to part2 since it will be obvious that part1 has been done


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

void Part1()
{
    int char1,char2,i;
    int long binVal,potVal,highbyte,lowbyte; //initializing needed variables
    
    while(1)
    {
        for(i=0;i<80;i++) Delay(); // two second delay for everytime it checks
        
        ADCON0 = 0b00111001; // setting bits to correct channels and clock to take in analog
        ADCON1 = 0b11000000; 
        TRISE  = 0b00000100; //setting E2 to take input
        
        
        
        Delay10TCYx(16); //16 microsecond delay
        ADCON0bits.GO = 1; // go bit for whenever we are ready to convert
        
        while(!ADCON0bits.DONE);
        
        highbyte = ADRESH;  // the value is 16 bits so we need to separate them because of the nature of the registers
        lowbyte = ADRESL;   // these registers are what is grabbed from E2 and converted to digital
        binVal = (highbyte << 8) + lowbyte; // this is to concatenate the bits to make it a 16bit number from 0-1023 
        potVal= (binVal*50)/1023; // since its from 0-1023 we have to create a ratio between the numbers if we want it from 0-5
        
        char2= (potVal % 10)+ 48; //finding the ones place digit
        char1 =(potVal /10) + 48; // finding the tens place digit
        Str_2[1]= char1; // this will be placed in our string to be displayed on LCD
        Str_2[2]='.';
        Str_2[3]= char2;
        Str_2[4]= 'V';
        ADCON1 = 0b10001110;//setting bits back in order to get ready for LCD display
        DisplayC(Str_2); // LCD display time!
            
        
  }  
}
void Part2()
{   
  int char1,char2,char3,char4,i;
  int long binVal1,binVal2,potVal1,potVal2,highbyte1,lowbyte1,highbyte2,lowbyte2; 
    
    while(1)
    {
        for(i=0;i<80;i++) Delay();  // same 2 second delay as above
        
/* A lot of this process remains the same so there is no need to repeat the commenting.
   Only the different things from part1 will be commented.
*/
        

        ADCON0 = 0b00111001;
        ADCON1 = 0b11000000;
        TRISE  = 0b00000100;
        
        Delay10TCYx(16); //16 microsecond delay
        ADCON0bits.GO = 1;
        
        while(!ADCON0bits.DONE);
        Delay10TCYx(16); //16 microsecond delay
        highbyte1 = ADRESH;
        lowbyte1 = ADRESL;
        binVal1 = (highbyte1 << 8) + lowbyte1;
        potVal1= (binVal1*50)/1023;
        
        char2= (potVal1 % 10)+ 48;
        char1 =(potVal1 /10) + 48;
        Str_1[1]='1';
        Str_1[2]='0';  // we have now added a 10k ohm symbol to properly denote the difference between the two POT
        Str_1[3]='K';
        Str_1[4]= 244; // omega symbol, thought it would look fancier ;)
        Str_1[5]= ':';
        Str_1[6]= char1;
        Str_1[7]='.';
        Str_1[8]= char2;
        Str_1[9]= 'V';
        
        
        
     ///Second READ!!
        
        for(i=0;i<80;i++) Delay();  // 2 second delay in-between the reads of the POTs
        
        ADCON0 = 0b00100001; // channel as been changed to accommodate the proper POT
        ADCON1 = 0b11000000;
        TRISE  = 0b00000100;
       
        
        
        Delay10TCYx(16); //16 microsecond delay
        ADCON0bits.GO = 1;
        
        while(!ADCON0bits.DONE);
        Delay10TCYx(16); //16 microsecond delay
        highbyte2 = ADRESH;
        lowbyte2 = ADRESL;
        binVal2 = (highbyte2 << 8) + lowbyte2;
        potVal2= (binVal2*50)/1023;
        
        char4= (potVal2 % 10)+ 48;
        char3 =(potVal2 /10) + 48;
        Str_2[1]='P';
        Str_2[2]='O';  //changed the LCD display to be POT to differentiate the two, we also now use the second string on the bottom
        Str_2[3]='T';
        Str_2[4]='1';
        Str_2[5]=':';
        Str_2[6]= char3;
        Str_2[7]='.';
        Str_2[8]= char4;
        Str_2[9]= 'V';
        ADCON1 = 0b10001110;//setting the bits back to normal to prepare for display
        DisplayC(Str_1);
        DisplayC(Str_2);   //displaying both bits
    
    
        TRISA = 0b0100000; // setting up for LED showcase
        ADCON1 =0b11000110;  // we need to set up digital 
        
        if(potVal1>potVal2) // if the hardware pot is great show this LED
        {
          PORTAbits.RA3=1;
          PORTAbits.RA2=0; 
          PORTAbits.RA1=0;
        
        }
        else if(potVal1 == potVal2) // equal show the middle LED
        { 
          PORTAbits.RA3=0;
          PORTAbits.RA2=1; 
          PORTAbits.RA1=0;
            
        }
        else // same toggling as two above
        {   
          PORTAbits.RA3=0;
          PORTAbits.RA2=0; 
          PORTAbits.RA1=1;
      
        }
      
    
    
    
    
    
    
    
    
    
    }
        
       
  
 
 
} 
    
