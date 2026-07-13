#define GM65_RX 4
#define GM65_TX 5

HardwareSerial GM65(1);

void setup() {
  Serial.begin(115200);

  GM65.begin(115200, SERIAL_8N1, GM65_RX, GM65_TX);

  Serial.println("GM65 Hazir");
  Serial.println("Space veya Enter ile tara");
}

void loop() {

  // Klavyeden tetikleme
  if (Serial.available()) {
    char c = Serial.read();

    if (c == ' ' || c == '\n' || c == '\r') {

      Serial.println("Tarama basladi");

      // GM65 tetikleme komutu
      byte trigger[] = {0x7E,0x00,0x08,0x01,0x00,0x02,0x01,0xAB,0xCD};

      GM65.write(trigger, sizeof(trigger));
    }
  }


  // GM65 verisi
  while (GM65.available()) {
    char c = GM65.read();
    Serial.write(c);
  }
}