#include <TinyS2.h>

// Initialise the TinyS2 library
TinyS2 ts = TinyS2();

void setup()
{
    // Not used
}

void loop()
{
    // Cycle the NeoPixel colour every 25 milliseconds
    ts.NeoPixel_CycleColor(25);

    // You can set the NeoPixel colour directly using r,g,b values
    // ts.NeoPixel_SetPixelColor(255, 128, 0);

    // You can set the NeoPixel colour directly using a uint32_t value
    // ts.NeoPixel_SetPixelColor(0xFFC900);

    // You can clear the NeoPixel too
    // ts.NeoPixel_Clear();

    // To power down the NeoPixel for deep sleep you call this
    // ts.NeoPixel_SetPower(false);
}
