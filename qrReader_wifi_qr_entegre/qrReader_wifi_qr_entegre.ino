#include <mbedtls/base64.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#define GM65_RX 4
#define GM65_TX 5

HardwareSerial GM65(1);

String qrData = "";
bool receiving = false;
unsigned long lastByteTime = 0;

void setup() {

  Serial.begin(115200);
  delay(2000);

  GM65.begin(115200, SERIAL_8N1, GM65_RX, GM65_TX);

  Serial.println("================================");
  Serial.println("GM65 Hazir");
  Serial.println("Space veya Enter ile tara");
  Serial.println("================================");
}

void loop() {

  // Space veya Enter ile taramayı başlat
  if (Serial.available()) {

    char c = Serial.read();

    if (c == ' ' || c == '\n' || c == '\r') {

      qrData = "";
      receiving = false;

      Serial.println("\nTarama Basladi");

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

    if (isPrintable(c)) {

      qrData += c;
      receiving = true;
      lastByteTime = millis();
    }
  }

  // Veri gelmesi bittiyse
  if (receiving && millis() - lastByteTime > 100) {

    receiving = false;

    Serial.println("\n========== QR OKUNDU ==========");
    Serial.print("QR Verisi : ");
    Serial.println(qrData);
    Serial.println();

    // WiFi QR kontrolü
    if (qrData.startsWith("WIFI:")) {

      if (qrData.startsWith("WIFI:")) {

        Serial.println("QR Tipi : WiFi QR");
        Serial.println("Mod     : Konfigurasyon");

        // Bilgileri ayıkla
        int tStart = qrData.indexOf("T:") + 2;
        int tEnd = qrData.indexOf(";", tStart);

        int sStart = qrData.indexOf("S:") + 2;
        int sEnd = qrData.indexOf(";", sStart);

        int pStart = qrData.indexOf("P:") + 2;
        int pEnd = qrData.indexOf(";", pStart);

        String security = qrData.substring(tStart, tEnd);
        String ssid = qrData.substring(sStart, sEnd);
        String password = qrData.substring(pStart, pEnd);

        Serial.println();
        Serial.println("===== WiFi Bilgileri =====");
        Serial.print("Guvenlik : ");
        Serial.println(security);

        Serial.print("SSID      : ");
        Serial.println(ssid);

        Serial.print("Sifre     : ");
        Serial.println(password);

        Serial.println("==========================");

        // WiFi'ye bağlan
        Serial.println();
        Serial.println("WiFi baglantisi baslatiliyor...");

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());

        int timeout = 20;

        while (WiFi.status() != WL_CONNECTED && timeout > 0) {

          delay(500);
          Serial.print(".");

          timeout--;
        }

        if (WiFi.status() == WL_CONNECTED) {

          Serial.println();
          Serial.println("WiFi Baglandi!");

          Serial.print("IP Adresi : ");
          Serial.println(WiFi.localIP());

          // ===== Tarih / Saat =====
          configTime(3 * 3600, 0, "pool.ntp.org", "time.google.com");
          struct tm timeinfo;
          if (getLocalTime(&timeinfo, 10000)) {
            Serial.printf("Tarih : %02d/%02d/%04d\n", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
            Serial.printf("Saat  : %02d:%02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
          } else {
            Serial.println("Saat alinamadi.");
          }

          // ===== USD/TRY =====
          HTTPClient http;
          http.begin("https://open.er-api.com/v6/latest/USD");
          int code = http.GET();
          if (code == 200) {
            DynamicJsonDocument doc(8192);
            deserializeJson(doc, http.getString());
            Serial.print("1 USD = ");
            Serial.print((float)doc["rates"]["TRY"]);
            Serial.println(" TRY");
          } else {
            Serial.println("Kur bilgisi alinamadi.");
          }
          http.end();


        } else {

          Serial.println();
          Serial.println("WiFi Baglanamadi!");
        }
      }
    } else {

      Serial.println("QR Tipi : Base64 QR");
      Serial.println("Mod     : Decode");

      unsigned char decoded[512];
      size_t decodedLen = 0;

      int ret = mbedtls_base64_decode(
        decoded,
        sizeof(decoded) - 1,
        &decodedLen,
        (const unsigned char*)qrData.c_str(),
        qrData.length());

      if (ret == 0) {

        decoded[decodedLen] = '\0';

        Serial.print("Decoded : ");
        Serial.println((char*)decoded);

      } else {

        Serial.print("Decode Hatasi : ");
        Serial.println(ret);
      }
    }

    Serial.println("================================");

    qrData = "";
  }
}