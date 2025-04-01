#define RX_PIN 0
#define TX_PIN 1 

void setup() {
  Serial.begin(9600);
}

void loop() {
  String data;
  if (Serial.available())
  {
      data = (char) Serial.read();
      Serial.print(data);
  }
}
