            ////////  WWW.EMIC.IR  ////////
#include <mega16.h>
#include <delay.h>
#include <math.h>
void main(void)
{
int i,a ;

DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
PORTA=(1<<PORTA7) | (1<<PORTA6) | (1<<PORTA5) | (1<<PORTA4) | (1<<PORTA3) | (1<<PORTA2) | (1<<PORTA1) | (1<<PORTA0);

DDRC=(1<<DDC7) | (1<<DDC6) | (1<<DDC5) | (1<<DDC4) | (1<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

while (1)
      {
       PORTC=0;

       if(PINA.0==0)
       {
       PORTC=0b10101010;
       delay_ms(500);
       PORTC=0b01010101;
       delay_ms(500);
       }

       if(PINA.1==0)
         {
          for(i=0;(i<=7)&(PINA.1==0);i++)
           { 
            a=pow(2,i);    
            PORTC=a;
            delay_ms(250);
           }
         }
        
       if(PINA.2==0)
         {a=0;
          for(i=0;(i<=7)&(PINA.2==0);i++)
           { 
            a=a+pow(2,i);    
            PORTC=a;
            delay_ms(250);
           }
         }  

     }
}
