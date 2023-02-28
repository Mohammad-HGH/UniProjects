/**********************************************************
Solomon Systech SSD2119 Color TFT LCD controller demo.
Functions used for displaying a horizontal bar-graph gauge.

(C) Copyright 2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
**********************************************************/

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "bargraph.h"
#include "font5x7.h"

/* print a scale value */
static void print_scale(bargraph_t *pb, GLCDX_t dx, int value)
{
char buf[7];

/* convert value to decimal */
itoa(value,buf);
/* display the value */
/* ensure the displayed value will fit in the middle of the x coordinate */
glcd_outtextxy(pb->xt+dx-glcd_textwidth(buf)/2,pb->yt,buf);
}

/* print current value */
static void print_value(bargraph_t *pb)
{
char buf[7]="      ";
GLCDY_t y;
GLCDCOL_t old_color=glcd_getcolor();

glcd_setcolor(GLCD_CL_WHITE);
y=pb->yb+pb->bar_height+2;
/* delete previous value */
glcd_transparent(false);
glcd_outtextxy(pb->xt+(pb->width-glcd_textwidth(buf))/2,y,buf);
glcd_transparent(true);
/* convert value to decimal */
itoa(pb->crt_value,buf);
/* display the value */
glcd_outtextxy(pb->xt+(pb->width-glcd_textwidth(buf))/2,y,buf);
glcd_setcolor(old_color);
}

/* display a bar-graph gauge */
void bargraph_init(bargraph_t *pb)
{
int n;
GLCDX_t x,dx;
GLCDY_t y;

/* display scale */
glcd_setfont(font5x7);
glcd_setcolor(GLCD_CL_YELLOW);
pb->tick_len=pb->height/10;
y=pb->yt+pb->tick_len+glcd_textheight();
glcd_line(pb->xt,y,pb->xt+pb->width,y);
x=pb->xt;
dx=pb->width/(pb->ticks-1);
for (n=0; n<pb->ticks; n++)
    {
    glcd_moveto(x,y);
    glcd_linerel(0,-(int) pb->tick_len);
    x+=dx;
    }
    
/* print scale values */    
glcd_setcolor(GLCD_CL_GREEN);
print_scale(pb,0,pb->min_value);
print_scale(pb,pb->width/2,(pb->max_value-pb->min_value)/2);
print_scale(pb,pb->width,pb->max_value);

/* get bar height */
pb->bar_height=pb->height-pb->tick_len-2*glcd_textheight()-5;
/* get bar top coordinate */
pb->yb=y+3;
/* get scale factor used for displaying bar length */
pb->scale_factor=(float) pb->width/(pb->max_value-pb->min_value);
/* set initial value for bar-graph */
pb->crt_value=pb->min_value;
pb->bar_length=0;
}

static GLCDX_t roundx(float x)
{
GLCDX_t result;
result=floor(x);
if ((x-result)>=0.5) ++result;
return result;
}

/* updates the bar-graph gauge indication */
void bargraph_update(bargraph_t *pb, int value)
{
GLCDCOL_t old_color; 
GLCDX_t old_length,new_length;

if (value==pb->crt_value) return;

old_length=pb->bar_length;
new_length=roundx((value-pb->min_value)*pb->scale_factor);

old_color=glcd_getfillcolor();
if (value>pb->crt_value)
   {
   /* add a portion of a bar */
   glcd_setfillcolor(GLCD_CL_RED);
   glcd_barrel(pb->xt+old_length,pb->yb,new_length-old_length,pb->bar_height);
   }
else
   {
   /* remove a portion of a bar */
   glcd_setfillcolor(glcd_getbkcolor());
   glcd_barrel(pb->xt+new_length,pb->yb,old_length-new_length,pb->bar_height);
   }
glcd_setfillcolor(old_color);

pb->crt_value=value;
pb->bar_length=new_length;
print_value(pb);
}

