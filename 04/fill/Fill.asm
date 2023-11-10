// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
/////////////////////
(check)
@SCREEN
D=A
@0
M=D

@KBD
D=M
@black
D;JGT
@white
D;JEQ

@check
0;JMP
//////////////////////////
(black)
@1
M=-1
@change
0;JMP
///////////////////////////
(white)
@1
M=0
@change
0;JMP
/////////////////////////
(change)
@1
D=M
@0
A=M
M=D
@0
M=M+1
D=M
@KBD
D=A-D
@change
D;JGT
@check
0;JMP
//16384~24575