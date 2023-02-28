/*--------------------------------------------------------------------------*/
/*  PCF8563 RTC controls                                                    */

#include "rtc.h"
#include "pcf8563.h"
#include <ff.h>

void rtc_gettime(RTC *rtc)
{
rtc_get_time(&(rtc->hour),&(rtc->min),&(rtc->sec));
rtc_get_date(&(rtc->mday),&(rtc->month),&(rtc->year));
}

void rtc_settime(RTC *rtc)
{
rtc_set_time(rtc->hour,rtc->min,rtc->sec);
rtc_set_date(rtc->mday,rtc->month,rtc->year);
}

void rtc_initialize(void)
{
/* Pointer to a Real Time Clock function used for reading time */
prtc_get_time= (void (*)(unsigned char *, unsigned char *, unsigned char *)) rtc_get_time;
/* Pointer to a Real Time Clock function used for reading date */
prtc_get_date= rtc_get_date;

i2c_init();
rtc_init(0 /* no RTC interrupts */,RTC_CLKOUT_1,RTC_TIMER_CLK_1);
}

