import serial
import json
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime, timezone
from dateutil.parser import parse

# InfluxDB settings
URL = 'http://localhost:8086'
ORG = 'Kaffeknekt'
TOKEN = 'dlatvWih4FyEIXjqDQS0L1LVU33YcUzdcq5FF81z_cJ6Mw6cnqe5mn_C__n3pDt0rmFKqRXjcWddyRk2DXyLiQ=='
BUCKET = 'sensor_data'

client = InfluxDBClient(url=URL, token=TOKEN, org=ORG)
API = client.write_api(write_options=SYNCHRONOUS)

serial_port = '/dev/ttyUSB0'
baud_rate = 115200

def main():
    try:
        with serial.Serial(serial_port, baud_rate, timeout=1) as esp:
            print("Esp connected")
            while True:
                line = esp.readline().decode('utf-8').strip() #reads line, converts to string, and cleans excess characters
                if line: #checks if line exists
                    try:
                        data = json.loads(line)
                        print(data)

                        datastamp = "timestamp"

                        stamp = datetime.fromtimestamp(data["timestamp"] / 1e9).replace(microsecond=0)
                        iso = stamp.isoformat()
                        date_part, time_part = iso.split("T")
                        readable_time = f"{date_part} {time_part}"

                        #prøver på sensor adding flexibility
                        #Inisialiserer Point() bygging
                        point = Point("Esp32Metrics") \
                        .field("readable_time", readable_time)

                        #adder dataen for hver verdi i rekken
                        for field, value in data.items():
                            if field != "flag" and "timestamp":
                                point = point.field(field, float(value))
                                
                            elif field == "flag":
                                point = point.field(field, str(value))

                            else:
                                point = point.time(value)
                        # 

                        #point = Point("Esp32Metrics") \
                            #.field("temperature_C", data["temperature_C"]) \
                            #.field("power_mW", data["power_mW"]) \
                            #.field("readable_time", readable_time) \
                            #.time(data["timestamp_ns"]) 

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
