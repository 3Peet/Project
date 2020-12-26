
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "RP4";
const char* password = "0852902000s";


//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.1.111:3000/sw-light";


String sensorReadings;
String status_light;


void setup() {
  
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  
}

void loop() {
  sensorReadings = httpGETRequest(serverName);
  Serial.println(sensorReadings.substring(182,186));
  status_light = sensorReadings.substring(182,186);
  Serial.println("------------------------------------------------------------------------");
  if(status_light=="true") Serial.println("ON");
  else Serial.println("OFF");
  delay(1000);
  
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
