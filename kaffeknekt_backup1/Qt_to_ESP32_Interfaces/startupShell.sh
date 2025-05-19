#!bin/bash

#Start influx evironment
source /home/kaffeknekt/influx_env/bin/activate

#Start python script that controls sensor data to influxdb and socket
python /home/kaffeknekt/kaffeknekt/temp_and_watt.py

#Start QT app
#/home/kaffeknekt/kaffeknekt/qtApp