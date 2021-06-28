// ---------------------------------------------------------------------------
// TinyS2 Helper Library - v1.0 - 14/04/2021
//
// AUTHOR/LICENSE:
// Created by Seon Rozenblum - seon@unexpectedmaker.com
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// LINKS:
// Project home: http://tinys2.io
// Blog: http://unexpectedmaker.com
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no
// warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
// Helper Library for the TinyS2 http://tinys2.io
//
// HISTORY:
// v1.0 - Initial Release
// ---------------------------------------------------------------------------

#ifndef TinyS2_h
    #define TinyS2_h

    #if defined(ARDUINO) && ARDUINO >= 100
        #include <Arduino.h>
    #else
        #include <WProgram.h>
        #include <pins_arduino.h>
    #endif

    #include <SPI.h>

    #define RGB_PWR 2
    #define RGB_DATA 1

    #define VBAT_SENSE 3
    #define VBUS_SENSE 21

    class TinyS2
    {
        public:
            TinyS2();
            ~TinyS2();

            // TinyS2 Features
            void NeoPixel_SetPower(bool state);
            float GetBatteryVoltage();
            bool IsChargingBattery();

            // RGB LED
            void NeoPixel_Clear();                            // Set all pixel data to zero
            void NeoPixel_SetBrightness(uint8_t);             // Set global brightness 0-255
            void NeoPixel_SetPixelColor(uint32_t c);
            void NeoPixel_SetPixelColor(uint8_t r, uint8_t g, uint8_t b);
            void NeoPixel_Show(void);                         // Issue color data to strip
            void NeoPixel_CycleColor();
            void NeoPixel_CycleColor(unsigned long wait);
            uint32_t Color(uint8_t r, uint8_t g, uint8_t b);  // R,G,B to 32-bit color

            // Tone for making sound on any ESP32 - just using channel 0
            void Tone(uint8_t, uint32_t);
            void NoTone(uint8_t);

        protected:
            void swspi_init(void);                            // Start bitbang SPI
            void swspi_out(uint8_t n);                        // Bitbang SPI write
            void swspi_end(void);                             // Stop bitbang SPI

        private:
            unsigned long nextVoltage;
            float lastMeasuredVoltage;
            byte colorRotation;
            unsigned long nextRotation;
            uint8_t brightness;                               // Global brightness setting
            uint8_t pixel[3];                                 // LED RGB values (3 bytes ea.)
            bool isInit;
            bool isToneInit;
    };

#endif
