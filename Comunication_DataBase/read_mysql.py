import MySQLdb, requests, time
from requests.exceptions import ConnectionError



while True:

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



    data = get_db_data()


    print data[0] 
    print data[1]



    url = "https://transports-rest-api.herokuapp.com/reports"
    headers = {'content-type':'application/json'}

    def api_post():
        try:
            response = requests.request("GET", url, headers=headers)
            print(response.status_code)
        except ConnectionError as e:
            print("tentando bostar...")
            time.sleep(5)
            return api_post()


    api_post()



    sql = 'DELETE FROM transorg WHERE date = ' + "'" + str(data[0]) + "'"
    print(sql)

    try:
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()


    db.close()
