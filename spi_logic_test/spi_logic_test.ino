#include <SPI.h>

//==============================
// BUTONLAR
//==============================
const int butonArtir   = 2;
const int butonAzalt   = 3;
const int butonOnayla  = 0;

//==============================
// LEDLER
//==============================
const int ledPinleri[4] = {4, 5, 8, 14};

//==============================
// SPI PINLERİ
//==============================
const int SPI_SCK  = 6;
const int SPI_MOSI = 7;
const int SPI_CS   = 10;

//==============================
int hafizadakiSayi = 0;
bool spiAktif = false;

//==============================
void ledleriYak(byte sayi)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(ledPinleri[i], bitRead(sayi, i));
  }
}

//==============================
void ledleriSondur()
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(ledPinleri[i], LOW);
  }
}

//==============================
void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(butonArtir, INPUT_PULLUP);
  pinMode(butonAzalt, INPUT_PULLUP);
  pinMode(butonOnayla, INPUT_PULLUP);

  for (int i = 0; i < 4; i++)
  {
    pinMode(ledPinleri[i], OUTPUT);
  }

  ledleriSondur();

  pinMode(SPI_CS, OUTPUT);
  digitalWrite(SPI_CS, HIGH);

  SPI.begin(SPI_SCK, -1, SPI_MOSI, SPI_CS);

  Serial.println("=================================");
  Serial.println("SPI EGITIM PROJESI");
  Serial.println("GPIO2 = Artir");
  Serial.println("GPIO3 = Azalt");
  Serial.println("GPIO0 = SPI Baslat / Durdur");
  Serial.println("=================================");
}

//==============================
void loop()
{

  //-----------------------------
  // Artır
  //-----------------------------
  if (digitalRead(butonArtir) == LOW)
  {
    hafizadakiSayi++;

    if (hafizadakiSayi > 15)
      hafizadakiSayi = 0;

    Serial.print("Secilen Deger : ");
    Serial.print(hafizadakiSayi);
    Serial.print(" (");

    for (int i = 3; i >= 0; i--)
      Serial.print(bitRead(hafizadakiSayi, i));

    Serial.println(")");

    delay(250);
  }

  //-----------------------------
  // Azalt
  //-----------------------------
  if (digitalRead(butonAzalt) == LOW)
  {
    hafizadakiSayi--;

    if (hafizadakiSayi < 0)
      hafizadakiSayi = 15;

    Serial.print("Secilen Deger : ");
    Serial.print(hafizadakiSayi);
    Serial.print(" (");

    for (int i = 3; i >= 0; i--)
      Serial.print(bitRead(hafizadakiSayi, i));

    Serial.println(")");

    delay(250);
  }

  //-----------------------------
  // SPI Aç / Kapat
  //-----------------------------
  if (digitalRead(butonOnayla) == LOW)
  {
    delay(50);

    if (digitalRead(butonOnayla) == LOW)
    {
      spiAktif = !spiAktif;

      if (spiAktif)
      {
        Serial.println();
        Serial.println("===== SPI AKTIF =====");

        Serial.print("Gonderilen Veri : ");
        Serial.print(hafizadakiSayi);
        Serial.print(" (");

        for (int i = 3; i >= 0; i--)
          Serial.print(bitRead(hafizadakiSayi, i));

        Serial.println(")");

        ledleriYak(hafizadakiSayi);
      }
      else
      {
        Serial.println();
        Serial.println("===== SPI DURDURULDU =====");

        ledleriSondur();

        digitalWrite(SPI_CS, HIGH);
      }

      while (digitalRead(butonOnayla) == LOW);
      delay(150);
    }
  }

  //-----------------------------
  // SPI Sürekli Çalışıyor
  //-----------------------------
  if (spiAktif)
  {
    byte veri = hafizadakiSayi;

    // Eğer artır/azalt ile sayı değişirse
    // LED'ler de anında güncellensin
    ledleriYak(veri);

    digitalWrite(SPI_CS, LOW);

    SPI.beginTransaction(
      SPISettings(
        10000,
        MSBFIRST,
        SPI_MODE0));

    SPI.transfer(veri);

    SPI.endTransaction();

    digitalWrite(SPI_CS, HIGH);

    delay(2);
  }

}