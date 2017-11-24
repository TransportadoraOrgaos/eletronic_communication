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

#Printa os valores do ultimo registro no terminal
def print_data():
    data = get_db_data()
    print data[0] 
    print data[1]

#Post to API
def api_post():
    url = "https://transports-rest-api.herokuapp.com/reports"
    headers = {'content-type':'application/json'}
    try:
        response = requests.request("GET", url, headers=headers)
        print(response.status_code)
    except ConnectionError as e:
        print("tentando bostar...")
        time.sleep(5)
        return api_post()

#Delete data
def delete_data():
    data = get_db_data()
    sql = 'DELETE FROM transorg WHERE date = ' + "'" + str(data[0]) + "'"
    print(sql)
    try:
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()


while True:
    get_db_data()
    print_data()
    api_post()
    delete_data()
    db.close()