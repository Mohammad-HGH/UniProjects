
typedef struct {
	unsigned short	year;	/* 2000..2099 */
	unsigned char	month;	/* 1..12 */
	unsigned char	mday;	/* 1.. 31 */
	unsigned char	hour;	/* 0..23 */
	unsigned char	min;	/* 0..59 */
	unsigned char	sec;	/* 0..59 */
} RTC;

void rtc_initialize (void); /* Initialize RTC */
void rtc_gettime (RTC*);	   /* Get time */
void rtc_settime (RTC*);	   /* Set time */

