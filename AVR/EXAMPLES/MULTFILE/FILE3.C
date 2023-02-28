// #include the macro and type definitions and
// external declarations for global variables,
// from other .c program modules
#include "file1.h"
#include "file2.h"
#include "file3.h"

int var3;
ts3 struct3={MACRO_CONSTANT1*10,MACRO_CONSTANT2*10,MACRO_CONSTANT3*10};

// this function is declared in the header file "file3.h"
int func3(int a) {
return a*3+var1+var2+var3;
}
