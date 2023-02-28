#ifndef _FILE1_INCLUDED_ // prevent multiple inclusion in the same .c program module
#define _FILE1_INCLUDED_

#define MACRO_CONSTANT1 10

// type definition
typedef struct {
int a1,b1,c1;
} ts1;

// external declaration for global variables declared in "file1.c"
extern int var1;
extern ts1 struct1;

// prototype declaration of the function "func1"
// located in the project file "file1.c"
int func1(int a);

#endif
