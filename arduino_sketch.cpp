#include <Adafruit_NeoPixel.h>
#include <inttypes.h>

#define NEOPIXEL_PIN 1

// We use "pixel" to refer to a single WS2812B package, which contains three LEDs (red, green and blue).

// Select number of pixels.
#define NUM_PIXELS 32

// Create the NeoPixel driver object. On the PC, this becomes the emulator.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// This must be below the short int type names and the NeoPixel library init.
#ifndef Arduino_h
#include "arduino_sketch.hpp"
// When running on the PC, we include stdio.h for printf() debugging.
#include <stdio.h>
#endif

enum Mode {
  RING = 0,
  BLINK = 1,
  FILL_RING = 2,
  QUAD_BLINK = 3,
  TRACERS = 4,
  SHADY = 5,
  MODE_MAX = 6,
};

enum HueMode {
  RAINBOW = 0,
  GLOW = 1,
  HUE_MODE_MAX = 2,
};

// Example of wrapping stuff that will only compile in the IDE.
int8_t  offset = 0; // Position of spinny eyes
float hue = 0.f;
uint8_t mode = 0;
uint32_t prevTime;
bool color = true;
int8_t inc = 1;
uint8_t iterations = 0;
float glowRate = .0031415926535897932384626433832795;
uint8_t hueMode = RAINBOW;
bool forceMode = false;
 
#ifdef Arduino_h
void debugMarker()
{
  pinMode(1, OUTPUT);
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(1, HIGH);
    delay(500);
    digitalWrite(1, LOW);
    delay(500);
  }
}
#endif

// End of boilerplate.

void ring(uint8_t i, uint32_t t);
void fillRing(uint8_t i, uint32_t t);

void enterMode(uint32_t t) {
}
void exitMode(uint32_t t) {
}
uint32_t getHue(uint32_t t) {
  switch (hueMode) {
    case RAINBOW:
      hue = fmod(hue + 1, 360.f);
      return HueToRgb(hue);
    case GLOW:
      return HueToRgb(0.f, 1.f, fabs(sin(t * glowRate)));
  }
}

uint32_t HueToRgb(float h, float s, float v) {
  int c = s * v * 255;
  int m = (v * 255) - c;
  int x = c * (1.f - abs(fmod(h / 60.f, 2.f) - 1.f));
  switch ((int) h / 60) {
    case 0:
      return ((c + m) << 16) + ((x + m) << 8) + m;
    case 1:
      return ((x + m) << 16) + ((c + m) << 8) + m;
    case 2:
      return (m << 16) + ((c + m) << 8) + x + m;
    case 3:
      return (m << 16) + ((x + m) << 8) + c + m;
    case 4:
      return ((x + m) << 16) + (m << 8) + c + m;
    default: // 5
      return ((c + m) << 16) + (m << 8) + x + m;
	}
}

uint32_t HueToRgb(float h) {
  return HueToRgb(h, 1.f, 1.f);
}

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
  uint32_t t = millis();

  enterMode(t);
  switch (mode) {
    case RING:
      ring(t);
      break;
    case BLINK:
      blink(t);
      break;
    case FILL_RING:
      fillRing(t);
      break;
    case QUAD_BLINK:
      quadBlink(t);
      break;
    case TRACERS:
      tracers(t);
      break;
    case SHADY:
      shady(t);
      break;
  }
  exitMode(t);
}

void ring(uint32_t t) {
  for(int8_t i=0; i<16; i++) {
    uint32_t c = 0;
    if(((offset + i) & 7) < 2) {

      pixels.setPixelColor(   i, getHue(t)); // First eye
      pixels.setPixelColor(31-i, getHue(t)); // Second eye (flipped)
    }
  }
  pixels.show();
  offset++;
  delay(50);

  if (t - prevTime > 8000) {
    nextMode(t);
  }
}

void fillRing(uint32_t t) {
  for(int8_t i=0; i<16; i++) {
    uint32_t c = 0;
    if(((offset + i) & 7) < 1) {
      pixels.setPixelColor(   i, color ? getHue(t) : 0); // First eye
      pixels.setPixelColor(31-i, color ? getHue(t) : 0); // Second eye (flipped)
    }
  }
  pixels.show();
  offset++;
  if (offset == 16) {
    offset = 0;
    color = !color;
    iterations++;
  }

  delay(50);

  if (iterations == 8) {
    nextMode(t);
  }
}

void blink(uint32_t t) {
  if (iterations == 0) {
    offset = 8;
  }
  resetFrameBuffer();
  for(int8_t i=0; i<8; i++) {
    if(i < offset) {
      pixels.setPixelColor(i, getHue(t));
      pixels.setPixelColor(15 - i, getHue(t));
      pixels.setPixelColor(24 + i, getHue(t));
      pixels.setPixelColor(23 - i, getHue(t));
    }
  }
  pixels.show();
  delay(50);
  offset += inc;
  if (offset > 8 || offset < 0) {
    inc = -inc;
    iterations++;
    if (iterations == 4) {
      delay(500);
      nextMode(t);
    }
  }
}

void quadBlink(uint32_t t) {
  resetFrameBuffer();
  for(int8_t i=0; i<4; i++) {
    if(i < offset) {
      pixels.setPixelColor(3 - i, getHue(t));
      pixels.setPixelColor(4 + i, getHue(t));
      pixels.setPixelColor(11 - i, getHue(t));
      pixels.setPixelColor(12 + i, getHue(t));

      pixels.setPixelColor(28 + i, getHue(t));
      pixels.setPixelColor(27 - i, getHue(t));
      pixels.setPixelColor(20 + i, getHue(t));
      pixels.setPixelColor(19 - i, getHue(t));
    }
  }
  pixels.show();
  delay(50);
  offset += inc;
  if (offset > 4 || offset < 0) {
    inc = -inc;
    iterations++;
    if (iterations == 4) {
      delay(500);
      nextMode(t);
    }
  }
}

void tracers(uint32_t t) {
  resetFrameBuffer();
  for (uint8_t i = 0; i < 16; i++) {
    if(((offset + i) & 7) < 2) {
      pixels.setPixelColor(i, getHue(t));
      pixels.setPixelColor(31 - i, getHue(t));
    }
  }
  pixels.show();
  offset++;
  delay(50);
  if (t - prevTime > 8000) {
    nextMode(t);
  }
}

void shady(uint32_t t) {
  hueMode = GLOW;
  resetFrameBuffer();

  pixels.setPixelColor(4 + (inc * 4), getHue(t - prevTime));
  pixels.setPixelColor(11 + (-inc * 4), getHue(t - prevTime));
  pixels.setPixelColor(20 + (inc * 4), getHue(t - prevTime));
  pixels.setPixelColor(27 + (-inc * 4), getHue(t - prevTime));

  pixels.show();
  delay(50);
  if (t - prevTime > 1000) {
    prevTime = t;
    inc *= -1;
    iterations++;
  }
  if (iterations == 8) {
    nextMode(t);
  }
}

uint32_t redScale(uint32_t hue) {
  return (int) (hue * (1.f / 0xFFFFFF) * 0xFF) << 16;
}

void resetFrameBuffer() {
  for (int8_t i = 0; i  < 32; i++) {
    pixels.setPixelColor(i, 0);
  }
}

void nextMode(uint32_t t) {
  prevTime = t;
  offset = 0;
  inc = 1;
  color = true;
  hueMode = RAINBOW;
  if (!forceMode) {
    mode = (mode + 1) % MODE_MAX;
    iterations = 0;
  }
}
