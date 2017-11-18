#Isto e um comentario
import serial
import time

phone = serial.Serial("/dev/ttyAMA0", 9600, timeout=1)


#print(" before action")
    #sim800_responde()
def sim800_responde():
    while True:
        response = phone.readline()    
        print  response
        if "OK" in response:
            break


def web(): 

    phone.write('AT+CREG?\r')
    sim800_responde()

    phone.write('AT+SAPBR=3,1,"Contype","GPRS"\r')
    sim800_responde()

    phone.write('AT+SAPBR=3,1,"APN","tim.br"\r')
    sim800_responde()

    phone.write('AT+SAPBR=1,1\r')
    sim800_responde()


    #phone.write('AT+SAPBR=2,1\r')
    #sim800_responde()


    phone.write('AT+HTTPINIT\r')
    sim800_responde()

    phone.write('AT+HTTPPARA="CID",1\r')
    sim800_responde()

    phone.write('AT+HTTPPARA="URL","http://transports-rest-api.herokuapp.com/report/50"\r')
    sim800_responde()

    phone.write('AT+HTTPPARA="CONTENT","application/json"\r')
    #sim800_responde()

    payload='{"date":"05/34/2050","latitude":324.45,"longitude":4123.678,"temperature":34,"is_locked":1,"transport_id":50,"enable":1}'
    payload_len=len(payload)


    phone.write('AT+HTTPDATA= '+str(payload_len)+',10000\r')

    phone.write(payload)
    sim800_responde() 
    #phone.write('AT+HTTPSSL=0\r')
    #payload='{\"date":"13424",\"latitude":111111,\"longitude":11111,\"temperature":2,\"is_locked":1,\"transport_id":1\}'
    #sim800_responde()
    #print("before sleep")
    #time.sleep(1)
    #print(" after sleep")
    phone.write('AT+HTTPACTION=1\r')
    time.sleep(3)
    sim800_responde()
    phone.write('AT+HTTPREAD\r')
    sim800_responde()

    phone.write('AT+HTTPTERM\r')
    sim800_responde()

    phone.write('AT+SAPBR=0,1\r')   
    sim800_responde()
  

web()
