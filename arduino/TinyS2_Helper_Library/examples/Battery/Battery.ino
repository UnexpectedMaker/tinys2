#include <TinyS2.h>

UMTINYS2 ts2;

void setup() {
  Serial.begin(115200);

  // Initialize all board peripherals, call this first
  ts2.begin();

  // Brightness is 0-255. We set it to 1/3 brightness here
  ts2.setPixelBrightness(255 / 3);
}

// Gets the battery voltage and shows it using the neopixel LED.
// These values are all approximate, you should do your own testing and
// find values that work for you.
void checkBattery() {
  // Get the battery voltage, corrected for the on-board voltage divider
  // Full should be around 4.2v and empty should be around 3v
  float battery = ts2.getBatteryVoltage();
  Serial.println(String("Battery: ") + battery);

  if (ts2.getVbusPresent()) {
    // If USB power is present
    if (battery < 3.9) {
      // Charging - blue
      ts2.setPixelColor(0x0000FF);
    } else {
      // Close to full - off
      ts2.setPixelColor(0x000000);
    }
  } else {
    // Else, USB power is not present (running from battery)
    if (battery < 3.1) {
      // Uncomment the following line to sleep when the battery is critically low
      //esp_deep_sleep_start();
    } else if (battery < 3.3) {
      // Below 3.3v - red
      ts2.setPixelColor(0xFF0000);
    } else if (battery < 3.6) {
      // Below 3.6v (around 50%) - orange
      ts2.setPixelColor(0xFF8800);
    } else {
      // Above 3.6v - green
      ts2.setPixelColor(0x00FF00);
    }
  }
}

// Store the millis of the last battery check
unsigned long lastBatteryCheck = 0;
// Define the battery check interval as one second
#define BATTERY_CHECK_INTERVAL 1000

void loop() {
  if (lastBatteryCheck == 0 || millis() - lastBatteryCheck > BATTERY_CHECK_INTERVAL) {
    checkBattery();
    lastBatteryCheck = millis();
  }
}