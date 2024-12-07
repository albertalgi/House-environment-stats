### IoT environment
This platform uses a Python REST API to receive data from Arduino sensors at home. This data is collected by the server and stored in a MySQL database. This data is processd and is use dto feed a Grafana portal to check temperatures and humidity units from every paired sensor.

![Domotic diagram](Domotic%20diagram.jpg)

All elements run in the local network. No online hosting is needed.
