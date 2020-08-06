#include "rtc_ts.h"




time_t RTC_timestamp_get(uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t min, uint8_t sec)
{
	struct tm t;
	time_t t_of_day;

	t.tm_year = year - 1900;    // Year - 1900
	t.tm_mon = month;             // Month, where 0 = jan
	t.tm_mday = day;            // Day of the month
	t.tm_hour = hour;
	t.tm_min = min;
	t.tm_sec = sec;
	t.tm_isdst = 0;         // Is DST on? 1 = yes, 0 = no, -1 = unknown
	t_of_day = mktime(&t);
	return t_of_day;
}

tm RTC_timestamp_set(time_t timestamp)
{
	time_t rawtime = timestamp;
	struct tm  ts;
	ts = *localtime(&rawtime);
	return ts;
}

