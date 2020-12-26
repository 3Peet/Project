#define relay 26

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(relay,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(relay,LOW);
  Serial.println("Current Flowing");
  delay(5000);

  digitalWrite(relay,HIGH);
  Serial.println("Current not Flowing");
  delay(5000);

}
