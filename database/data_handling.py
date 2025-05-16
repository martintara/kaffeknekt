import serial
import json
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime
import socket
import os

"""ESP32 to InfluxDB and Socket Connection data bridge

This script reads JSON-formatted sensor data from an ESP32 over a serial
connection, parses and formats the data, writes it to InfluxDB, and transmits
it to a local client over a UNIX domain socket.

Requirements:
    - InfluxDB client library
    - A running InfluxDB instance with a configured bucket
    - ESP32 sending JSON-encoded data via serial
"""

# InfluxDB settings
URL = 'http://localhost:8086'
ORG = 'Kaffeknekt'
TOKEN = 'ETV_6VBhkfF7HzNGfOjN6F7nTvX0ye_tblcGObcB1OVJDLYxQXUWpt8NU84PJmrn6R6IV921X2eWLLJDg1wgdQ=='
BUCKET = 'sensor_data'

#Registers "address information" to connect to InfluxDB
client = InfluxDBClient(url=URL, token=TOKEN, org=ORG)

#Grants Write ability to the API
API = client.write_api(write_options=SYNCHRONOUS)

#Predetermined connection settings for Esp32
serial_port = '/dev/ttyUSB0'
baud_rate = 115200

def main(): 

    """Main loop to read, transform, and transfer sensor data from Esp32.
    
    Opens a serial connection to the Esp32 and a UNIX socket
    for a connection to a client. Continuously reads JSON-formatted data
    from the Esp32, converts timestamps, writes data to InfluxDB, and
    forwards the data to a connected client.
    
    Steps:
        - Sets up a UNIX socket with filepath and waits for client
        - Reads serial input line-by-line
        - Converts JSON strings into Python dictionaries
        - Extracts and formats timestamps
        - Structures points with serial data and writes to InfluxDB
        - Sends the same data through the socket connection to a connected client

    Raises:
        KeyboardInterrupt: If execution is manually stopped
        JSONDecodeError: If serial input cannot be parsed, i.e because of bad format
        KeyError: If certain fields are missing from data
        SerialException: If there's an error with the serial port, i.e no connection
    """
    
    #Define socket path
    path = '/tmp/socket'

    #unlinking 'path' to potentially existing filepath
    if os.path.exists(path):
        os.unlink(path)

    #Defining 'server'
    server = socket.socket(
        socket.AF_UNIX, #Declares a local socket
        socket.SOCK_STREAM #Ensures secure connection-based communication between server and client
    )

    #Assigns path to the socket
    server.bind(path)

    #Sets the server to listen mode for 1 client to connect
    server.listen(1)
    
    try:
        #Opens port to esp32 temporarily
        with serial.Serial(serial_port, baud_rate, timeout=1) as ser:
            print("Esp connected")
            
            print("Waiting for client connection")

            #Waits for client to connect
            connection, cli_addr = server.accept()
            print("Connected:", cli_addr)
            
            while True:
                #reads line, converts byte to string, and cleans excess characters like \n
                line = ser.readline().decode('utf-8').strip() 
                
                if line: #checks if line exists
                    try:
                        #Convert json string to python dictionary
                        data = json.loads(line)
                        print(data)

                        #Turns nanoseconds into seconds and removes microseconds
                        stamp = datetime.fromtimestamp(int(data["timestamp"]) / 1e9).replace(microsecond=0)
                        #convert to ISO format
                        iso = stamp.isoformat()
                        #remove T
                        date_part, time_part = iso.split("T")
                        #save as string
                        readable_time = f"{date_part} {time_part}"

                        #Initializing Point structurization
                        point = Point("Esp32Metrics") \
                        .field("readable_time", readable_time)

                       
                        #Goes through and adds every item in the dictionary to the Point with defined value types
                        for field, value in data.items():
                            if field != "timestamp" and field != "flag":
                                point = point.field(field, float(value))
                                
                            elif field == "flag":
                                point = point.field(field, str(value))

                            elif field == "timestamp":
                                point = point.time(int(value))

                        print(point)

                        #Tells the API to write the current Point to the bucket with nanoseconds presicion
                        API.write(bucket=BUCKET, org=ORG, record=point, write_precision='ns')
                        print("Data written to Influx")
                        
                        #Adding readable time to the json string
                        data["readable_time"] = readable_time

                        #Converting python dictionary back to json string and adding newline for the recieving code to detect
                        socketstring = json.dumps(data) + '\n'
                        
                        print(socketstring)

                        #Encodes the current JSON string and sends to the client
                        connection.send(socketstring.encode('utf-8'))

                    except json.JSONDecodeError:
                        print("Invalid JSON:", line)
                        
                    except KeyError as e:
                        print(f"{e} is missing.")
                        
    except KeyboardInterrupt:
        print("Stopped by user.")

    except serial.SerialException as e:
        print(f"Serial error: {e}")
        
    finally:

        #In the case of session termination, closes the connection in a secure manner
        connection.close()

        #Unlinks the current filepath from socket upon connection close
        os.unlink(path)

#Calls the main() function every time te script is being run
if __name__ == "__main__":
    main()
