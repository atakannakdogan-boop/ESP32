#include <WiFi.h>

const char* ssid = "MUSTAFA-ATA 1554";
const char* password = "12345678";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("WiFi baglaniyor...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi baglandi!");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Web sunucusu baslatildi.");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Yeni istemci baglandi.");

    while (client.connected() && !client.available()) {
      delay(1);
    }

    while (client.available()) {
      client.read(); // Gelen HTTP isteğini oku ve at
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<head>");
    client.println("<meta charset='UTF-8'>");
    client.println("<title>ESP32 Test</title>");
    client.println("</head>");
    client.println("<body style='font-family:Arial;'>");
    client.println("<h1>FATMINUR</h1>");
    client.println("<p>Tebrikler, WiFi bağlantısı başarılı.</p>");
    client.println("</body>");
    client.println("</html>");

    delay(1);
    client.stop();
    Serial.println("İstemci ayrildi.");
  }
}