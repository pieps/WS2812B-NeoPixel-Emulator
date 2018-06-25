/* This file was automatically generated.  Do not edit! */
extern float hue;
uint32_t redScale(uint32_t hue);
void resetFrameBuffer();
void nextMode(uint32_t t);
void shady(uint32_t t);
void tracers(uint32_t t);
void quadBlink(uint32_t t);
void blink(uint32_t t);
void loop();
void setup();
uint32_t HueToRgb(float h,float s,float v);
uint32_t HueToRgb(float h);
uint32_t getHue();
void exitMode(uint32_t t);
void enterMode(uint32_t t);
void fillRing(uint8_t i,uint32_t t);
void fillRing(uint32_t t);
void ring(uint8_t i,uint32_t t);
void ring(uint32_t t);
#if defined(Arduino_h)
void debugMarker();
#endif
extern bool forceMode;
extern uint8_t iterations;
extern int8_t inc;
extern bool color;
extern uint32_t prevTime;
extern uint8_t mode;
extern float brightness;
extern uint32_t rgb;
extern int8_t offset;
extern Adafruit_NeoPixel pixels;
