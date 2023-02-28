/**********************************************************
Solomon Systech SSD1963 Color TFT LCD controller demo.
Functions used for displaying a button.

(C) Copyright 2011-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
**********************************************************/

#include <stdbool.h>
#include "button.h"

/* initializes a button */
void button_init(button_t *pb)
{
pb->pressed=false; /* button not pressed by default */
/* calculate glyph's and caption's coordinates */
if (pb->glyph)
   {
   /* take in account the glyph's width
      which is stored as an unsigned int in the first 2 bytes */
   pb->cx=((flash unsigned int *) pb->glyph)[0]+
     /* and add a space between the glyph and caption */
     glcd_charwidth(' ');
   /* get the glyph left coordinate */
   pb->gx=pb->left+(pb->width-(pb->cx+glcd_textwidthf(pb->caption)))/2;
   /* get glyph top coordinate so that it will centered vertically
      the glyph height is stored as an unsigned int in bytes 3 & 4 */
   pb->gy=pb->top+(pb->height-((flash unsigned int *) pb->glyph)[1])/2;
   /* get caption left display coordinate */
   pb->cx+=pb->gx;
   } 
else
   /* no glyph, get caption left display coordinate */
   pb->cx=pb->left+(pb->width-glcd_textwidthf(pb->caption))/2;
/* set the text font for caption */
glcd_setfont(pb->font);
/* get caption top display coordinate */   
pb->cy=pb->top+(pb->height-glcd_textheight())/2;
/* set button border color */
glcd_setcolor(pb->border_color);
/* use 1 pixel solid line for border */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
/* draw border as a rectangle with rounded corners */
glcd_rectround(pb->left,pb->top,pb->width,pb->height,4);
/* draw the rest of the button */
button_update(pb);
}

/* updates the button display */
void button_update(button_t *pb)
{
/* save the current text transparent mode */
bool tp_mode=glcd_state.text.transparent;
/* use text transparent mode */
glcd_transparent(true);
/* set text color for caption */
glcd_setcolor(pb->text_color);
/* set the text font for caption */
glcd_setfont(pb->font);
/* set button background color for fill */
glcd_setfillcolor(pb->pressed ? pb->pressed_color : pb->bg_color);
/* fill the middle of the button with background color */ 
glcd_barrel(pb->left+1,pb->top+4,pb->width-2,pb->height-8);
/* if used, display the glyph in transparent mode */
if (pb->glyph)
   {
   glcd_settpcolor(pb->glyph_tp_color);
   glcd_putimagef(pb->gx,pb->gy,pb->glyph,GLCD_PUTTP);
   }
/* display the caption */
glcd_outtextxyf(pb->cx,pb->cy,pb->caption);
/* fill the upper & lower parts of the button */
glcd_floodfill(pb->cx,pb->top+2,pb->border_color);
glcd_floodfill(pb->cx,pb->top+pb->height-2,pb->border_color);
/* restore current text transparent mode */
glcd_transparent(tp_mode);
}
