import MySQLdb, requests, time
from requests.exceptions import ConnectionError



db = MySQLdb.connect("localhost", "root", "root", "teste")

cursor = db.cursor()

cursor.execute("SELECT * FROM teste")

data = cursor.fetchone()
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



sql = 'DELETE FROM teste WHERE id = ' + str(data[0])

try:
    cursor.execute(sql)
    db.commit()
except:
    db.rollback()


db.close()


