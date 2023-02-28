/****************************************** 
   Example of a simple multifile project

   CodeVisionAVR C Compiler
   (C) 2000-2009 HP InfoTech S.R.L.
   www.hpinfotech.ro 
*******************************************/

/*
- do Project|Build All
- do Tools|Debugger or press the Debugger toolbar button
  to launch AVR Studio 4.15 or later
- in AVR Studio do File|Open File the COFF object file:
  \cvavr2\examples\multfile\multfile.cof
- when prompted save the AVR Studio project file
- select in AVR Studio the debug platform: Simulator and device: ATmega128
- in AVR Studio select View|Watch to show the variables watch window
- add watches for the global variables: var1, var2, var3, struct1, struct2, struct3
- add also a watch for the local variable i
- start single stepping through the program using the F11 key

When rebuilding the program, there is no need to close AVR Studio,
it will detect that the COFF object file has changed and reload it 
*/

// this is the main file of the project

// #include the macro and type definitions and
// external declarations for global variables,
// from other .c program modules
#include "file1.h"
#include "file2.h"
#include "file3.h"

void main() {
// declare a local variable
int i;

// assign values to global variables declared in "file1.c", "file2.c" and "file3.c"
// using the macros defined in the header files "file1.h", "file2.h" and "file3.h" 
var1=MACRO_CONSTANT1;
var2=MACRO_CONSTANT2;
var3=MACRO_CONSTANT3;

// call a function with the prototype declared in "file1.h"
// the function itself is located in "file1.c"
i=func1(var1);

// call a function with the prototype declared in "file2.h"
// the function itself is located in "file2.c"
i=i-func2(var2);

// call a function with the prototype declared in "file3.h"
// the function itself is located in "file3.c"
i=i+func3(var3);

// use the structures declared in "file1.c", "file2.c" and "file3.c"
i=i+struct1.a1+struct2.a2+struct3.a3;
i=i-(struct1.b1+struct2.b2+struct3.b3);
i=i+struct1.c1+struct2.c2+struct3.c3;

// stop here
while (1);
}
