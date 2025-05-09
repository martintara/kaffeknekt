#!/usr/bin/env python3
"""
RTC Time Setting Script for ESP32
This script allows setting the time on an ESP32 with DS3232 RTC module via serial connection.
"""

import serial
import serial.tools.list_ports
import datetime
import time
import sys

def list_serial_ports():
    """List all available serial ports"""
    ports = serial.tools.list_ports.comports()
    
    if not ports:
        print("No serial ports found")
        return []
    
    print("Available serial ports:")
    for i, port in enumerate(ports):
        print(f"{i+1}. {port.device} - {port.description}")
    
    return [port.device for port in ports]

def select_serial_port(ports):
    """Let the user select a serial port"""
    if not ports:
        return None
    
    while True:
        try:
            selection = input(f"Select a port (1-{len(ports)}), or enter the full port name: ")
            
            # Check if it's a direct port name
            if selection in ports:
                return selection
            
            # Check if it's a selection number
            selection_num = int(selection)
            if 1 <= selection_num <= len(ports):
                return ports[selection_num-1]
            else:
                print(f"Please enter a number between 1 and {len(ports)}")
        except ValueError:
            print("Invalid input. Please enter a number or port name.")

def get_date_time_input():
    """Get date and time input from user"""
    print("\nEnter date and time (leave blank to use current system time):")
    
    try:
        date_input = input("Date (YYYY-MM-DD) or press Enter for today: ")
        time_input = input("Time (HH:MM:SS) or press Enter for now: ")
        
        # Use current date/time if not provided
        now = datetime.datetime.now()
        
        if date_input.strip():
            year, month, day = map(int, date_input.split('-'))
        else:
            year, month, day = now.year, now.month, now.day
            
        if time_input.strip():
            hour, minute, second = map(int, time_input.split(':'))
        else:
            hour, minute, second = now.hour, now.minute, now.second
            
        # Validate input
        if not (2000 <= year <= 2099):
            print("Year must be between 2000 and 2099")
            return None
        if not (1 <= month <= 12):
            print("Month must be between 1 and 12")
            return None
        if not (1 <= day <= 31):
            print("Day must be between 1 and 31")
            return None
        if not (0 <= hour <= 23):
            print("Hour must be between 0 and 23")
            return None
        if not (0 <= minute <= 59):
            print("Minute must be between 0 and 59")
            return None
        if not (0 <= second <= 59):
            print("Second must be between 0 and 59")
            return None
        
        return (year, month, day, hour, minute, second)
    
    except ValueError as e:
        print(f"Invalid input format: {e}")
        return None

def send_time_to_esp32(port, datetime_tuple):
    """Send the time command to ESP32"""
    year, month, day, hour, minute, second = datetime_tuple
    
    # Format the command
    command = f"SET:{year},{month},{day},{hour},{minute},{second}\n"
    
    try:
        # Open serial port
        with serial.Serial(port, 115200, timeout=2) as ser:
            print(f"\nConnected to {port} at 115200 baud")
            print("Waiting for ESP32 to be ready...")
            
            # Allow some time for the ESP32 to reset/initialize
            time.sleep(2)
            
            # Flush any pending input
            ser.reset_input_buffer()
            
            # Send the command
            print(f"Sending command: {command.strip()}")
            ser.write(command.encode())
            
            # Read and print the response
            print("\nResponse from ESP32:")
            timeout_start = time.time()
            while time.time() < timeout_start + 5:  # 5-second timeout
                if ser.in_waiting:
                    line = ser.readline().decode('utf-8').strip()
                    print(f"> {line}")
                    if "Time set successfully" in line:
                        print("\nTime set successfully!")
                        return True
                time.sleep(0.1)
            
            print("No confirmation received. Time may not have been set.")
            return False
            
    except serial.SerialException as e:
        print(f"Serial error: {e}")
        return False

def main():
    """Main function"""
    print("ESP32 RTC Time Setting Utility")
    print("==============================")
    
    # List available ports
    ports = list_serial_ports()
    if not ports:
        sys.exit(1)
    
    # Let user select a port
    selected_port = select_serial_port(ports)
    if not selected_port:
        print("No port selected. Exiting.")
        sys.exit(1)
    
    # Get date and time from user
    datetime_tuple = get_date_time_input()
    if not datetime_tuple:
        print("Invalid date/time input. Exiting.")
        sys.exit(1)
    
    # Confirm before sending
    year, month, day, hour, minute, second = datetime_tuple
    print(f"\nAbout to set time to: {year}-{month:02d}-{day:02d} {hour:02d}:{minute:02d}:{second:02d}")
    confirm = input("Proceed? (y/n): ")
    
    if confirm.lower() != 'y':
        print("Operation cancelled.")
        sys.exit(0)
    
    # Send time to ESP32
    success = send_time_to_esp32(selected_port, datetime_tuple)
    
    if success:
        print("\nOperation completed successfully.")
    else:
        print("\nOperation may not have completed successfully.")
        print("Check the ESP32 serial output for more information.")

if __name__ == "__main__":
    main()
