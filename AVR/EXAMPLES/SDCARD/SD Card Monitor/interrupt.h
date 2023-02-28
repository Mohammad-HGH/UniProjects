/*------------------------------------------*/
/* Macros used to enable/disable interrupts */
/*------------------------------------------*/

#define sei() #asm("sei") // enable interrupts
#define cli() #asm("cli") // disable interrupts
