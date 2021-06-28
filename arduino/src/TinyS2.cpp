// ---------------------------------------------------------------------------
// Created by Seon Rozenblum - seon@unexpectedmaker.com
// Copyright 2021 License: MIT https://github.com/tinypico/tinypico-arduino/blob/master/LICENSE
//
// See "TinyS2.h" for purpose, syntax, version history, links, and more.
//
// v1.0 - Initial Release
// ---------------------------------------------------------------------------

#include "TinyS2.h"
#include <SPI.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"

// Battery divider resistor values
#define UPPER_DIVIDER 442
#define LOWER_DIVIDER 160
#define DEFAULT_VREF  1100  // Default reference voltage in mv
#define BAT_ADC_CH ADC1_CHANNEL_2  // Battery voltage ADC input

TinyS2::TinyS2()
{
    pinMode(RGB_PWR, OUTPUT);
    pinMode(VBUS_SENSE, INPUT); // is charging battery
    pinMode(VBAT_SENSE, INPUT); // estimated battery voltage

    NeoPixel_SetPower(false);
    nextVoltage = millis();

    for (int i = 0; i < 3; i++) {
        pixel[i] = 0;
    }

    isInit = false;
    brightness = 128;
    colorRotation = 0;
    nextRotation = 0;
}

TinyS2::~TinyS2()
{
    isInit = false;
    NeoPixel_SetPower(false);
}

void TinyS2::NeoPixel_SetBrightness(uint8_t b)
{
    // Stored brightness value is different than what's passed.  This
    // optimizes the actual scaling math later, allowing a fast 8x8-bit
    // multiply and taking the MSB.  'brightness' is a uint8_t, adding 1
    // here may (intentionally) roll over...so 0 = max brightness (color
    // values are interpreted literally; no scaling), 1 = min brightness
    // (off), 255 = just below max brightness.
    brightness = b + 1;
}

// Convert separate R,G,B to packed value
uint32_t TinyS2::Color(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

void TinyS2::NeoPixel_Show(void)
{
    if (!isInit) {
        isInit = true;
        swspi_init();
        delay(10);
    }

    uint16_t b16 = (uint16_t)brightness;  // Type-convert for fixed-point math

    // Start-frame marker
    for (int i=0; i<4; i++) {
        swspi_out(0x00);
    }

    // Pixel start
    swspi_out(0xFF);

    for (int i=0; i<3; i++) {
        if (brightness > 0) {
            swspi_out((pixel[i] * b16) >> 8);  // Scale, write - Scaling pixel brightness on output
        }
        else {
            swspi_out(pixel[i]);  // R,G,B @Full brightness (no scaling)
        }
    }

    // // End frame marker
    swspi_out(0xFF);
}

void TinyS2::swspi_out(uint8_t n)
{
    for (uint8_t i=8; i--; n <<= 1) {
        if (n & 0x80) {
            digitalWrite(RGB_DATA, HIGH);
        }
        else {
            digitalWrite(RGB_DATA, LOW);
        }
        digitalWrite(RGB_CLK, HIGH);
        digitalWrite(RGB_CLK, LOW);
    }
    delay(1);
}

void TinyS2::NeoPixel_Clear() { // Write 0s (off) to full pixel buffer
    for (int i = 0; i < 3; i++) {
        pixel[i] = 0;
    }

    NeoPixel_Show();
}

// Set pixel color, separate R,G,B values (0-255 ea.)
void TinyS2::NeoPixel_SetPixelColor(uint8_t r, uint8_t g, uint8_t b)
{
    pixel[0] = b;
    pixel[1] = g;
    pixel[2] = r;

    NeoPixel_Show();
}

// Set pixel color, 'packed' RGB value (0x000000 - 0xFFFFFF)
void TinyS2::NeoPixel_SetPixelColor(uint32_t c)
{
    pixel[0] = (uint8_t)c;
    pixel[1] = (uint8_t)(c >> 8);
    pixel[2] = (uint8_t)(c >> 16);

    NeoPixel_Show();
}

void TinyS2::swspi_init(void)
{
    NeoPixel_SetPower(true);
    digitalWrite(RGB_DATA, LOW);
    digitalWrite(RGB_CLK, LOW);
}

void TinyS2::swspi_end()
{
    NeoPixel_SetPower(false);
}

// Switch the RGB power
void TinyS2::NeoPixel_SetPower( bool state )
{
    digitalWrite(RGB_PWR, !state);
    pinMode(RGB_DATA, state ? OUTPUT : INPUT_PULLDOWN);
    pinMode(RGB_CLK, state ? OUTPUT : INPUT_PULLDOWN);
}

void TinyS2::NeoPixel_CycleColor()
{
    NeoPixel_CycleColor(0);
}

void TinyS2::NeoPixel_CycleColor(unsigned long wait = 0)
{
    if (millis() > nextRotation + wait) {
        nextRotation = millis();

        colorRotation++;
        byte WheelPos = 255 - colorRotation;
        if (WheelPos < 85) {
            NeoPixel_SetPixelColor(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        else if (WheelPos < 170) {
            WheelPos -= 85;
            NeoPixel_SetPixelColor(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        else {
            WheelPos -= 170;
            NeoPixel_SetPixelColor(WheelPos * 3, 255 - WheelPos * 3, 0);
        }
        NeoPixel_Show();
    }
}

// Return the current charge state of the battery
bool TinyS2::IsChargingBattery()
{
    int measuredVal = 0;
    for (int i = 0; i < 10; i++) {
        int v = digitalRead(VBUS_SENSE);
        measuredVal += v;
    }

    return (measuredVal == 0);
}

// Return a *rough* estimate of the current battery voltage
float TinyS2::GetBatteryVoltage()
{
    uint32_t raw, mv;
    esp_adc_cal_characteristics_t chars;

    // only check voltage every 1 second
    if (nextVoltage - millis() > 0) {
        nextVoltage = millis() + 1000;

        // grab latest voltage
        analogRead(VBAT_SENSE);  // Just to get the ADC setup
        raw = adc1_get_raw(BAT_ADC_CH);  // Read of raw ADC value

        // Get ADC calibration values
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_BIT_12, DEFAULT_VREF, &chars);

        // Convert to calibrated mv then volts
        mv = esp_adc_cal_raw_to_voltage(raw, &chars) * (LOWER_DIVIDER + UPPER_DIVIDER) / LOWER_DIVIDER;
        lastMeasuredVoltage = (float)mv / 1000.0;
    }

    return (lastMeasuredVoltage);
}

// Tone - Sound wrapper
void TinyS2::Tone(uint8_t pin, uint32_t freq)
{
    if (!isToneInit) {
        pinMode(pin, OUTPUT);
        ledcSetup(0, freq, 8);  // Channel 0, resolution 8
        ledcAttachPin(pin, 0);
        isToneInit = true;
    }

    ledcWriteTone(0, freq);
}

void TinyS2::NoTone(uint8_t pin)
{
    if (isToneInit) {
        ledcWriteTone(0, 0);
        pinMode(pin, INPUT_PULLDOWN);
        isToneInit = false;
    }
}
