/**
 * \addtogroup httpd
 * @{
 */

/**
 * \file
 *         Web server script interface
 * \author
 *         Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2001-2006, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: httpd-cgi.c,v 1.2 2006/06/11 21:46:37 adam Exp $
 *
 */

#include "uip.h"
#include "psock.h"
#include "httpd.h"
#include "httpd-cgi.h"
#include "httpd-fs.h"

#include <stdio.h>
#include <string.h>

HTTPD_CGI_CALL(file, "file-stats", file_stats);
HTTPD_CGI_CALL(tcp, "tcp-connections", tcp_stats);
HTTPD_CGI_CALL(net, "net-stats", net_stats);

static flash struct httpd_cgi_call * flash calls[] = { &file, &tcp, &net, NULL };

/*---------------------------------------------------------------------------*/
static
PT_THREAD(nullfunction(struct httpd_state *s, flash char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
flash char *strchr_P(flash char *str, char c)
{
while ((*str != 0) && (*str != c)) str++;
if (*str == 0) return(0);
return str;
}
/*---------------------------------------------------------------------------*/
signed char strncmp_P(flash char *str1, char flash *str2, unsigned int n)
{
char k1=0,k2=0;
while (n--)
      {
      k1=*str1++;
      k2=*str2++;
      if (k1!=k2) break;
      if (k2==0) break; 
      }
return k1-k2;
}
/*---------------------------------------------------------------------------*/
httpd_cgifunction httpd_cgi(flash char *name)
{
  flash struct httpd_cgi_call * flash *f;


  /* Find the matching name in the table, return the function. */
  for(f = calls; *f != NULL; ++f) {
    if(strncmp_P((*f)->name, name, strlenf((*f)->name)) == 0) {
      return (*f)->function;
    }
  }
  return nullfunction;
}
/*---------------------------------------------------------------------------*/
#if defined(_MODEL_MEDIUM_) || defined (_MODEL_LARGE_)
static unsigned short generate_file_stats(unsigned long arg)
#else
static unsigned short generate_file_stats(void *arg)
#endif
{
#pragma warn-
return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%5u", httpd_fs_count((flash char *)arg));
#pragma warn+
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(file_stats(struct httpd_state *s, flash char *ptr))
{
  PSOCK_BEGIN(&s->sout);

#pragma warn-
#if defined(_MODEL_MEDIUM_) || defined (_MODEL_LARGE_)
  PSOCK_GENERATOR_SEND(&s->sout, generate_file_stats, (unsigned long) strchr_P(ptr, ' ') + 1);
#else
  PSOCK_GENERATOR_SEND(&s->sout, generate_file_stats, (void *) (strchr_P(ptr, ' ') + 1));
#endif
#pragma warn+
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static flash char closed[] = "CLOSED";
static flash char syn_rcvd[] = "SYN-RCVD";
static flash char syn_sent[] = "SYN-SENT";
static flash char established[] = "ESTABLISHED";
static flash char fin_wait_1[] = "FIN-WAIT-1";
static flash char fin_wait_2[] = "FIN-WAIT-2";
static flash char closing[] = "CLOSING";
static flash char time_wait[] = "TIME-WAIT";
static flash char last_ack[] = "LAST-ACK";

static flash char * flash states[] = {
  closed,
  syn_rcvd,
  syn_sent,
  established,
  fin_wait_1,
  fin_wait_2,
  closing,
  time_wait,
  last_ack};
  
#if defined(_MODEL_MEDIUM_) || defined (_MODEL_LARGE_)
static unsigned short generate_tcp_stats(unsigned long arg)
#else
static unsigned short generate_tcp_stats(void *arg)
#endif
{
  struct uip_conn *conn;
  struct httpd_state *s = (struct httpd_state *)arg;
  conn = &uip_conns[s->count]; 
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE,
		 "<tr><td>%d</td><td>%u.%u.%u.%u:%u</td><td>%p</td><td>%u</td><td>%u</td><td>%c %c</td></tr>\r\n",
		 htons(conn->lport),
		 htons(conn->ripaddr[0]) >> 8,
		 htons(conn->ripaddr[0]) & 0xff,
		 htons(conn->ripaddr[1]) >> 8,
		 htons(conn->ripaddr[1]) & 0xff,
		 htons(conn->rport),
		 states[conn->tcpstateflags & UIP_TS_MASK], // stored in FLASH, use %p as format specifier
		 conn->nrtx,
		 conn->timer,
		 (uip_outstanding(conn))? '*':' ',
		 (uip_stopped(conn))? '!':' ');
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(tcp_stats(struct httpd_state *s, flash char *ptr))
{
  
  PSOCK_BEGIN(&s->sout);

  for(s->count = 0; s->count < UIP_CONNS; ++s->count) {
    if((uip_conns[s->count].tcpstateflags & UIP_TS_MASK) != UIP_CLOSED) {
#if defined(_MODEL_MEDIUM_) || defined (_MODEL_LARGE_)
      PSOCK_GENERATOR_SEND(&s->sout, generate_tcp_stats, (unsigned long) s);
#else
      PSOCK_GENERATOR_SEND(&s->sout, generate_tcp_stats, s);
#endif
    }
  }

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
#if defined(_MODEL_MEDIUM_) || defined (_MODEL_LARGE_)
static unsigned short generate_net_stats(unsigned long arg)
#else
static unsigned short generate_net_stats(void *arg)
#endif
{
struct httpd_state *s = (struct httpd_state *)arg;
return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE,"%5u\n", ((uip_stats_t *)&uip_stat)[s->count]);
}

static
PT_THREAD(net_stats(struct httpd_state *s, flash char *ptr))
{
  PSOCK_BEGIN(&s->sout);

#if UIP_STATISTICS

  for(s->count = 0; s->count < sizeof(uip_stat) / sizeof(uip_stats_t);
      ++s->count) {
#if defined(_MODEL_MEDIUM_) || defined (_MODEL_LARGE_)
    PSOCK_GENERATOR_SEND(&s->sout, generate_net_stats, (unsigned long) s);
#else
    PSOCK_GENERATOR_SEND(&s->sout, generate_net_stats, s);
#endif
  }
  
#endif /* UIP_STATISTICS */
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
/** @} */
