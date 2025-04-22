# DFRobot_DS323X
* [中文版](./README_CN.md)

DS323X is a low-cost, extremely accurate, I2C real-time clock(RTC) module. It can provide ±5%ppm accuracy (no more than 0.432s error) within the whole temperature range and lifespan, and is compatible with 2.5-5.5V wide voltage range. The current comsumption of the module is as low as 2uA when powered by batteries. This module can be used to measure ambient temperature with ±3℃ accuracy.   <br>

![产品实物图](../../resources/images/DS3232.png)


## Product Link (https://www.dfrobot.com/search-DS323.html)
    SKU：DFR0641/DFR0819/DFR0821


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

* Get real-time, the initial time needs to be set by users. You can get compiled time, NTP time or set time by yourself, the minimum unit: s <br> 
* Set the alarm clock, users can set two alarm clocks and can get the falling edge pulse at the interrupt pin to trigger the alarm clock <br>
* Measure the chip temperature, error: ±0.5℃ <br>
* Read and write 236 bytes of SRAM inside DS3232 chip <br> 


## Installation

To use the library, first download the library file, paste it into the specified directory, then open the examples folder and run the demo in that folder.


## Methods

```Python

  '''!
    @brief Init chip 
    @return True means IIC communication succeeds, false means it fails. 
  '''
  def begin():

  '''!
    @brief get year of now time
    @return year
  '''
  def get_year():

  '''!
    @brief get the month of now time
    @return month
  '''
  def get_month():

  '''!
    @brief get the date of now time
    @return date
  '''
  def get_date():

  '''!
    @brief get hour of now time
    @return hour
  '''
  def get_hour():

  '''!
    @brief get minute of now time
    @return minute
  '''
  def get_minute():

  '''!
    @brief get second of now time
    @return second
  '''
  def get_second():

  '''!
    @brief get the day of week
    @return day of week
  '''
  def get_day_of_week():

  '''!
    @brief Set mode of time
    @param mode H24hours, H12hours
  '''
  def set_hour_system(self, mode = H24hours):

  '''!
    @brief Set time into rtc and take effect immediately
    @param year, 1900~2100
    @param month, 1~12
    @param date, 1~31
    @param hour, 0-23
    @param minute, 0~59
    @param second, 0~59
  '''
  def set_time(year, month, date, hour, minute, second):

  '''!
    @brief get time mode of now
    @return time mode
  '''
  def get_AM_or_PM():

  '''!
    @brief get temperature of sensor, unit:℃
    @param mode Select the sensor temperature measurement mode
    @n true Enable temperature conversion to measure and obtain the current temperature immediately
    @n false Disable temperature conversion and obtain the temperature measurement data within 64 seconds
    @n       (the sensor measures the temperature every 64 seconds by default)
    @return temperature, unit:℃
  '''
  def get_temperature_C(mode=False):

  '''!
    @brief get temperature of sensor, unit:℉
    @param mode Select the sensor temperature measurement mode
    @n true Enable temperature conversion to measure and obtain the current temperature immediately
    @n false Disable temperature conversion and obtain the temperature measurement data within 64 seconds
    @n       (the sensor measures the temperature every 64 seconds by default)
    @return temperature, unit:℉
  '''
  def get_temperature_F(mode=False):

  '''!
    @brief check if rtc has lost power
    @return True means rtc has lost power before and need to reset time;False means rtc opearates well
  '''
  def is_lost_power():

  '''!
    @brief Read the value of pin sqw
    @return mode SquareWave_OFF  = 0x1C # Off
    @n           SquareWave_1Hz  = 0x00 # 1Hz square wave
    @n           SquareWave_1kHz = 0x08 # 1kHz square wave
    @n           SquareWave_4kHz = 0x10 # 4kHz square wave
    @n           SquareWave_8kHz = 0x18 # 8kHz square wave
  '''
  def read_sqw_pin_mode():

  '''!
    @brief Set the vaule of pin sqw
    @param mode SquareWave_OFF  = 0x1C # Not output square wave, enter interrupt mode
    @n          SquareWave_1Hz  = 0x00 # 1Hz square wave
    @n          SquareWave_1kHz = 0x08 # 1kHz square wave
    @n          SquareWave_4kHz = 0x10 # 4kHz square wave
    @n          SquareWave_8kHz = 0x18 # 8kHz square wave
  '''
  def write_sqw_pin_mode(mode):

  '''!
    @brief Set alarm1 clock
    @param alarmType:EverySecond,
    @n               SecondsMatch,
    @n               SecondsMinutesMatch,
    @n               SecondsMinutesHoursMatch,
    @n               SecondsMinutesHoursDateMatch,
    @n               SecondsMinutesHoursDayMatch, #Alarm1
    @n               UnknownAlarm
    @param days    1-31
    @param hours   0-23
    @param minutes 0-59
    @param seconds 0-59
  '''
  def set_alarm1(alarmType, date, hour, minute, second):

  '''!
    @brief Set alarm2 clock
    @param alarmType:EveryMinute,
    @n               MinutesMatch,
    @n               MinutesHoursMatch,
    @n               MinutesHoursDateMatch,
    @n               MinutesHoursDayMatch,        #Alarm2
    @n               UnknownAlarm
    @param days    1-31
    @param hours   0-23
    @param minutes 0-59
  '''
  def set_alarm2(alarmType, date, hour, minute):

  '''!
    @brief check if alarm flag has been triggered
  '''
  def is_alarm_trig():

  '''!
    @brief clear alarm flag triggered
  '''
  def clear_alarm():

  '''!
    @brief enable the interrupt of alarm 1
  '''
  def enable_alarm1_int():

  '''!
    @brief disable the interrupt of alarm 1
  '''
  def disable_alarm1_int():

  '''!
    @brief enable the interrupt of alarm 2
  '''
  def enable_alarm2_int():

  '''!
    @brief disable the interrupt of alarm 2
  '''
  def disable_alarm2_int():

  '''!
    @brief enable the 32k output 
  '''
  def enable_32k():

  '''!
    @brief disable the 32k output 
  '''
  def disable_32k():

  '''!
    @brief clear data on the SRAM of DS3232
    @param addr address of SRAM
  '''
  def clear_SRAM(addr)

  '''!
    @brief write data on the SRAM of DS3232
    @param addr address of SRAM
    @param data Written data
  '''
  def write_SRAM(addr, data)

  '''!
    @brief read data on the SRAM of DS3232
    @param addr address of SRAM
    @return Read the data
  '''
  def read_SRAM(addr)

```

## Compatibility

* RaspberryPi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## History

- 2021/10/25 - Version 1.0.0 released.


## Credits

Written by(yufeng.luo@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

