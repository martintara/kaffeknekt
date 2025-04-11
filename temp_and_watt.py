import serial
import json
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime, timezone
from dateutil.parser import parse

# InfluxDB settings
URL = 'http://localhost:8086'
ORG = 'kaffeknekt_admin'
TOKEN = 'zIKDeYY-92Yyzcs2FTe5q5ZBh9laYZ9ZlwsoIDR1E2Alz26gKoobMmd93xug_e27QFCdMJclWOSWwsMDadbXXQ=='
BUCKET = 'sensor_data'

client = InfluxDBClient(url=URL, token=TOKEN, org=ORG)
API = client.write_api(write_options=SYNCHRONOUS)

SERIAL_PORT = '/dev/ttyUSB0'
BAUD_RATE = 115200

def main():
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
            print(f"Listening on {SERIAL_PORT} at {BAUD_RATE} baud...")
            while True:
                line = ser.readline().decode('utf-8').strip()
                if line:
                    try:
                        data = json.loads(line)
                        print("Received:", data)

                        human_time = datetime.fromtimestamp(data["timestamp_ns"] / 1e9).replace(microsecond=0)
                        iso = human_time.isoformat()
                        date_part, time_part = iso.split("T")
                        readable_time = f"{date_part} {time_part}"

                        point = Point("Esp32Metrics") \
                            .field("temperature_C", data["temperature_C"]) \
                            .field("power_mW", data["power_mW"]) \
                            .field("readable_time", readable_time) \
                            .time(data["timestamp_ns"]) 

                        API.write(bucket=BUCKET, org=ORG, record=point, write_precision='ns')
                        print("✅ Wrote to Influx")

                    except json.JSONDecodeError:
                        print("⚠️ Invalid JSON:", line)
                    except KeyError as e:
                        print(f"⚠️ Missing expected key: {e}")
    except KeyboardInterrupt:
        print("\nStopped by user.")
    except serial.SerialException as e:
        print(f"Serial error: {e}")

if __name__ == "__main__":
    main()
