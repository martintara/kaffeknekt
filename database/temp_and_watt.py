import serial
import json
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime, timezone
from dateutil.parser import parse
import socket
import os

# InfluxDB settings
URL = 'http://localhost:8086'
ORG = 'Kaffeknekt'
TOKEN = 'dlatvWih4FyEIXjqDQS0L1LVU33YcUzdcq5FF81z_cJ6Mw6cnqe5mn_C__n3pDt0rmFKqRXjcWddyRk2DXyLiQ=='
TOKEN_2 = 'ETV_6VBhkfF7HzNGfOjN6F7nTvX0ye_tblcGObcB1OVJDLYxQXUWpt8NU84PJmrn6R6IV921X2eWLLJDg1wgdQ=='
BUCKET = 'sensor_data'

client = InfluxDBClient(url=URL, token=TOKEN_2, org=ORG)
API = client.write_api(write_options=SYNCHRONOUS)

serial_port = '/dev/ttyUSB0'
baud_rate = 115200

def main():

    clk = 0 #counter to add milliseconds to every other datawrite to avoid duplicate timestamps
    session = 1

    #Define socket path
    path = '/tmp/socket'

    #Declare socket 
    client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) 
    
    try:
        with serial.Serial(serial_port, baud_rate, timeout=1) as ser:
            print("Esp connected")
            while True:
                line = ser.readline().decode('utf-8').strip() #reads line, converts to string, and cleans excess characters
                if line: #checks if line exists
                    try:
                        data = json.loads(line)
                        print(data)

                        datastamp = "timestamp"

                        stamp = datetime.fromtimestamp(int(data["timestamp"]) / 1e9).replace(microsecond=0)
                        iso = stamp.isoformat()
                        date_part, time_part = iso.split("T")
                        readable_time = f"{date_part} {time_part}"

                        #prøver på sensor adding flexibility
                        #Inisialiserer Point() bygging
                        point = Point("Esp32Metrics") \
                        .field("readable_time", readable_time)

                        if data["flag"] == "U":
                            session = 1
                        elif data["flag"] == "D":
                            session = 0

                        #if session == 1:
                        #    point = point.tag("sessionStatus", active)
                        #else:
                        #    point = point.tag("sessionStatus", inactive)


                        #Add flag to point from "data" if it exists, else add default value
                        
                        #try: 
                        #    point = point.field("flag", data["flag"]) 
                        #except KeyError: 
                        #    if session == 1:
                        #        point = point.field("flag", str("U"))
                        #    elif session == 0:
                        #        point = point.field("flag", str("D"))
                        
                        #if session == 1:
                         #   point = point.field("flag", data.get('flag', "1"))
                        #else:
                         #   point = point.field("flag", data.get('flag', "0"))

                        if clk == 0:
                            clk = 1
                        elif clk == 1:
                            clk = 0
                       
                        #adder dataen for hver verdi i rekken
                        for field, value in data.items():
                            if field != "timestamp" and field != "flag":
                                point = point.field(field, float(round(value, 3)))
                                
                            #elif field == "flag":
                                #point = point.field(field, str(value))

                            elif field == "timestamp":
                                point = point.time(int(value + clk*5*1e8 - (7200 * 1e9))) #temporary corrected time for random_generator
                        
                        #point = Point("Esp32Metrics") \
                            #.field("temperature_C", data["temperature_C"]) \
                            #.field("power_mW", data["power_mW"]) \
                            #.field("readable_time", readable_time) \
                            #.time(data["timestamp_ns"]) 

                        #Printing point to console to compare with json string
                        print(point)
                        API.write(bucket=BUCKET, org=ORG, record=point, write_precision='ns')
                        print("Data written to Influx")
                        

                    except json.JSONDecodeError:
                        print("Invalid JSON:", line)
                    except KeyError as e:
                        print(f"{e} is missing.")
    except KeyboardInterrupt:
        print("\nStopped by user.")
    except serial.SerialException as e:
        print(f"Serial error: {e}")

if __name__ == "__main__":
    main()
