/*******************************
 * Name:Alex Limon	
 * Student ID#:1000818599
 * Lab Day: 3441-001 Monday LAB
 * CSE 3442/5442 - Embedded Systems 1
 * Lab 5: Digital to Analog Converter (DAC)*
 ********************************/

#include <p18cxxx.h>
#include<delays.h>
#include <math.h>
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
void main()
{
    Initial(); 	  	  
	DisplayC(Clear1); 
	DisplayC(Clear2); 
    
    
    
    
    //Part1();
    


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
   int tens1,ones1,ones2,tens2,hundreds2,i,cleandata;
   int long binVal1,binVal2,potVal1,potVal2,highbyte1,lowbyte1,highbyte2,lowbyte2,freq; 
   double data,t,amp;
    while(1)
    {
        //for(i=0;i<40;i++) Delay();  // same 2 second delay as above
        
/* A lot of this process remains the same so there is no need to repeat the commenting.
   Only the different things from part1 will be commented.
*/
        

        ADCON0 = 0b00111001;
        ADCON1 = 0b11000000;
        TRISE  = 0b00000100;
        PORTCbits.RC0 = 1;
        
        Delay10TCYx(4); //16 microsecond delay
        ADCON0bits.GO = 1;
        
        while(!ADCON0bits.DONE);
      
        highbyte1 = ADRESH;
        lowbyte1 = ADRESL;
        binVal1 = (highbyte1 << 8) + lowbyte1;
        amp = ((binVal1*5)/1023);
        potVal1= (binVal1*50)/1023;
        
        ones1= (potVal1 % 10)+ 48;
        tens1 =(potVal1 /10) + 48;
        Str_1[1]='A';
        Str_1[2]='M';  // we have now added a 10k ohm symbol to properly denote the difference between the two POT
        Str_1[3]='P';
        
        Str_1[4]= ':';
        Str_1[5]= tens1;
        Str_1[6]='.';
        Str_1[7]= ones1;
        Str_1[8]= 'V';
        
        
        
     ///Second READ!!
        
       // for(i=0;i<40;i++) Delay();  // 2 second delay in-between the reads of the POTs
        
        ADCON0 = 0b00100001; // channel as been changed to accommodate the proper POT
        ADCON1 = 0b11000000;
        TRISE  = 0b00000100;
       
        
        
        Delay10TCYx(4); //16 microsecond delay
        ADCON0bits.GO = 1;
        
        while(!ADCON0bits.DONE);
       
        highbyte2 = ADRESH;
        lowbyte2 = ADRESL;
        binVal2 = (highbyte2 << 8) + lowbyte2;
        
        potVal2= ((binVal2*100)/1023)+2;
        if(potVal2 > 100) potVal2= 100;
        freq = potVal2;
        hundreds2 = (potVal2/100) + 48;
        potVal2= (potVal2% 100);
        ones2 = (potVal2 % 10) + 48;
        tens2 =(potVal2 /10) + 48;
        
        Str_2[1]='F';
        Str_2[2]='R';  //changed the LCD display to be POT to differentiate the two, we also now use the second string on the bottom
        Str_2[3]='E';
        Str_2[4]='Q';
        Str_2[5]=':';
        Str_2[6]= hundreds2;
        Str_2[7]= tens2;
        Str_2[8]= ones2;
        
        ADCON1 = 0b10001110;//setting the bits back to normal to prepare for display
        DisplayC(Str_1);
        DisplayC(Str_2);   //displaying both bits
    
    
     
        TRISCbits.RC0 = 0;
       
        SSPSTAT =0b11000000;
        SSPCON1= 0b00100000;
        
        for(t=0;t < 1; t += .001)
        {
            PORTCbits.RC0 = 0;//turn on chip select
            SSPBUF = 0b00100001; //send control bits to buffer
           
            while(!SSPSTATbits.BF); // wait for control bits to be sent
            
            data= ( amp*sin(2*3.14159*freq*t)) + 5;
             
            /* convert to a 255 number and equal it to SSP because data will give you a
              number from 0-10 and cast into an integer, then send it  */
            data = (data/10) * 255;
           
            SSPBUF = data;
            while(!SSPSTATbits.BF);
            
            PORTCbits.RC0 = 1;
            
        }
 
    
        
       
  
 
 
} 

}