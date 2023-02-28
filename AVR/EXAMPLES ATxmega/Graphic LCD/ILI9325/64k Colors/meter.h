/**********************************************************
Solomon Systech SSD1289 Color TFT LCD controller demo.
Functions used for displaying a meter gauge.

(C) Copyright 2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
**********************************************************/

#ifndef METER_INCLUDED
#define METER_INCLUDED

#include <glcd.h>

typedef struct
        {
        GLCDX_t xc; // meter center x coordinate
        GLCDY_t yc; // meter center y coordinate
        GLCDRAD_t radius; // meter radius
        int min_value; // minimum scale value
        int max_value; // maximum scale value
        int crt_value; // current meter value
        unsigned char ticks; // # of scale ticks
        unsigned char tick_len; // tick length
        } meter_t;

/* display a meter */
void meter_init(meter_t *pm);
/* updates the meter indication */
void meter_update(meter_t *pm, int value);

#endif
