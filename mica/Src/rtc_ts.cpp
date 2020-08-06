
#include "rtc_ts.h"

extern RTC_HandleTypeDef hrtc;


time_t RTC_GetTimestamp(RTC_HandleTypeDef *hrtc, time_t timestamp)
{
	time_t tmp_ts = 0U;
	tmp_ts = READ_REG(hrtc->Instance->CNTH & RTC_CNTH_RTC_CNT) << 16;
	tmp_ts |= READ_REG(hrtc->Instance->CNTL & RTC_CNTL_RTC_CNT);
	return tmp_ts;
}

HAL_StatusTypeDef RTC_SetTimestamp(RTC_HandleTypeDef *hrtc, time_t timestamp)
{
	uint32_t tickstart = 0U;

	tickstart = HAL_GetTick();
	/* Wait till RTC is in INIT state and if Time out is reached exit */
	while ((hrtc->Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET)
	{
		if ((HAL_GetTick() - tickstart) >  RTC_TIMEOUT_VALUE)
		{
			return HAL_TIMEOUT;
		}
	}
	SET_BIT((hrtc)->Instance->CRL, RTC_CRL_CNF);
	/* Set RTC COUNTER MSB word */
	WRITE_REG(hrtc->Instance->CNTH, (timestamp >> 16U));
	/* Set RTC COUNTER LSB word */
	WRITE_REG(hrtc->Instance->CNTL, (timestamp & RTC_CNTL_RTC_CNT));
	
	CLEAR_BIT((hrtc)->Instance->CRL, RTC_CRL_CNF);

	tickstart = HAL_GetTick();
	/* Wait till RTC is in INIT state and if Time out is reached exit */
	while ((hrtc->Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET)
	{
		if ((HAL_GetTick() - tickstart) >  RTC_TIMEOUT_VALUE)
		{
			return HAL_TIMEOUT;
		}
	}
}

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

