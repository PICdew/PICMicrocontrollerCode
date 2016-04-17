/*
 * Created by: Alex Limon Naranjo
 * Class: Section: CSE 3441-001
 * Assignment: Homework #3
 */
#ifndef _circularbuffer_h
#define _circularbuffer_h

/* Defining the MAX buffer size as a constant*/
#define MAXBUF 200



/*External declarations for .c file, see .c file for actual usage*/
extern int headptr;
extern int tailptr;
extern int buffersize;

/*Function definition prototypes, see .c file for usage */
char insertChar( unsigned char newchar );
__int16_t deleteChar();
int numChar();

/*External declaration of actual buffer*/
extern unsigned char buffer[MAXBUF];


#endif
