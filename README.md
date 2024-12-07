### IoT environment
This platform uses a Python REST API to receive data from Arduino sensors at home. This data is collected by the server and stored in a MySQL database. This data is processd and is use dto feed a Grafana portal to check temperatures and humidity units from every paired sensor.

![Domotic diagram](Domotic%20diagram.jpg)

All elements run in the local network. No online hosting is needed.

In ths repository there is also a telegram chatbot implementation to get the data when the user is not home (and hence access to localhost).

### Summary of elements
- Python REST API / Server: Receive and process inputs from sensors
- MySQL DB: Store data to feed Grafana
- Grafana: Print charts from stored data in MySQL DB
- esp8266 chip Arduino code to handle sensors
- Telegram bot: Get temperature and humidity of your home from wherever you are.
