#!bin/bash

#Start influx evironment
source /home/kaffeknekt/influx_env/bin/activate

#Start python script that controls sensor data to influxdb and socket
python /home/kaffeknekt/kaffeknekt/database/influxsocketesp_trinity.py

#Start QT app
/home/kaffeknekt/kaffeknekt/qtApp