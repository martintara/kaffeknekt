import glob
import time

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


#Prints the temperature every 0.21 seconds
while True:
    celsius = temp_data()
    print(f'{celsius:.5f}°C')
    time.sleep(0.21)
