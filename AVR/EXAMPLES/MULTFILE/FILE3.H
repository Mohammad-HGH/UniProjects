#ifndef _FILE3_INCLUDED_ // prevent multiple inclusion in the same .c program module
#define _FILE3_INCLUDED_

#define MACRO_CONSTANT3 30

// type definition
typedef struct {
int a3,b3,c3;
} ts3;

// external declaration for global variables declared in "file3.c"
extern int var3;
extern ts3 struct3;

// prototype declaration of the function "func3"
// located in the project file "file3.c"
int func3(int a);

#endif
