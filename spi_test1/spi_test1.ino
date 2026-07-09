#include <Adafruit_NeoPixel.h>

#define RGB_PIN 27
#define NUMPIXELS 1

Adafruit_NeoPixel rgb(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

// Yazılımsal SPI pinleri
#define SCK_PIN   6
#define MOSI_PIN  7
#define CS_PIN    10

// Gönderilecek veri
const byte data = 0b10001001;

// Her clock yarım periyodu (ms)
// 100 ms = 5 Hz clock
const int HALF_PERIOD = 100;

//----------------------------------------

void setColor(uint8_t r, uint8_t g, uint8_t b)
{
  rgb.setPixelColor(0, rgb.Color(r, g, b));
  rgb.show();
}

//----------------------------------------

void sendByte(byte value)
{
  Serial.print("Gonderilen: ");

  digitalWrite(CS_PIN, LOW);

  // MSB -> LSB
  for (int i = 7; i >= 0; i--)
  {
    bool bit = bitRead(value, i);

    // MOSI'yi clock yükselmeden önce ayarla
    digitalWrite(MOSI_PIN, bit);

    // LED
    if (bit)
      setColor(0, 255, 0);   // Yeşil
    else
      setColor(255, 0, 0);   // Kırmızı

    Serial.print(bit);

    // Clock LOW
    digitalWrite(SCK_PIN, LOW);
    delay(HALF_PERIOD);

    // Clock HIGH
    digitalWrite(SCK_PIN, HIGH);
    delay(HALF_PERIOD);
  }

  digitalWrite(CS_PIN, HIGH);

  Serial.println();
  Serial.println("-----------------------");

  setColor(0, 0, 255);   // Gönderim bitti = Mavi

  delay(1000);
}

//----------------------------------------

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);

  digitalWrite(SCK_PIN, LOW);
  digitalWrite(MOSI_PIN, LOW);
  digitalWrite(CS_PIN, HIGH);

  rgb.begin();
  rgb.clear();
  rgb.show();

  Serial.println("===== SPI EGITIM TESTI =====");
}

//----------------------------------------

void loop()
{
  sendByte(data);
}