#Isto e um comentario
import serial
import time

phone = serial.Serial("/dev/ttyAMA0", 9600, timeout=1)

def sim800_responde():
    while True:
        response = phone.readline()    
        print  response
        if "OK" in response:
            break
     

def web(): 
    phone.write('AT+HTTPTERM\r')
    #sim800_responde()

    phone.write('AT+SAPBR=0,1\r')
    sim800_responde()
     
    phone.write('AT+HTTPTERM\r')
    sim800_responde()
  
web()
