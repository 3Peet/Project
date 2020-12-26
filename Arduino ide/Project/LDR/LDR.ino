//int ledPin = D0;
//int ledPin3 = D1;
int analogPin = A0; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาที่5
int val = 0;
void setup() {
  //pinMode(ledPin, OUTPUT);  // sets the pin as output
  //pinMode(ledPin3, OUTPUT);  // sets the pin as output
  Serial.begin(9600);
}

void loop() {
  val = analogRead(analogPin);
  Serial.print("val = ");
  Serial.println(val); // พิมพ์ค่าของตัวแปร val
  //if (val < 900) { // ค่า 900 สามารถกำหนดปรับได้ตามค่าแสงในห้องต่างๆ
  //  digitalWrite(ledPin, LOW); // สั่งให้ LED ที่ Pin2 ดับ
  //  digitalWrite(ledPin3, HIGH); // สั่งให้ LED ที่ Pin3 ติดสว่าง
  //}
  //else {
  //  digitalWrite(ledPin, HIGH); // สั่งให้ LED ที่ Pin2 ติดสว่าง
  // digitalWrite(ledPin3, LOW); // สั่งให้ LED ที่ Pin3 ดับ
    
  //}
  delay(100);
}
