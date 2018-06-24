#include <Adafruit_NeoPixel.h>
#include <inttypes.h>

#define NEOPIXEL_PIN 1

// We use "pixel" to refer to a single WS2812B package, which contains three LEDs (red, green and blue).

// Select number of pixels.
#define NUM_PIXELS 32

// We copy some type name defines here to keep the sketch self contained.
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define s16 int16_t

// Create the NeoPixel driver object. On the PC, this becomes the emulator.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// This must be below the short int type names and the NeoPixel library init.
#ifndef Arduino_h
#include "arduino_sketch.hpp"
// When running on the PC, we include stdio.h for printf() debugging.
#include <stdio.h>
#endif

// Example of wrapping stuff that will only compile in the IDE.
uint8_t  offset = 0; // Position of spinny eyes
uint32_t hue  = 0xFF0000; // Start red
uint8_t mode = 1;
uint32_t prevTime;
 
#ifdef Arduino_h
void debugMarker()
{
  pinMode(1, OUTPUT);
  for (u8 i = 0; i < 10; ++i) {
    digitalWrite(1, HIGH);
    delay(500);
    digitalWrite(1, LOW);
    delay(500);
  }
}
#endif

// End of boilerplate.

void setup() {
// Arduino_h is only defined when compiling the sketch in the Arduino IDE.
#ifndef Arduino_h
    pixels.setPixelLayout(DoubleRing);
#endif
    pixels.begin();
    // setBrightness() is intended for use only in setup() and not for animations. It's ignored by the emulator (see
    // the README).
    pixels.setBrightness(50);
    pixels.show();
    prevTime = millis();
}

void loop(){
  uint8_t  i;
  uint32_t t;

  switch (mode) {
    case 0:
      ring(i, t);
      break;
    case 1:
      fillRing(i, t);
      break;
  }
}

void ring(uint8_t i, uint32_t t) {
  for(i=0; i<16; i++) {
    uint32_t c = 0;
    if(((offset + i) & 7) < 2) c = hue;
    //if(((offset + i) & 8 && (offset + i) & 7 < 2)) c = hue + 128;
    pixels.setPixelColor(   i, c); // First eye
    pixels.setPixelColor(31-i, c); // Second eye (flipped)
  }
  pixels.show();
  offset++;
  delay(50);
}

void fillRing(uint8_t i, uint32_t t) {
  for(i=0; i<16; i++) {
    uint32_t c = 0;
    if(((offset + i) & 7) < 1) {
      pixels.setPixelColor(   i, hue); // First eye
      pixels.setPixelColor(31-i, hue); // Second eye (flipped)
    }
  }
  pixels.show();
  offset++;
  delay(50);
}
