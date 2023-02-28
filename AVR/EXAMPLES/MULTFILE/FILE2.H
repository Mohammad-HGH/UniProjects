#ifndef _FILE2_INCLUDED_ // prevent multiple inclusion in the same .c program module
#define _FILE2_INCLUDED_

#define MACRO_CONSTANT2 20

// type definition
typedef struct {
int a2,b2,c2;
} ts2;

// external declaration for global variables declared in "file2.c"
extern int var2;
extern ts2 struct2;

// prototype declaration of the function "func2"
// located in the project file "file2.c"
int func2(int a);

#endif

