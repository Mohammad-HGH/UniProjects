/*
 Adam Dunkels uIP Webserver example
 http://www.sics.se/~adam/uip/index.php/Main_Page
  
 Adapted for the Crumb644-NET AVR ATmega644P Ethernet
 module from http://www.chip45.com
 by S.Perzborn http://www.perzborn.net
        
 Adapted/optimized for the CodeVisionAVR V2.05.0+
 compiler by HP InfoTech s.r.l. http://www.hpinfotech.com
        
 * ----------------------------------------------------------------------------
  Programm Fuse Bits (20MHz crystal, no Prescaler, no JTAG) : 
  CKSEL 0=0 not checked
  CKSEL 1=0 not checked
  CKSEL 2=0 not checked
  CKSEL 3=0 checked
  
 * ----------------------------------------------------------------------------
 IP-address (192.168.178.14), netmask (255.255.255.0) and gateway (192,168,178,1) 
 are predefined in module CP2200.c function network_device_init()
 
 Type HELP at serial (RS232) interface for information about how to display and 
 change IP address and netmask during runtime
 (Serial interface parameters: 19200 baud, 8 data bits, no parity, 1 stop bit)
 */

#ifndef F_CPU
#define F_CPU 20000000
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <ctype.h>
#include <stdio.h>
#include <delay.h>
#include "CP2200.h"

#include "uip.h"
#include "uip_arp.h"
//#include "network-device.h"
#include "httpd.h"
#include "timer.h"

#define cli() #asm("cli")
#define sei() #asm("sei")

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])


/*---------------------------------------------------------------------------*/
 
void InitUART(void);

extern void write_CP2200(int adr, char value);
extern char read_CP2200(int adr);
extern void network_device_init(void);
extern u16_t network_device_read(void);
extern void network_device_send(void);


// globale Variablen
static bit toggle=0;
static char time;
//static char CP2200_status=0;
clock_time_t  sec_counter;

interrupt [TIM0_COMPA] void timer0_isr(void)
{
if (--time == 0)
   {
   // 1 Sekunden Timer
   time=75;
   sec_counter++;
   if (toggle==0)
      {
      PORTC &= ~(1<<1); // clear PC1 = LED ON
      toggle=1;
      }
   else
      {
      PORTC |= (1<<1); // set PC1 = LED OFF
      toggle=0;	
      }
   }
}

interrupt [EXT_INT1] void ext_int1_isr(void)
{
PORTB &= ~(1<<0); // clear PB0 = LED ON
}


void InitUART(void)
{
UBRR0H = 0;
UBRR0L = 64;
UCSR0B = (1<<RXEN0) | (1<<TXEN0); // transmit & receive enable 
UCSR0C = 0x06; // init RS232: 8,N,1
UCSR0A = 0x00; 
}
 
void clock_init(void)
{
cli();
// Timer0 init
TCCR0A = 0x02;
TCCR0B = 0x05;
OCR0A = 0x80; // compare value
TIMSK0 = 0x02;
sei();
}
	
clock_time_t clock_time(void)
{
return(sec_counter);
}

void uip_log(char *msg)
{
printf("%s",msg);
}

void cmd_ip(char *ipstr)
{
char *ip;
char *tmp;
unsigned char i;
uip_ipaddr_t ipaddr;
//		struct uip_eth_addr eaddr;
uip_gethostaddr(&ipaddr);
ip = (char *) &ipaddr;

if (*ipstr == 0)
   {
   // show ip address
   printf("%u.%u.%u.%u\r\n",ip[0],ip[1],ip[2],ip[3]);
   }	
else
   {
   // set ip address
   tmp=ipstr;		
   for (i=0;((*ipstr != 0) && (i<4));ipstr++)
       {
       if (*ipstr=='.') 
          {	
	  *ipstr=0;
	  ip[i++]=atoi(tmp);
	  tmp = ipstr + 1;
	  }
       }
   *ipstr=0;
   ip[i]=atoi(tmp);
   uip_ipaddr(ipaddr, ip[0],ip[1],ip[2],ip[3]);
   uip_sethostaddr(ipaddr);			
   }
} // cmd_ip

void cmd_netmask(char *ipstr)
{
char *ip;
char *tmp;
unsigned char i;
uip_ipaddr_t ipaddr;
//struct uip_eth_addr eaddr;
uip_getnetmask(&ipaddr);
ip = (char *) &ipaddr;
	
if (*ipstr == 0)
   {
   // show netmask
   printf("%u.%u.%u.%u\r\n",ip[0],ip[1],ip[2],ip[3]);
   }	
else
   {
   // set netmask
   tmp=ipstr;		
   for (i=0;((*ipstr != 0) && (i<4));ipstr++)
       {
       if (*ipstr=='.') 
          {	
	  *ipstr=0;
	  ip[i++]=atoi(tmp);
	  tmp = ipstr + 1;
	  }
       }
   *ipstr=0;
   ip[i]=atoi(tmp);
   uip_ipaddr(ipaddr, ip[0],ip[1],ip[2],ip[3]);
   uip_setnetmask(ipaddr);			
   }
} // cmd_netmask

void cmd_help(void)
{
printf("\r\nCommands:\r\n");
printf("  CPRESET                   // reinitialize ethernet controller\r\n");
printf("  IP <xxx.xxx.xxx.xxx>      // display / set IP address\r\n");
printf("  NETMASK <xxx.xxx.xxx.xxx> // display / set netmask\r\n");
printf("  HELP                      // display available commands\r\n\r\n");
}

