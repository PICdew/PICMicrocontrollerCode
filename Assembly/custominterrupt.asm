
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

  ORG 0000H
  i1 EQU 0x25
  i2 EQU 0x26
  GOTO _MAIN
  
  
  ORG 0008H
  BTFSS INTCON, INT0IF
  RETFIE ;set up the PC
  GOTO _INT0handler
  
  
  ORG 00100H
    _MAIN:
      MOVLW b'00000110'; 
      MOVWF ADCON1 ; have the correct configuration for ADCON1
      BSF INTCON, GIE    ; enable interrupts
      BSF INTCON, INT0IE   ; enable peripheral interrupts
      MOVLW b'11111111'
      MOVWF TRISB  ;setting PORTB for input
      MOVWF TRISC  ;setting PORTC for input
      BCF   TRISA, 5 ; setting PORTA.5 to output
      MOVF i1, W; this is going to be used as a default check to see if i1 is 0 ie the lower byte
      BNZ   _CHECKPA
      MOVF i2, W;
      BNZ   _CHECKPA
      GOTO  _MAIN
    
   _CHECKPA: 
     BTG PORTA, 5
     GOTO _INNERLOOP
   
  _INNERLOOP:
     DECF i1 ; decrement the low byte
     BZ  _OUTERLOOP 
     GOTO _CHECKPA
        
  _OUTERLOOP:
     MOVF i2, W
     BZ _MAIN ; if 0 we are done
     DECF i2 ; decrement from the high byte
     MOVLW 0xFF ; make the low byte to the highest possible
     MOVWF i1 
     GOTO _INNERLOOP 
   
     
   _INT0handler:
     ORG 200H
     BCF INTCON, INT0IF ; removing that the interrupt occurred
     MOVF PORTB, W ;reading the lower byte from PORTB
     MOVWF i1; this is moving the lower byte into memory
     MOVF PORTC, W ; reading the higher byte from PORTC
     MOVWF i2; moving higher byte into memory
     RETFIE 
      
  
 END