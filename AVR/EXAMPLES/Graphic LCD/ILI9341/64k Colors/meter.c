/**********************************************************
Solomon Systech SSD1289 Color TFT LCD controller demo.
Functions used for displaying a meter gauge.

(C) Copyright 2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
**********************************************************/

#include <math.h>
#include <stdlib.h>
#include "meter.h"
#include "font5x7.h"

/* meter arc start/end angles [degrees] */
#define METER_START_ANGLE 35
#define METER_END_ANGLE 145

/* print a scale value */
static void print_scale(meter_t *pm, float angle, int value)
{
char buf[7];
GLCDRAD_t r0=pm->radius+glcd_textheight()*1.3;

/* convert value to decimal */
itoa(value,buf);
/* display value */
glcd_outtextxy(pm->xc+r0*cos(angle),pm->yc-r0*sin(angle),buf);
}

/* displays the meter needle for the crt_value */
static void display_indicator(meter_t *pm, GLCDCOL_t color)
{
GLCDCOL_t old_color;
GLCDX_t xe;
GLCDY_t ye;
float a,nx,ny,r0,r1;

/* check displayed value range */
if (pm->crt_value>=pm->max_value) pm->crt_value=pm->max_value;
else if (pm->crt_value<=pm->min_value) pm->crt_value=pm->min_value;

a=(METER_END_ANGLE- (float)(METER_END_ANGLE-METER_START_ANGLE)*(pm->crt_value-pm->min_value)/(pm->max_value-pm->min_value))*PI/180.0;
nx=cos(a);
ny=sin(a);
r0=(float) (pm->radius/12);
r1=pm->radius-pm->tick_len-2;
xe=pm->xc+r1*nx;
ye=pm->yc-r1*ny;

old_color=glcd_getcolor();
glcd_setcolor(color);
glcd_line(pm->xc+r0*nx,pm->yc-r0*ny,xe,ye);

r0=2.0*pm->tick_len;
if ((a>0.25*PI) && (a<=0.75*PI)) a=-(0.75+0.125)*PI-a;
else a=(1.25-0.125)*PI-a;
glcd_linerel(r0*cos(a),r0*sin(a));

a-=0.25*PI;
glcd_moveto(xe,ye);
glcd_linerel(r0*cos(a),r0*sin(a));

glcd_setcolor(old_color);
}
        
/* display a meter */
void meter_init(meter_t *pm)
{
unsigned char i;
GLCDRAD_t r0,r1;
float tick_angle;
float nx,ny;
float angle_2ticks=(METER_END_ANGLE-METER_START_ANGLE)*PI/180.0/(pm->ticks-1); 

/* draw the meter arc of circle */
glcd_setcolor(GLCD_CL_YELLOW);
glcd_setlinethick(2);
glcd_arc(pm->xc,pm->yc,METER_START_ANGLE,METER_END_ANGLE,pm->radius);

/* draw the ticks */
tick_angle=METER_END_ANGLE*PI/180.0;
pm->tick_len=pm->radius/10;
r1=pm->radius-1;
r0=r1-pm->tick_len;
glcd_setlinethick(1);
for (i=0; i<pm->ticks; i++)
    {
    nx=cos(tick_angle);
    ny=sin(tick_angle);
    glcd_line(pm->xc+r0*nx,pm->yc-r0*ny,
              pm->xc+r1*nx,pm->yc-r1*ny);
    tick_angle-=angle_2ticks;
    }

/* print scale values */    
glcd_setfont(font5x7);
glcd_setcolor(GLCD_CL_GREEN);
print_scale(pm,METER_END_ANGLE*PI/180.0,pm->min_value);
print_scale(pm,(METER_START_ANGLE+(METER_END_ANGLE-METER_START_ANGLE)/2)*PI/180.0,
           (pm->max_value-pm->min_value)/2);
print_scale(pm,METER_START_ANGLE*PI/180.0,pm->max_value);

/* draw filled center circle */
glcd_setfillcolor(GLCD_CL_YELLOW);
glcd_fillcircle(pm->xc,pm->yc,pm->radius/12);

/* display indicator for initial value */
pm->crt_value=pm->min_value;
display_indicator(pm,GLCD_CL_RED);
}

/* updates the meter indication */
void meter_update(meter_t *pm, int value)
{
if (pm->crt_value!=value)
   {
   /* delete old indicator */
   display_indicator(pm,glcd_getbkcolor());
   /* update the current value */
   pm->crt_value=value;
   /* draw new indicator */
   display_indicator(pm,GLCD_CL_RED);
   }
}
