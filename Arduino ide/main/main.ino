//Set timer
#define INTERVAL_DHT 1500 // 1.5sec
#define INTERVAL_LDR 1000 // 1 sec
#define INTERVAL_Relay_light 1000

#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>

//WiFi Setup
const char* ssid = "RP4";
const char* password = "0852902000s";
const char* serverName = "http://192.168.1.112:3000/";
const char* serverName_light = "http://192.168.1.112:3000/sw-light";

// Define Pin
#define DHTPIN 15
#define LDRPIN 34
#define DHTTYPE DHT11 // Sensor DHT11
#define relay_light 26      // Light Relay
DHT dht(DHTPIN, DHTTYPE); // lib of DHT

//FOR LDR sensor
float rawRange = 4096;
float logRange = 5.0;
 
unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long time_3 = 0;
unsigned long time_4 = 0;

//FOR Light Relay Setup
String sensorReadings;
String status_light;
 
void print_time(unsigned long time_millis);
 
void setup() {
    pinMode(relay_light,OUTPUT);
    dht.begin();
    Serial.begin(115200);

    //WiFi Setup
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}
 
void loop() {
  
    // DHT on timer #1
    if(millis() - time_1 > INTERVAL_DHT){

        time_1 = millis();
      
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        float f = dht.readTemperature(true); //(isFahrenheit = true)
        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t) || isnan(f)) {
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
        }
        Serial.print("Tempurature: ");
        Serial.print(t);
        Serial.print("  Humidity: ");
        Serial.println(h);
        
        //Create JSON Payload
        String payload_dht = "";
        payload_dht += "{";
        payload_dht += "\"id\":";
        payload_dht += "\"dht\"";
        payload_dht += ",";
        payload_dht += "\"temp\":";
        payload_dht += String(t);
        payload_dht += ",";
        payload_dht += "\"hum\":"; 
        payload_dht += String(h);
        payload_dht += "}";

        post_json(payload_dht);
 
    }
    
   // LDR on timer #2
    
    if(millis() - time_2 > INTERVAL_LDR){
        
        time_2 = millis();

        int rawValue = analogRead(LDRPIN);
        
        Serial.print("Raw = ");
        Serial.print(rawValue);
        Serial.print(" - Lux = ");
        Serial.println(RawToLux(rawValue));

        //Create JSON Payload
        String payload_ldr = "";
        payload_ldr += "{";
        payload_ldr += "\"id\":";
        payload_ldr += "\"ldr\"";
        payload_ldr += ",";
        payload_ldr += "\"light\":";
        payload_ldr += String(RawToLux(rawValue));
        payload_ldr += "}"; 

        post_json(payload_ldr);
    }

    if(millis()- time_3 > INTERVAL_Relay_light) {
      time_3 = millis();
      sensorReadings = httpGETRequest(serverName_light);
      Serial.println(sensorReadings);
      status_light = sensorReadings.substring(179,183);
      Serial.println(status_light);
      if(status_light=="true") {
        Serial.println("LIGHT ON");
        digitalWrite(relay_light,LOW);
      }
      else {
        Serial.println("LIGHT OFF");
        digitalWrite(relay_light,HIGH);
      }
  }
}
 

float RawToLux(int raw) {
    float logLux = raw * logRange / rawRange;
    return pow(10, logLux);
}

void post_json(String payload){
//Send POST Request to Web Server
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Content-Type", "application/json");      //JSON Type
    int httpResponseCode = http.POST(payload);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.println(payload);
    Serial.println("-----------------------------------");
    http.end();
    }
  else {
    Serial.println("WiFi Disconnected");
    }

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
