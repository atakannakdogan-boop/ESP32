#include <mbedtls/base64.h>

#define GM65_RX 4
#define GM65_TX 5

HardwareSerial GM65(1);

String qrData = "";
unsigned long lastByteTime = 0;
bool receiving = false;

void setup() {
  Serial.begin(115200);
  delay(2000);

  GM65.begin(115200, SERIAL_8N1, GM65_RX, GM65_TX);

  Serial.println("GM65 Hazir");
  Serial.println("Space veya Enter ile tara");
}

void loop() {

  // Space veya Enter ile GM65 tetikle
  if (Serial.available()) {
    char c = Serial.read();

    if (c == ' ' || c == '\n' || c == '\r') {

      qrData = "";
      receiving = false;

      Serial.println("\nTarama basladi");

      byte trigger[] = {
        0x7E, 0x00, 0x08, 0x01, 0x00,
        0x02, 0x01, 0xAB, 0xCD
      };

      GM65.write(trigger, sizeof(trigger));
    }
  }

  // GM65'ten gelen veriyi oku
  while (GM65.available()) {

    char c = GM65.read();

    // Yazdırılabilir karakterleri al
    if (isPrintable(c)) {
      qrData += c;
      receiving = true;
      lastByteTime = millis();
    }
  }

  // Veri gelmesi bittiyse (100 ms yeni byte gelmezse)
  if (receiving && millis() - lastByteTime > 100) {

    receiving = false;

    Serial.println("\n===== QR OKUNDU =====");

    Serial.print("Base64 Veri : ");
    Serial.println(qrData);

    // Base64 Decode
    unsigned char decoded[512];
    size_t decodedLen = 0;

    int ret = mbedtls_base64_decode(
      decoded,
      sizeof(decoded) - 1,
      &decodedLen,

        
   (const unsigned char*)qrData.c_str(),
qrData.length()
);
/*
      (const unsigned char*)"MTUxMjIwMjIyMTA4IEF0YWthbiBBa2RvZ2FuIA==",
strlen("MTUxMjIwMjIyMTA4IEF0YWthbiBBa2RvZ2FuIA==")
    );
*/
    if (ret == 0) {

      decoded[decodedLen] = '\0';
/*
      Serial.print("Decoded Veri: ");
      Serial.println((char*)decoded);
*/
      Serial.print("Decoded Veri: ");
      Serial.println((char*)decoded);

    } else {

      Serial.print("Decode Hatasi! Kod = ");
      Serial.println(ret);
    }

    Serial.println("======================");

    qrData = "";
  }
}