
Created by: Alex Limon Naranjo
Class: Section: CSE 3441-001
Assignment: Homework #3


Compilation Instructions: gcc circularbuffer.c -o circularbuffer
Run: ./circularbuffer

Description:
This program is a circular buffer with a maximum size of 200 unsigned characters. The user is proved with a easy
UI that you can either remove and insert a character. All declarations are made in a .h file and the source can
be seen in circularbuffer.c.

Functionality:
The way the program works is it initializes with buffer with the constant defintion of 200 characters and inserts
it at the tail pointer position. This is simulating writing to a buffer, useful in serial communication. The way
the tail pointer is advances is by incrementing by when and then it is modded with the maximum buffer size,
so that if it reaches the limit it will go back to zero. For example if the position is 150, 150%200 = 150 so it
stays at 150, but if it is 200 then 200% 200 = 0.This creates the feeling of the circular buffer. There is
exception handling when the buffer size is too big. The character is an argument to the insertChar function.The
head pointer advances in the same exact way and it checks if the buffer is empty and returns -1. The only
difference is that insertChar will increase the buffer size and deleteChar will decrement the buffersize.