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
    phone.write('AT+SAPBR=3,1,"APN","CMNET"\r')
    sim800_responde()
 
    phone.write('AT+SAPBR=1,1\r')
    sim800_responde()
  

    phone.write('AT+SAPBR=2,1\r')
    sim800_responde()


    phone.write('AT+HTTPINIT\r')
    sim800_responde()

    phone.write('AT+HTTPPARA="CID",1\r')
    sim800_responde()

    phone.write('AT+HTTPPARA="URL","https://transports-rest-api.herokuapp.com/transport/1"\r')
    sim800_responde()
  
    phone.write('AT+HTTPACTION=0\r')
    sim800_responde()
    time.sleep(7)

    phone.write('AT+HTTPREAD\r')
    sim800_responde()

    phone.write('AT+HTTPTERM\r')
    sim800_responde()

    phone.write('AT+SAPBR=0,1\r')   
    sim800_responde()
  

web()
