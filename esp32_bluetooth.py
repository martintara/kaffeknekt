#!/usr/bin/env python3
import bluetooth
import time
import threading
import argparse
import json
import sys
import datetime

class ESP32BluetoothCommunication:
    def __init__(self, mac_address=None, device_name="ESP32_FireBeetle2"):
        """Initialize Bluetooth communication with ESP32"""
        self.mac_address = mac_address
        self.device_name = device_name
        self.socket = None
        self.is_connected = False
        self.is_running = False
        self.sensor_data = {}
        self.last_update_time = 0
        
    def discover_devices(self):
        """Discover nearby Bluetooth devices"""
        print("Scanning for Bluetooth devices...")
        nearby_devices = bluetooth.discover_devices(duration=8, lookup_names=True)
        print(f"Found {len(nearby_devices)} devices:")
        
        esp32_address = None
        
        for addr, name in nearby_devices:
            print(f"  {addr} - {name}")
            if name == self.device_name:
                esp32_address = addr
                print(f"Found ESP32 device: {name} ({addr})")
                
        return esp32_address
    
    def connect(self):
        """Connect to the ESP32 device"""
        if not self.mac_address:
            self.mac_address = self.discover_devices()
            
        if not self.mac_address:
            print(f"Could not find device with name {self.device_name}")
            return False
            
        try:
            # Create a new socket
            self.socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
            # Connect to ESP32 using port 1 (default for SPP)
            self.socket.connect((self.mac_address, 1))
            self.socket.settimeout(1.0)  # Set timeout for socket operations
            self.is_connected = True
            print(f"Connected to {self.device_name} ({self.mac_address})")
            return True
        except bluetooth.BluetoothError as e:
            print(f"Bluetooth connection error: {e}")
            self.socket = None
            return False
    
    def disconnect(self):
        """Disconnect from the ESP32"""
        if self.socket:
            try:
                self.socket.close()
                print("Disconnected from ESP32")
            except bluetooth.BluetoothError as e:
                print(f"Error disconnecting: {e}")
            finally:
                self.socket = None
                self.is_connected = False
    
    def send_command(self, command):
        """Send a command to the ESP32 device"""
        if not self.is_connected or not self.socket:
            print("Not connected to ESP32")
            return False
            
        try:
            # Add newline to the command
            full_command = command + '\n'
            self.socket.send(full_command)
            print(f"Sent: {command}")
            return True
        except bluetooth.BluetoothError as e:
            print(f"Error sending command: {e}")
            self.is_connected = False
            return False
    
    def process_json_data(self, json_string):
        """Process received JSON data"""
        try:
            data = json.loads(json_string)
            
            # Check if this is sensor data
            if "sensors" in data:
                self.sensor_data = data
                self.last_update_time = time.time()
                timestamp = data.get("timestamp", 0) / 1000.0  # Convert ms to seconds
                
                # Print formatted sensor data
                print("\n--- Sensor Data ---")
                print(f"Time: {datetime.datetime.now().strftime('%H:%M:%S')}")
                print(f"Device: {data.get('device', 'Unknown')}")
                
                # Print each sensor reading
                for sensor, value in data["sensors"].items():
                    unit = self.get_sensor_unit(sensor)
                    print(f"{sensor.capitalize()}: {value}{unit}")
                
                # Print system info if available
                if "system" in data:
                    print("\n--- System Info ---")
                    for key, value in data["system"].items():
                        if key == "uptime":
                            print(f"Uptime: {value} seconds")
                        elif key == "heap":
                            print(f"Free Heap: {value} bytes")
                        elif key == "led":
                            print(f"LED Status: {'ON' if value else 'OFF'}")
                        else:
                            print(f"{key.capitalize()}: {value}")
                
                print("------------------")
                return True
                
            # Check if this is a command response
            elif "type" in data and data["type"] == "response":
                print("\n--- Command Response ---")
                print(f"Command: {data.get('command', 'Unknown')}")
                print(f"Status: {data.get('status', 'Unknown')}")
                
                if "message" in data:
                    print(f"Message: {data['message']}")
                    
                if "commands" in data:
                    print(f"Available Commands: {data['commands']}")
                    
                print("------------------------")
                return True
            
            # Unknown JSON format
            else:
                print(f"\nReceived unknown JSON data: {json_string}")
                return False
                
        except json.JSONDecodeError:
            print(f"\nReceived invalid JSON: {json_string}")
            return False
            
    def get_sensor_unit(self, sensor_name):
        """Return the appropriate unit for a sensor type"""
        units = {
            "temperature": " °C",
            "humidity": " %",
            "pressure": " hPa",
            "light": " lux",
            "battery": " %",
            "motion": ""
        }
        return units.get(sensor_name.lower(), "")
            
    def start_monitoring(self):
        """Start monitoring for messages from ESP32"""
        if not self.is_connected:
            print("Not connected to ESP32")
            return False
            
        self.is_running = True
        self.monitor_thread = threading.Thread(target=self._monitor_task)
        self.monitor_thread.daemon = True
        self.monitor_thread.start()
        return True
    
    def stop_monitoring(self):
        """Stop the monitoring thread"""
        self.is_running = False
        if hasattr(self, 'monitor_thread'):
            self.monitor_thread.join(timeout=1)
    
    def _monitor_task(self):
        """Thread task for monitoring Bluetooth data"""
        print("Starting monitor thread...")
        buffer = ""
        
        while self.is_running and self.is_connected:
            try:
                self.socket.settimeout(0.5)
                data = self.socket.recv(1024).decode('utf-8')
                
                if data:
                    # Add to buffer and look for complete lines
                    buffer += data
                    lines = buffer.split('\n')
                    
                    # Process all complete lines
                    for i in range(len(lines) - 1):
                        line = lines[i].strip()
                        if line:  # Skip empty lines
                            # Check if it's a JSON string
                            if line.startswith('{') and line.endswith('}'):
                                self.process_json_data(line)
                            else:
                                print(f"\nReceived: {line}")
                    
                    # Keep the last incomplete line in the buffer
                    buffer = lines[-1]
                    
            except bluetooth.BluetoothError as e:
                if "timed out" not in str(e).lower():
                    print(f"\nError in monitor thread: {e}")
                    self.is_connected = False
                    break
            except Exception as e:
                print(f"\nUnexpected error in monitor thread: {e}")
            
            time.sleep(0.1)
        
        print("\nMonitor thread stopped")

    def get_latest_sensor_value(self, sensor_name):
        """Get the latest value for a specific sensor"""
        if not self.sensor_data or "sensors" not in self.sensor_data:
            return None
            
        sensors = self.sensor_data.get("sensors", {})
        return sensors.get(sensor_name, None)
        
    def display_available_commands(self):
        """Display available commands to the user"""
        print("\nAvailable Commands:")
        print("  led_on      - Turn on the ESP32's LED")
        print("  led_off     - Turn off the ESP32's LED")
        print("  get_sensors - Request immediate sensor data")
        print("  status      - Get system status")
        print("  help        - Show available commands")
        print("  quit        - Exit the program")

def main():
    """Main function for command-line interface"""
    parser = argparse.ArgumentParser(description='Communicate with ESP32 FireBeetle2 over Bluetooth')
    parser.add_argument('--mac', help='MAC address of the ESP32 (if known)')
    parser.add_argument('--name', default='ESP32_FireBeetle2', help='Bluetooth device name (default: ESP32_FireBeetle2)')
    args = parser.parse_args()

    esp = ESP32BluetoothCommunication(mac_address=args.mac, device_name=args.name)
    
    if not esp.connect():
        print("Failed to connect. Exiting.")
        return

    esp.start_monitoring()
    esp.display_available_commands()
    
    print("\nWaiting for sensor data and commands (press Ctrl+C to exit)...\n")
    
    try:
        while True:
            command = input("> ").strip().upper()  # Convert to upper case
            
            if command.lower() == 'quit':
                break
                
            if command:
                esp.send_command(command)
            
            # Wait briefly for a response
            time.sleep(0.5)
            
    except KeyboardInterrupt:
        print("\nProgram terminated by user")
    finally:
        esp.stop_monitoring()
        esp.disconnect()

if __name__ == "__main__":
    main()
