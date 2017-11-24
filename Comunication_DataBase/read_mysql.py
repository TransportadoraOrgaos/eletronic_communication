import MySQLdb, requests, time
from requests.exceptions import ConnectionError

#Obtem o primeiro registro do banco de dados
def get_db_data():
    global db
    db = MySQLdb.connect("localhost", "root", "root", "transorg")
    global cursor
    cursor = db.cursor() 
    cursor.execute("SELECT * FROM transorg")
    data = cursor.fetchone()
    if not data:
        print "Banco vazio"
        time.sleep(3)
        db.close()
        return get_db_data()
    else:
        return data

#Post to API
def api_post():
    data = get_db_data()
    url = "https://transports-rest-api.herokuapp.com/report/" + str(data[6])
    payload = "{\n\t\"date\": \""+ str(data[0]) +"\",\n\t\"latitude\": \""+ str(data[1]) +"\",\n\t\"longitude\": \"" + str(data[2]) + "\",\n\t\"temperature\": \"" + str(data[3]) + "\",\n\t\"is_locked\": " + str(data[4]) + ",\n\t\"transport_id\": " + str(data[6]) + ",\n\t\"enable\": "+ str(data[5]) +"\n}"
    headers = {'content-type':'application/json'}
    try:
        response = requests.request("POST", url, data=payload, headers=headers)
    except ConnectionError as e:
        print("tentando bostar...")
        time.sleep(5)
        return api_post()
    if response.status_code == 200 or response.status_code == 201:
        print("SUCCESSFULLY sent to API")
    else:
        print("ERROR to send to API: " + response.status_code)

#Delete data
def delete_data():
    data = get_db_data()
    sql = 'DELETE FROM transorg WHERE date = ' + "'" + str(data[0]) + "'"
    try:
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()


while True:
    get_db_data()
    api_post()
    delete_data()
    db.close()