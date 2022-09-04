#include <TinyS2.h>

UMTINYS2 ts2;

void setup() {
  // Initialize all board peripherals, call this first
  ts2.begin();

  // Brightness is 0-255. We set it to 1/3 brightness here
  ts2.setPixelBrightness(255 / 3);
}

int color = 0;

void loop() {
  // colorWheel cycles red, orange, ..., back to red at 256
  ts2.setPixelColor(UMTINYS2::colorWheel(color));
  color++;
  delay(15);
}