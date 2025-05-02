import serial
import json
import time

# === CONFIGURATION ===
SERIAL_PORT = "/dev/ttyUSB0"  # Adjust as needed
BAUD_RATE = 115200

# === MANUAL TIME TO SET ===
MANUAL_TIME = {
    "year": 2025,
    "month": 4,
    "day": 30,
    "hour": 15,
    "minute": 45,
    "second": 0
}

def main():
    print(f"Connecting to ESP32 on {SERIAL_PORT}...")

    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
            time.sleep(2)  # Give ESP32 time to reset
            print("Connected. Listening to ESP32...\n")

            # Send manual time update
            msg = {"set_time": True, **MANUAL_TIME}
            ser.write((json.dumps(msg) + "\n").encode())
            print("Sent time update:", msg)

            # Listen for a reply or incoming data
            timeout = time.time() + 5  # Wait up to 5 seconds for response
            while time.time() < timeout:
                if ser.in_waiting:
                    line = ser.readline().decode("utf-8", errors="ignore").strip()
                    if line:
                        try:
                            data = json.loads(line)
                            print("ESP32 →", data)
                        except json.JSONDecodeError:
                            print("Malformed line:", line)
                time.sleep(0.1)

    except serial.SerialException as e:
        print("Serial error:", e)

if __name__ == "__main__":
    main()

