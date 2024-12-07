/*
  HTTP libraries
*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

/*
  Sensor libraries
*/
#include <OneWire.h>
#include <DallasTemperature.h>

/*
  Set Wifi Settings
*/
const char* ssid = "<SSID>";
const char* password = "<PASSWD>";
String serverPath = "http://IP:PORT/updateMeteo?";

#define DALLASPIN 4     // esp8266 D2 pin map as 4 in Arduino IDE



//const int oneWirePin = 5;

OneWire oneWireBus(DALLASPIN);
DallasTemperature sensor(&oneWireBus);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  
  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the LED_BUILTIN pin as an output
  
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
}

String sensor_id = "2";
int timeSinceLastRead = 900000;
void loop() {
  // Report every 15 minutes.
  if(timeSinceLastRead > 900000) {
    
    Serial.println("Llegint temperature exterior... ");
    sensor.requestTemperatures();
    
    Serial.print("Temp: ");
    float temp = sensor.getTempCByIndex(0);
    Serial.print(temp);
    Serial.println(" ºC");
    
    
    
  /*
    HTTP
  */
  String jsonBuffer;
  
  //http://localhost:5000/updateMeteo?t=0&h=90
  String url = serverPath + "h=0&t=" + temp + "&s=" + sensor_id;
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
  
  if (httpResponseCode > 0) {
    Serial.println("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    Serial.print("%\t Server may be down");
  }
  // Free resources
  http.end();

  return payload;
}
