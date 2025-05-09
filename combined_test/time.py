import serial
from datetime import datetime

# Update port to match your setup (e.g. /dev/ttyUSB0 or /dev/ttyS0)
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)

# Wait for ESP32 to be ready
ser.flush()
print("Waiting 2s for ESP32...")
ser.write(b'\n')  # wake ESP32 read loop
ser.flush()
time_str = datetime.utcnow().strftime("TIME:%Y-%m-%dT%H:%M:%SZ")
print("Sending:", time_str)

ser.write((time_str + "\n").encode())
ser.flush()

