from flask import Flask, request
import db

app = Flask(__name__)

@app.route("/")
def main():
    
    # get_last_temp_humidity() --> [(14.0, 70.0)]
    result_1 = (14.0, 70.0)
    result_2 = (0.0, 0.0)
    try:
        result_1 = db.get_last_temp_humidity(1)[0]
        result_2 = db.get_last_temp_humidity(2)[0]
    except Exception as e:
        pass
    
    return '''<h1>METEO SERVER</h1>
    <h3>Indoor humidity:</h3>
    <p>{} %</p>
    <h3>Indoor temperature:</h3>
    <p>{} ºC</p>
    <h3>Exterior humidity:</h3>
    <p>{} %</p>
    <h3>Exterior temperature:</h3>
    <p>{} ºC</p>
    '''.format(result_1[1], result_1[0], result_2[1], result_2[0])

@app.route("/updateMeteo", methods=['GET'])
def update_meteo():
    '''
    Updates temperature(t) and humidity(h)
    '''
    args = request.args.to_dict()
    sensor = 1
    if "h" in args and "t" in args:
        main_temperature = request.args.get("t")
        main_humidity = request.args.get("h")
        if "s" in args: # Sensor id in request. If not specified it is set to 1 by the DB default
            sensor = request.args.get("s")
        db.insert_data(main_humidity,main_temperature, sensor)
    else:
        return "Bad request", 400
    return "Updated", 200