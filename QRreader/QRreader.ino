#define RX_PIN 4
#define TX_PIN 5

void setup() {
  Serial.begin(115200);

  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  Serial.println("GM65 test basladi");
}

void loop() {

  while (Serial1.available()) {
    char c = Serial1.read();
    Serial.print(c);
  }

}