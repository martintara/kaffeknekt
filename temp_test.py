import serial
import time
from datetime import datetime, timezone
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

# InfluxDB settings
URL = 'http://localhost:8086'
ORG = 'Kaffeknekt'
TOKEN = 'ETV_6VBhkfF7HzNGfOjN6F7nTvX0ye_tblcGObcB1OVJDLYxQXUWpt8NU84PJmrn6R6IV921X2eWLLJDg1wgdQ=='  # Paste your token here
BUCKET = 'sensor_data'

client = InfluxDBClient(url=URL, token=TOKEN, org=ORG)
API = client.write_api(write_options=SYNCHRONOUS)  # Ensures all writes are sent immediately

# Serial settings
SERIAL_PORT = '/dev/ttyUSB0'
BAUD_RATE = 115200
OUTPUT_FILE = 'serial_log.txt'

def main():
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser, open(OUTPUT_FILE, 'a') as file:
            print(f"Listening on {SERIAL_PORT} at {BAUD_RATE} baud...")
            while True:
                line = ser.readline().decode('utf-8').strip()
                if line:
                    timestamp = datetime.now(timezone.utc)
                    log_line = f"{timestamp} - {line}"
                    print(log_line)
                    file.write(log_line + '\n')
                    file.flush()

                    try:
                        temp_val = float(line)
                        point = Point("Temperature") \
                            .field("temperature_C", temp_val) \
                            .time(timestamp)
                        API.write(bucket=BUCKET, org=ORG, record=point, write_precision='s')
                    except ValueError:
                        print(f"⚠️ Could not parse value as float: {line}")

    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except KeyboardInterrupt:
        print("\nStopped by user.")

if __name__ == "__main__":
    main()
