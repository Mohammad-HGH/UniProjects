// #include the macro and type definitions and
// external declarations for global variables,
// from other .c program modules
#include "file1.h"
#include "file2.h"
#include "file3.h"

int var1;
ts1 struct1={MACRO_CONSTANT1,MACRO_CONSTANT2,MACRO_CONSTANT3};

// this function is declared in the header file "file1.h"
int func1(int a) {
return a+a+var1+var2+var3;
}
