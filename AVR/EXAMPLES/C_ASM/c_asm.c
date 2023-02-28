/* Example how to call a function written in
   assembly from C

   CodeVisionAVR C Compiler
   (C) 2000-2008 HP InfoTech S.R.L.
   www.hpinfotech.ro */

// function in assembler declaration
// this function will return a+b+c
#pragma warn- // this will prevent warnings
int sum_abc(int a, int b, unsigned char c) {
#asm
    ldd   r30,y+3 ;R30=LSB a
    ldd   r31,y+4 ;R31=MSB a
    ldd   r26,y+1 ;R26=LSB b
    ldd   r27,y+2 ;R27=MSB b
    add   r30,r26 ;(R31,R30)=a+b
    adc   r31,r27
    ld    r26,y   ;R26=c
    clr   r27     ;promote unsigned char c to int
    add   r30,r26 ;(R31,R30)=(R31,R30)+c
    adc   r31,r27
#endasm
}
#pragma warn+ // enable warnings

void main(void) {
int r;
// now we call the function and store the result in r
r=sum_abc(2,4,6);
}

/*
Some explanations:

The compiler passes function parameters using the Data Stack.
First it pushes the integer parameter a, then b, and finally the
unsigned char parameter c.
On every push the Y register pair is decremented by the size of
the parameter (4 for long int, 2 for int, 1 for char).
For multiple byte parameters the MSB is pushed first.
As you see the Data Stack grows downward.
After all the functions parameters were pushed on the Data Stack,
the Y register points to the last parameter c, so we can read its
value in R26 using the instruction: ld r26,y.
The b parameter was pushed before c, so it is at a higher address
in the Data Stack.
We will read it using: ldd r27,y+2 (MSB) and ldd r26,y+1 (LSB).
The MSB was pushed first, so it is at a higher address.
The a parameter was pushed before b, so it is at a higher address
in the Data Stack.
We will read it using: ldd r31,y+4 (MSB) and ldd r30,y+3 (LSB).

The functions return their values in the registers:
    R30 for char & unsigned char
    R30, R31 for int & unsigned int
    R30, R31, R22, R23 for long & unsigned long.

So our function must return its result in the R30, R31 registers.
After the return from the function the compiler automatically generates
code to reclaim the Data Stack space used by the function parameters,
so you must do not worry about this.

The #pragma warn compiler directives will prevent the compiler from
generating a warning that our function does not return a value.
This is needed because the compiler does not know what we do in our
assembler portion of the function.
*/
