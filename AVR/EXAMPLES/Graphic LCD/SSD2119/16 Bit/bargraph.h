/**********************************************************
Solomon Systech SSD2119 Color TFT LCD controller demo.
Functions used for displaying a horizontal bar-graph gauge.

(C) Copyright 2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
**********************************************************/

#ifndef BARGRAPH_INCLUDED
#define BARGRAPH_INCLUDED

#include <glcd.h>

typedef struct
        {
        GLCDX_t xt; // bar-graph top left x coordinate
        GLCDY_t yt; // bar-graph top left y coordinate
        GLCDX_t width; // total bar-graph width
        GLCDY_t height; // total bar-graph height
        int min_value; // minimum scale value
        int max_value; // maximum scale value
        int crt_value; // current meter value
        unsigned char ticks; // # of scale ticks
        unsigned char tick_len; // tick length
        GLCDY_t yb; // meter bar top y coordinate
        GLCDY_t bar_height; // meter bar height
        GLCDX_t bar_length; // current bar length
        float scale_factor;
        } bargraph_t;

/* display a bar-graph gauge */
void bargraph_init(bargraph_t *pb);
/* updates the bar-graph gauge indication */
void bargraph_update(bargraph_t *pb, int value);

#endif
