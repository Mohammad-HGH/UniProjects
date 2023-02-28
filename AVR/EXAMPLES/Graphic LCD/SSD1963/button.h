/**********************************************************
Solomon Systech SSD1963 Color TFT LCD controller demo.
Functions used for displaying a button.

(C) Copyright 2011-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
**********************************************************/

#ifndef BUTTON_INCLUDED
#define BUTTON_INCLUDED

#include <glcd.h>

typedef struct
        {
        GLCDX_t left; // button top left x coordinate
        GLCDY_t top; // button top left y coordinate
        GLCDX_t width; // button width
        GLCDY_t height; // button height
        GLCDCOL_t border_color; // button border color
        GLCDCOL_t bg_color; // not pressed button background color
        GLCDCOL_t pressed_color; // pressed button background color
        GLCDCOL_t text_color; // button text color
        GLCDCOL_t glyph_tp_color; // glyph's transparent color
        flash char *glyph; // ptr. to image glyph
        GLCDX_t gx; // glyph left coordinate
        GLCDY_t gy; // glyph top coordinate
        flash char *caption; // button caption
        GLCDX_t cx; // caption left coordinate
        GLCDY_t cy; // caption top coordinate
        flash char *font; // caption font
        unsigned char pressed:1; // button is pressed 
        } button_t;

/* initializes a button */
void button_init(button_t *pb);
/* updates the button display */
void button_update(button_t *pb);

#endif
