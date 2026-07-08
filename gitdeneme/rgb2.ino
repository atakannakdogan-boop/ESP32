#include <Adafruit_NeoPixel.h>

#define RGB_PIN 27
#define NUMPIXELS 1

Adafruit_NeoPixel rgb(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  rgb.begin();
  //rgb.setBrightness(30);   
  
}

void loop() {

  /*
  // Kırmızı
  for(int i=0;i<=255;i++){

  rgb.setPixelColor(0, rgb.Color(i, 0, 0));
  rgb.show();
  delay(10);

  }
  for(int i=255;i>=0;i--){

  rgb.setPixelColor(0, rgb.Color(i, 0, 0));
  rgb.show();
  delay(10);

  }
   delay(10);
   */
  // Yeşil
    for(int i=0;i<=255;i++){

  rgb.setPixelColor(0, rgb.Color(0, i, 0));
  rgb.show();
  delay(10);

  }
  for(int i=255;i>=0;i--){

  rgb.setPixelColor(0, rgb.Color(0, i, 0));
  rgb.show();
  delay(10);

  }
  delay(10);

  // Mavi
  for(int i=0;i<=255;i++){

  rgb.setPixelColor(0, rgb.Color(0, 0, i));
  rgb.show();
  delay(10);

  }
  for(int i=255;i>=0;i--){

  rgb.setPixelColor(0, rgb.Color(0, 0, i));
  rgb.show();
  delay(10);

  }
  //mor
  for(int i=0;i<=255;i++){

  rgb.setPixelColor(0, rgb.Color(i, 0, i));
  rgb.show();
  delay(10);

  }
  for(int i=255;i>=0;i--){

  rgb.setPixelColor(0, rgb.Color(i, 0, i));
  rgb.show();
  delay(10);

  }
//beyaz
    for(int i=0;i<=255;i++){

  rgb.setPixelColor(0, rgb.Color(i, i, i));
  rgb.show();
  delay(10);

  }
  for(int i=255;i>=0;i--){

  rgb.setPixelColor(0, rgb.Color(i, i, i));
  rgb.show();
  delay(10);

  }
  
  
}