void parse(char *zeile)
{
char *pcmd;
char *argv[80];
int argc=0;
unsigned char i=0;
char k;
	
// Eingabestrings in Teilstrings wandeln
// Trennzeichen = Leerzeichen
// argv[0..9] = Teilstrings (Nullterminiert) 
// argc = Anzahl der Teilstrings (1..10) 
// Bei keiner Eingabe (nur Enter) ist argc = 1
for(;argc<10;)
   {
   // führende Leerzeichen überlesen (oder Ende)
   for(;((*(zeile+i) <=' ')&&(*(zeile+i) !=0));i++);
   argv[argc++]=zeile+i;  // pointer auf Teilstring
   if (*(zeile+i) ==0) break;
   // bis nächsten Leerzeichen lesen (oder Ende)
   for(;((*(zeile+i) >' ')&&(*zeile+i !=0));i++);
   if (*(zeile+i)==0) break;
   *(zeile+i) =0; // Teilsstring terminieren
   i++;
   }

pcmd=argv[0];
i=0;
#pragma warn-
while (k=*pcmd)
#define abc 5
      {
      *pcmd++=toupper(k);
      i++;
      }
#pragma warn+
pcmd=argv[0];

if ((i == 2) && (strcmpf(pcmd,"IP")==0))
   {
   if (argc>1) cmd_ip(argv[1]);
   else cmd_ip("");
   }
else
if (i == 7)
   {
   if (strcmpf(pcmd,"NETMASK")==0)
      {
      if (argc>1) cmd_netmask(argv[1]);
      else cmd_netmask("");
      }
   else
   if (strcmpf(pcmd,"CPRESET")==0)
      {
      printf("Reseting CP2200...");
      network_device_init();
      printf("done \n\r");
      }
   }
else
if ((i == 4) && (strcmpf(pcmd,"HELP")==0)) cmd_help();
} // parse


void main(void)
{
char taste=0;
char zeile[80];
int i=0;
struct timer periodic_timer, arp_timer;
DDRA = 0x00;    
DDRB = 0xFF;    
DDRC = 0xFF;
PORTA = 0x00;
PORTB = 0x00;
PORTC = 0xF7;
PORTC |= (1 << nRD); // set RD#
PORTC |= (1 << nWR); // set WR#
PORTC &= ~(1 << ALE); // clear ALE
PORTC &= ~(1 << nCS); // clear CS
PORTC |= (1 << nLRST); // reset aus, CP2200 wieder laufen lassen
	
InitUART();

printf("\r\nAdam Dunkels uIP webserver example for Crumb644-NET\r\n\r\n");
printf("Based on work of Mr. S.Perzborn\r\n");
printf("http://www.perzborn.net\r\n\r\n");
printf("Ported to CodeVisionAVR V2.05.0+ by HP InfoTech s.r.l.\r\n");
printf("http://www.hpinfotech.com\r\n");

cmd_help();
//Init_CP2000();
cli(); // disable interrupts

EICRA = 0x08; // INT1 fallede Flanke
EIMSK = 0x02; // enable external int1

sei();  // enable interrupts 
printf("init CP2200 ...");
network_device_init();
cmd_ip("");
clock_init();
timer_set(&periodic_timer, CLOCK_SECOND / 2);
timer_set(&arp_timer, CLOCK_SECOND * 10);
printf("\r\nready. \r\n");
delay_ms(10);

uip_arp_init();
uip_init();
httpd_init();

while (1) 
      {
      uip_len = network_device_read();
      if (uip_len > 0) 
         {
         if (BUF->type == htons(UIP_ETHTYPE_IP)) 
            {
            uip_arp_ipin();
            uip_input();
            /* If the above function invocation resulted in data that
            should be sent out on the network, the global variable
            uip_len is set to a value > 0. */
            if (uip_len > 0) 
               {
               uip_arp_out();
               network_device_send();
               }
            }
         else
         if (BUF->type == htons(UIP_ETHTYPE_ARP)) 
            {
            uip_arp_arpin();
            /* If the above function invocation resulted in data that
            should be sent out on the network, the global variable
            uip_len is set to a value > 0. */
            if (uip_len > 0) network_device_send();
            }
         } 
      else
      if (timer_expired(&periodic_timer)) 
         {
         timer_reset(&periodic_timer);
         for (i = 0; i < UIP_CONNS; i++) 
               {
               uip_periodic(i);
               /* If the above function invocation resulted in data that
               should be sent out on the network, the global variable
               uip_len is set to a value > 0. */
               if (uip_len > 0) 
                  {
                  uip_arp_out();
                  network_device_send();
                  }	
               }

         /*#if UIP_UDP
         for (i = 0; i < UIP_UDP_CONNS; i++) 
               {
               uip_udp_periodic(i);
               //If the above function invocation resulted in data that
               //should be sent out on the network, the global variable
               //uip_len is set to a value > 0. 
               if (uip_len > 0) 
                  {
                  uip_arp_out();
                  network_device_send();
                  }
               }
         #endif // UIP_UDP */
         /* Call the ARP timer function every 10 seconds. */
         if (timer_expired(&arp_timer)) 
              {
              timer_reset(&arp_timer);
              uip_arp_timer();
              }
         }
		
      if (UCSR0A & (1<<RXC0))  // Zeichen verfuegbar?
         {
         taste = UDR0; // Zeichen abholen
         putchar(taste); // Zeichen ausgeben
         if (taste == 13)
            {
            putchar('\n');
            zeile[i]=0;
            parse(zeile);	
            putchar('#');
            i=0;
            }
         else
         if (taste == 8)
            {
            // Backspace key
            if (i>0) 
               {
               i--;
               printf("\x1B[P"); // VT100-Code Backspace
               }
            }	
         else
            {
            if (i<78) zeile[i++]=taste; // Zeichen merken
            }
         }		
      }
}	// main
	
