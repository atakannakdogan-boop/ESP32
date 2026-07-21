#include <WiFi.h>
#include <time.h>

const char* ssid = "MUSTAFA-ATA 1554";
const char* password = "12345678";

bool wifiConnected = false;
bool timeSynced = false;

void printTime()
{
    struct tm timeinfo;

    if (getLocalTime(&timeinfo, 100))
    {
        Serial.printf("Saat : %02d/%02d/%04d %02d:%02d:%02d\n",
                      timeinfo.tm_mday,
                      timeinfo.tm_mon + 1,
                      timeinfo.tm_year + 1900,
                      timeinfo.tm_hour,
                      timeinfo.tm_min,
                      timeinfo.tm_sec);
    }
    else
    {
        Serial.println("Saat ayarlanmamis.");
    }
}

void connectWifiAndSyncTime()
{
    Serial.println("\nWiFi baglaniyor...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Baglandi.");
    Serial.print("IP : ");
    Serial.println(WiFi.localIP());

    Serial.println("NTP senkronizasyonu yapiliyor...");

    configTime(3 * 3600, 0, "pool.ntp.org", "time.google.com");

    struct tm timeinfo;

    while (!getLocalTime(&timeinfo, 1000))
    {
        Serial.print(".");
    }

    Serial.println("\nSaat guncellendi.");

    wifiConnected = true;
    timeSynced = true;
}

void disconnectWifi()
{
    Serial.println("\nWiFi kapatiliyor...");

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    wifiConnected = false;

    Serial.println("WiFi Kapali.");
    Serial.println("Saat internet olmadan devam ediyor.");
}

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("=================================");
    Serial.println("ESP32 RTC / NTP Testi");
    Serial.println("=================================");
    Serial.println("ENTER:");
    Serial.println("1. WiFi + NTP");
    Serial.println("2. WiFi Kapat");
    Serial.println("3. WiFi + NTP");
    Serial.println("===============================");
}

void loop()
{
    static unsigned long lastPrint = 0;

    if (timeSynced && millis() - lastPrint >= 1000)
    {
        lastPrint = millis();
        printTime();
    }

    if (Serial.available())
    {
        char c = Serial.read();

        if (c == '\n' || c == '\r')
        {
            if (!wifiConnected)
            {
                connectWifiAndSyncTime();
            }
            else
            {
                disconnectWifi();
            }
        }
    }
}