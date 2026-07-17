#include <WiFi.h>

const char* ssid = "MUSTAFA-ATA 1554";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Baglaniyor");

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  switch (WiFi.status()) {
    case WL_CONNECTED:
      Serial.println("Baglandi");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      break;

    case WL_NO_SSID_AVAIL:
      Serial.println("SSID bulunamadi.");
      break;

    case WL_CONNECT_FAILED:
      Serial.println("Sifre hatali.");
      break;

    case WL_DISCONNECTED:
      Serial.println("Baglanamadi.");
      break;

    default:
      Serial.print("Durum kodu: ");
      Serial.println(WiFi.status());
      break;
  }
}

void loop() {}