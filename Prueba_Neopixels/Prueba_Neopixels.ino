#include <Adafruit_NeoPixel.h>
#define BUZZER_PIN 13     // Pin del buzzer
#define BUZZER_CHANNEL 0 // Canal PWM del buzzer
// Primer parámetro cantidad de pixeles en la tira
// Segundo parámetro pin de conexión a Arduino.
// Tercer parámetro:

// NEO_KHZ800 800 KHz velocidad de datos (WS2812 y nuevos)
// NEO_KHZ400 400 KHz velocidad de datos (WS2811 y más antiguos)
// NEO GRB flujo de datos en orden GRB (WS2812 y nuevos)
// NEO RGB flujo de datos en orden RGB (versiones más antiguas)

Adafruit_NeoPixel tira5 = Adafruit_NeoPixel (8, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tira32 = Adafruit_NeoPixel (8, 32, NEO_GRB + NEO_KHZ800);

unsigned long milisActuales;
unsigned long milisPrevios;

void setup() {

  tira5.begin();
  tira5.setBrightness(100); // brillo global para toda la tira

  tira32.begin();
  tira32.setBrightness(100); // brillo global para toda la tira
}

void loop() {

  milisActuales = millis();

  // tira5();

  Tira32Naranja();

  delay(1000);
  
  Tira32Verde();
  
  delay(1000);

}

void Tira5() {

  tira5.setPixelColor(0, 255, 48, 0);
  tira5.setPixelColor(1, 255, 48, 0);
  tira5.setPixelColor(2, 255, 48, 0);
  tira5.setPixelColor(3, 255, 48, 0);
  tira5.setPixelColor(4, 255, 48, 0);
  tira5.setPixelColor(5, 255, 48, 0);
  tira5.setPixelColor(6, 255, 48, 0);
  tira5.setPixelColor(7, 255, 48, 0);
  tira5.show();

}

void Tira32Naranja() {

  tira32.setPixelColor(0, 255, 48, 0);
  tira32.setPixelColor(1, 255, 48, 0);
  tira32.setPixelColor(2, 255, 48, 0);
  tira32.setPixelColor(3, 255, 48, 0);
  tira32.setPixelColor(4, 255, 48, 0);
  tira32.setPixelColor(5, 255, 48, 0);
  tira32.setPixelColor(6, 255, 48, 0);
  tira32.setPixelColor(7, 255, 48, 0);
  tira32.show();

}

void Tira32Verde() {

  tira32.setPixelColor(0, 0, 143, 57);
  tira32.setPixelColor(1, 0, 143, 57);
  tira32.setPixelColor(2, 0, 143, 57);
  tira32.setPixelColor(3, 0, 143, 57);
  tira32.setPixelColor(4, 0, 143, 57);
  tira32.setPixelColor(5, 0, 143, 57);
  tira32.setPixelColor(6, 0, 143, 57);
  tira32.setPixelColor(7, 0, 143, 57);
  tira32.show();

}
