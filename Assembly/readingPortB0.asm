; TODO INSERT CONFIG CODE HERE USING CONFIG BITS GENERATOR

; PIC18C452 Configuration Bit Settings

; ASM source line config statements

#include "p18C452.inc"

; CONFIG1L
  CONFIG  CP = OFF              ; Code Protection bits (Program memory code protection off)

; CONFIG1H
  CONFIG  OSC = XT              ; Oscillator Selection bits (XT oscillator)
  CONFIG  OSCS = OFF            ; Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

; CONFIG2L
  CONFIG  PWRT = OFF            ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  BOR = ON              ; Brown-out Reset Enable bit (Brown-out Reset enabled)
  CONFIG  BORV = 25             ; Brown-out Reset Voltage bits (VBOR set to 2.5V)

; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  WDTPS = 128           ; Watchdog Timer Postscale Select bits (1:128)

; CONFIG3H
  CONFIG  CCP2MX = ON           ; CCP2 Mux bit (CCP2 input/output is multiplexed with RC1)

; CONFIG4L
  CONFIG  STVR = ON             ; Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause RESET)

ORG 0H ; Let us be at instruction 0
MyReg EQU 0x20 ; Creating a variable in memory location 0x20
LOOP:
  MOVLW b'00000001'; setting the I/O for the PORTB register
  MOVWF TRISB ; moving it into the PORTB Register
  BTFSC PORTB,0 ;lets skip an instruction if we find out that PortB's 0th bit is 0
  GOTO SET1 ; if it's not 0 we will set it to 1
  GOTO SET0; here is where it would hypothetically skip
  
SET1:
    MOVLW 2H    ;lets move the B1 bit one to 0 
    MOVWF MyReg ; before moving it into the PORT, it will go in the memory location
    GOTO FINISH; the finish will be explained later
SET0:
    MOVLW 0H ; moving the B1 bit to 0
    MOVWF MyReg; before transferring it will be placed in a memory location 
    GOTO FINISH
FINISH:
    MOVF MyReg,W ; move the contents of the memory location myreg into the work register
    MOVWF LATB ; write the contents of the work register to PORTB
    GOTO LOOP ; this will loop nonstop until in order to run the clock stimulus
    

END
  
  
  
    
    
    
    
    
    
