int sensorPin = 2; //
float rawRange = 4096;
float logRange = 5.0;
 
void setup()
{
Serial.begin(9600);
}
 
void loop()
{
// read the raw value from the sensor:
int rawValue = analogRead(sensorPin);
 
Serial.print("Raw = ");
Serial.print(rawValue);
Serial.print(" - Lux = ");
Serial.println(RawToLux(rawValue));
delay(1000);
}
 
float RawToLux(int raw)
{
float logLux = raw * logRange / rawRange;
return pow(10, logLux);
}
