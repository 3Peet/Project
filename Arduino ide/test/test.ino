#define P0 15
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  delay(1000);
  Serial.println(analogRead(P0));
  // put your main code here, to run repeatedly:

}
