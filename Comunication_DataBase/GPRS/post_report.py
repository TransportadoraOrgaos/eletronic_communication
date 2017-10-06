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
    #phone.write('AT+CREG?')
    #sim800_responde()

    phone.write('AT+SAPBR=3,1,"Contype","GPRS"\r')
    sim800_responde()

    phone.write('AT+SAPBR=3,1,"APN","www"\r')
    sim800_responde()
     
    phone.write('AT+SAPBR=1,1\r')
    sim800_responde()
  

    phone.write('AT+SAPBR=2,1\r')
    sim800_responde()


    phone.write('AT+HTTPINIT\r')
    sim800_responde()

    phone.write('AT+HTTPPARA="CID",1\r')
    sim800_responde()

    phone.write('AT+HTTPPARA="URL","https://transports-rest-api.herokuapp.com/transport/CaixaRPI"\r')
    sim800_responde()

    phone.write('AT+HTTPPARA="CONTENT","application/json"\r')
    print("application json ready")

    payload='{"organ":"rin","responsible":"qualquercoisa"}'
    payload_len=len(payload)
    print(payload_len)
    print(str(payload_len))    
    print("payload ready")

    phone.write('AT+HTTPDATA= '+str(payload_len)+',8000\r')
    print("write payload,500")
    phone.write(payload)
    #phone.write('AT+HTTPSSL=1\r')
    #sim800_responde()
    #phone.write(payload)
    #payload='{\"date":"13424",\"latitude":111111,\"longitude":11111,\"temperature":2,\"is_locked":1,\"transport_id":1\}'
    #sim800_responde()
    print("before sleep")
    time.sleep(1)
    print(" after sleep")
    phone.write('AT+HTTPACTION=1\r')
    print(" before action")
    sim800_responde()
    #time.sleep(3)

    phone.write('AT+HTTPREAD\r')
    sim800_responde()

    phone.write('AT+HTTPTERM\r')
    sim800_responde()

    phone.write('AT+SAPBR=0,1\r')   
    sim800_responde()
  

web()
