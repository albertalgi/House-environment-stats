import mysql.connector
from datetime import datetime

@staticmethod
def connect():
    '''Connects to database'''
    mydb = mysql.connector.connect(
      host="localhost",
      user="user",
      password="password",
      database="meteo"
    )

    return mydb.cursor(), mydb

@staticmethod
def get_last_temp_humidity(sensor_id: str):

    mycursor, mydb = connect()

    sql = "Select temperature, humidity from meteo_data where sensor = '{}' and date_meteo = (Select max(date_meteo) from meteo_data where sensor = '{}')".format(sensor_id, sensor_id)
    
    mycursor.execute(sql)

    result = mycursor.fetchall()

    print(result)

    return result

@staticmethod
def insert_data(humidity, temperature, sensor):
    
    timestamp = datetime.now()

    mycursor, mydb = connect()

    sql = "INSERT INTO meteo_data (temperature, humidity, date_meteo, sensor) VALUES (%s, %s, %s, %s)"

    val = (temperature, humidity, timestamp, sensor)
    mycursor.execute(sql, val)

    mydb.commit()

    print(mycursor.rowcount, "record inserted.")
    
    return