import MySQLdb, smbus, time

DEVICE_ADDRESS = 0x0F
DEVICE_REG = 0x1d


#Obtem o primeiro registro do banco de dados
def get_db_data():
    global db
    db = MySQLdb.connect("localhost", "root", "root", "transorg")
    global cursor
    cursor = db.cursor() 
    cursor.execute("SELECT * FROM transorg")
    data = cursor.fetchone()
    if not data:
        print ("Empty database. Waiting for data...")
        time.sleep(3)
        db.close()
        return get_db_data()
    else:
        return data

#Post to ARDUINO
def arduino_post():
    data = get_db_data()
    
    payload = "{\n\t\"date\":\""+ str(data[0]) +"\", \n\t\"latitude\":\""+ str(data[1]) +"\", \n\t\"longitude\":\"" + str(data[2]) + "\", \n\t\"temperature\":\"" + str(data[3]) + "\", \n\t\"is_locked\":" + str(data[4]) + ", \n\t\"transport_id\":" + str(data[6]) + ", \n\t\"enable\":"+ str(data[5]) +"\n}"

    
    bus = smbus.SMBus(1)    # 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)

    requisition_data = payload.split(" ",7)

    x = 0
    while (x < 7):
        data = changeToBytes(requisition_data[x])
        bus.write_i2c_block_data(DEVICE_ADDRESS, DEVICE_REG, data)
        time.sleep(1)
        print data
        x = x+1

def changeToBytes(string):
    data_array = []
    for char in string:
        data_array.append(ord(char[0]))
    return data_array

#Delete data
def delete_data():
    data = get_db_data()
    sql = 'DELETE FROM transorg WHERE date = ' + "'" + str(data[0]) + "'"
    try:
        cursor.execute(sql)
        db.commit()
        print("DELETED " + str(data[0]) + " from database")
    except:
        db.rollback()


while True:
    get_db_data()
    arduino_post()
    time.sleep(300)
    delete_data()
    db.close()