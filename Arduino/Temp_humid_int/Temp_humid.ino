/*
  HTTP libraries
*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//#include <Arduino_JSON.h>

/*
  Sensor libraries
*/
#include "DHT.h"

/*
  Set Wifi Settings
*/
const char* ssid = "SSID";
const char* password = "PASSWORD";
String serverPath = "http://IP:PORT/updateMeteo?";

#define DHTPIN 4     // esp8266 D2 pin map as 4 in Arduino IDE
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while(!Serial) { }

  /*
    Wifi connection
  */

  WiFi.begin(ssid, password);
  Serial.println("Connecting...");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

/*
  DHT11 Settings
*/
  dht.begin();

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running DHT!");
  Serial.println("-------------------------------------");

}

int timeSinceLastRead = 0;
void loop() {
  // Report every 15 minutes.
  if(timeSinceLastRead > 900000) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    //float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    //float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");
    
  /*
    HTTP
  */
  String jsonBuffer;
  
  //http://localhost:5000/updateMeteo?t=0&h=90
  String url = serverPath + "h=" + h + "&t=" + t;
  // Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    Serial.print("Sending data to --> ");
    Serial.print(url);
    jsonBuffer = httpGETRequest(url.c_str());
    Serial.println(jsonBuffer);
  }

    timeSinceLastRead = 0;
  }
  delay(100);
  timeSinceLastRead += 100;
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.println("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    Serial.print("%\t Server may be down");
  }
  // Free resources
  http.end();

  return payload;
}
