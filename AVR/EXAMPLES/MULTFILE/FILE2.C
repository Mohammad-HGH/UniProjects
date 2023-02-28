// #include the macro and type definitions and
// external declarations for global variables,
// from other .c program modules
#include "file1.h"
#include "file2.h"
#include "file3.h"

int var2;
ts2 struct2={MACRO_CONSTANT1*5,MACRO_CONSTANT2*5,MACRO_CONSTANT3*5};

// this function is declared in the header file "file2.h"
int func2(int a) {
return a*a+var1+var2+var3;
}
