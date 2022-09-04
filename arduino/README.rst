TinyS2 Arduino Helper
=======================

This library adds some helper functions and useful pin assignments to make coding with TinyS2 & Arduino easier

We will be adding this library to the Arduino IDE library manager once we get closer to shipping the TinyS2.

TinyS2 Hardware Pin Assignments
-------------------------------
.. code-block:: c++

    // RGB LED
    #define RGB_DATA 1
    #define RGB_PWR 2

    // Battery
    #define VBAT_SENSE 3
    #define VBUS_SENSE 21
..

Helper functions
----------------
.. code-block:: c++

    // Class constructor
    TinyS2();

    // Get a *rough* estimate of the current battery voltage
    // If the battery is not present, the charge IC will still report it's trying to charge at X voltage
    // so it will still show a voltage.
    float GetBatteryVoltage();

    // Return the current charge state of the battery - we need to read the value multiple times
    // to eliminate false negatives due to the charge IC not knowing the difference between no battery
    // and a full battery not charging - This is why the charge LED flashes
    bool IsChargingBattery();

    // Power to the on-board NeoPixel is controlled by a PNP transistor, so low is ON and high is OFF
    // We also need to set the NeoPixel data pin to be an input to prevent power leakage when power is off
    // The reason we have power control for the NeoPixel is that it has a quiescent current of around 1mA, so we
    // need to be able to cut power to it to minimise power consumption during deep sleep or with general battery powered use
    // to minimise unneeded battery drain
    void NeoPixel_SetPower(bool state);

    // On-board NeoPixel RGB LED control
    void NeoPixel_Clear();
    void NeoPixel_SetBrightness(uint8_t);
    void NeoPixel_SetPixelColor(uint32_t c);
    void NeoPixel_SetPixelColor(uint8_t r, uint8_t g, uint8_t b);
    void NeoPixel_Show(void);
    void NeoPixel_CycleColor();
    void NeoPixel_CycleColor(unsigned long wait);
    void NeoPixel_CycleColor();
    void NeoPixel_CycleColor(unsigned long wait);

    // Convert R,G,B values to uint32_t
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
..

Example Usage
-------------
.. code-block:: c++

    #include <TinyS2.h>

    // Interval between internal temperature reads
    unsigned long next_temp_read = 0;   // Next time step in milliseconds
    uint8_t temp_read_interval = 1000;  // This is in milliseconds

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
..
