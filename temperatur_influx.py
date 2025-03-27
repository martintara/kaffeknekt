import glob
import time
from influxdb_client import InfluxDBClient, Point

#Specify identification keys
URL = 'http://localhost:8086'
ORG = 'Kaffeknekt'
TOKEN = '' #token skal limes inn her
BUCKET = 'sensor_data'

#set up client between python script and influxdb
client = InfluxDBClient(url=URL, token=TOKEN, org=ORG)
API = client.write_api()


#Find the file containing the temperature data
path = glob.glob('/sys/bus/w1/devices/28*')[0] + '/w1_slave'

#Reads the contents of the file
def file_data():
    with open(path, 'r') as data:
        return data.readlines()


def temp_data():
    temp = file_data()
    
    #Checks and waits for the validity of the data
    while 'YES' not in temp[0]:
        time.sleep(0.2)
        temp = file_data()
        
    #Finds the temperature value and converts to celsius    
    t = temp[1].split('t=')
    c = float(t[1])/1000
    return c


#sends the temperature data every 3 seconds
while True:
    celsius = temp_data()
    
    #create datapoint to send
    p = Point('Temperature').field('temperature_C', celsius)
    
    API.write(bucket=BUCKET, org=ORG, record=p)
    print(f'{celsius:.5f}°C')
    
    time.sleep(3)
