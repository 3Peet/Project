#include "DHT.h"
#include "WiFi.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Define Pin
#define DHTPIN 15
#define LDR 2
#define DHTTYPE DHT11 // Sensor DHT11

DHT dht(DHTPIN, DHTTYPE); // lib of DHT

//Networks Setup
const char* ssid = "RP4";
const char* pwd = "0852902000s";
String serverName = "http://192.168.1.103:3000/update-sensor/add";
int counter = 1;

//ใช้สำหรับ Multitask Func 
//ใช้ unsigned long เพราะว่าเก็บค่าได้มากกว่า int
unsigned long lastTime=0;
unsigned long timerDelay=5000;  //Set timer to 5 seconds (5000ms)


void setup() {
  
  Serial.begin(115200);
  WiFi.begin(ssid,pwd);

  while(WiFi.status() != WL_CONNECTED){
     delay(500);
     Serial.println("Connecting to WiFi...");
    }
    
   Serial.println("Connected to the WiFi !");
   Serial.print("IPv4 : ");
   Serial.println(WiFi.localIP());
   
  dht.begin();


}

void loop() {
  delay(2000);
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  String str1 = "";
  str1 += "{";
  str1 +=  "\"id\":";
  str1 += String(counter);
  str1 += ",";
  str1 += "\"temp\":";
  str1 += String(t);
  str1 += ",";
  str1 += "\"hum\":";
  str1 += String(h);
  str1 += ",";
  str1 += "\"light\":";
  str1 += String(random(20, 90));
  str1 += "}";

  //Serial.print(F("Humidity: "));
  //Serial.print(h);
  //Serial.print(F("%  Temperature: "));
  //Serial.print(t);
  //Serial.println(F("°C "));
  Serial.println(str1);

  
    }
