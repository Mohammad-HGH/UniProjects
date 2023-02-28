/* Touchscreen calibration functions */

#ifndef _CALIBRATE_INCLUDED_
#define _CALIBRATE_INCLUDED_

#include <stdbool.h>
/* Resistive touchscreen functions */
#include <rtouch.h>

/* Display a cross for a calibration point */
void display_rt_point(RTPOINT_t p);

/* Erase the cross for a calibration point */
void erase_rt_point(RTPOINT_t p);

/* Calibrate the touch screen*/
bool calibrate_touchscreen(void);

#endif

