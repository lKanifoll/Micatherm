#pragma once
#include "time.h"


time_t RTC_timestamp_get(uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t min, uint8_t sec);
tm RTC_timestamp_set(time_t);