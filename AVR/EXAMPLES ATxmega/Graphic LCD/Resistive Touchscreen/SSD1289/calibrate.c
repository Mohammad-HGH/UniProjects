/* Touchscreen calibration helper functions */

#include "calibrate.h"

#include <glcd.h>
#include <stdio.h>
#include <delay.h>

/* (x,y) Coordinates of the three touch screen calibration points */
flash RTCALP_t display_calibration_points=
{{10,30},/* horizontal left-vertical top point */
{220,159}, /* horizontal right-vertical center point */
{119,310}}; /* horizontal center-vertical bottom point */

/* Display buffer */
extern char display_buf[];

/* Display a cross using the current foreground color */
void display_cross(RTPOINT_t p)
{
short text_width,xt;
short text_height,yt,t;
/* Display the cross */
glcd_line(p.x-5,p.y,p.x+5,p.y);
glcd_line(p.x,p.y-5,p.x,p.y+5);
/* Prepare to display cross center coordinates */
sprintf(display_buf,"(%d,%d)",p.x,p.y);
/* Establish displayed text size */
text_width=glcd_textwidth(display_buf);
text_height=glcd_textheight();
/* Try to display the text centered under the cross */
if ((t=p.y+5+text_height)<_GLCD_MAXY_) goto l0;
/* Try to display the text centered above the cross */
if ((t=p.y-5-text_height)>=0)
   {
   l0:
   yt=t;
   /* Ensure the text fits the screen horizontally */
   xt=p.x-text_width/2;
   if (xt<0) xt=0;
   else if ((xt+text_width)>=_GLCD_MAXX_) xt=_GLCD_MAXX_-1-text_width;
   }
/* If not possible, display on the same line with the cross */
else
    {
    yt=p.y-text_height/2;
    /* Try to display the text to the right of the cross */
    if ((t=p.x+5+text_width)<_GLCD_MAXX_) xt=t;
    /* Try to display the text to the left of the cross */
    else if ((t=p.x-5-text_width)>=0) xt=t;
    else xt=p.x-5;
    }
/* Display the cross center coordinates */
glcd_outtextxy(xt,yt,display_buf);
}

/* Display a cross for a calibration point */
void display_rt_point(RTPOINT_t p)
{
glcd_setcolor(GLCD_CL_WHITE);
/* Draw the cross with white color */
display_cross(p);
}

/* Erase the cross for a calibration point */
void erase_rt_point(RTPOINT_t p)
{
GLCDCOL_t c;
/* Save foreground color */
c=glcd_getcolor();
/* Set foreground color same as background */ 
glcd_setcolor(glcd_getbkcolor()); 
/* Draw the cross with the background color (erase it) */
display_cross(p);
/* Restore the foreground color */
glcd_setcolor(c); 
}

/* Calibrate the touch screen*/
bool calibrate_touchscreen(void)
{
RTCALP_t rt_calibration_points;

glcd_outtextxyf(0,10,"Touch cross center to calibrate");

/* Display calibration points and read the touchscreen result for each of them */

/* horizontal left-vertical top point */
display_rt_point(display_calibration_points.left_top);
rt_readctrl(&rt_calibration_points.left_top.x,&rt_calibration_points.left_top.y);
erase_rt_point(display_calibration_points.left_top);

delay_ms(1000);
   
/* horizontal right-vertical center point */
display_rt_point(display_calibration_points.right_center);
rt_readctrl(&rt_calibration_points.right_center.x,&rt_calibration_points.right_center.y);
erase_rt_point(display_calibration_points.right_center);

delay_ms(1000);
   
/* horizontal center-vertical bottom point */
display_rt_point(display_calibration_points.center_bottom);
rt_readctrl(&rt_calibration_points.center_bottom.x,&rt_calibration_points.center_bottom.y);
erase_rt_point(display_calibration_points.center_bottom);
   
/* Calculate the calibration coeficients and store them to EEPROM */
return rt_calibrate(&rt_calibration_points,&display_calibration_points);
}

