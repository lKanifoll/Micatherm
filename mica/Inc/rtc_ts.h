#pragma once
#include "time.h"
#include "main.h"

time_t RTC_timestamp_get(uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t min, uint8_t sec);
tm RTC_timestamp_set(time_t);
HAL_StatusTypeDef RTC_SetTimestamp(RTC_HandleTypeDef *hrtc, time_t timestamp);
time_t RTC_GetTimestamp(RTC_HandleTypeDef *hrtc, time_t timestamp);
HAL_StatusTypeDef RTC_SetTimestampAlarm(RTC_HandleTypeDef *hrtc, time_t timestamp);