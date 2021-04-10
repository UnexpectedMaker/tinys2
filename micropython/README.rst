TinyS2 MicroPython Support
==========================

In this folder you will find pre-release MicroPython firmware for the TinyS2 as well as the tinys2 helper library and example code

Flashing your TinyS2 with MicroPython
-------------

Put your TinyS2 into `download mode` by holding the [boot] button down, pressing [reset] then letting go of the [boot] button.

Now you need to determine which device/port your TinyS2 is connectd to. On macOS it should be ``/dev/cu.usbmodem01`` - on Linux it will likely be ``/dev/ttyUSB0`` and on Windows it will be one of your ``COM`` ports.

First we erase the flash:

``esptool.py --chip esp32s2 --port /dev/cu.usbmodem01 erase_flash``

Now we write the microPython firmware to the flash - make sure you have downloaded the firmware from the ``alpha-firmware`` folder:

``esptool.py --port /dev/cu.usbmodem01 -b 921600 --before default_reset --after no_reset --chip esp32s2 write_flash --flash_mode dio --flash_size detect --flash_freq 80m 0x1000 firmware_2021_04_10.bin``


TinyS2 Helper library
---------

This library adds some helper functions and useful pin assignments to make coding with your TinyS2 & MicroPython easier.


TinyS2 Hardware Pin Assignments
---------------
.. code-block:: python

    # Sense Pins
    VBUS_SENSE = const(21)
    VBAT_SENSE = const(3)


    # RGB LED Pins
    RGB_DATA = const(1)
    RGB_PWR = const(2)

    # SPI
    SPI_MOSI = const(35)
    SPI_MISO = const(36)
    SPI_CLK = const(37)

    # I2C
    I2C_SDA = const(8)
    I2C_SCL = const(9)

    # DAC
    DAC1 = const(17)
    DAC2 = const(18)
..

Helper functions
----------------
.. code-block:: python

    # Enable or Disable power to the onboard NeoPixel to either show colour, or to reduce power when not in use.
    def set_pixel_power(state)

    # Returns the current battery voltage. If no battery is connected, returns 4.2V which is the charge voltage.
        This is an approximation only, but useful to detect if the voltage of the battery is getting low.
    def get_battery_voltage()

    # Detect if VBUS (5V) power source is present.
    def get_vbus_present()

    # Color wheel to allow for cycling through the rainbow of RGB colors.
    def rgb_color_wheel(wheel_pos)

    # Deep sleep helper that also powers down the on-board RGB LED.
    def go_deepsleep(t)
..

Example Usage
-------------
.. code-block:: python

    from machine import Pin
    import tinys2
    from neopixel import NeoPixel
    import time, random, micropython, gc, esp32

    # Configure the RGB LED
    pixel = NeoPixel(Pin(tinys2.RGB_DATA), 1) # Just one RGB LED

    # Turn on the power to the DotStar
    tinys2.set_pixel_power( True )

    # Say hello
    print("\nHello from TinyS2!")
    print("------------------\n")

    # Show available memory
    print("Memory Info - gc.mem_free()")
    print("---------------------------")
    print("{} Bytes\n".format(gc.mem_free()))

    p = esp32.Partition('vfs')
    flash_size = p.ioctl(4, 0) * p.ioctl(5, 0)

    # Show flash size
    print("Flash - esp32.Partition('vfs')")
    print("------------------------------")
    print("Size: {} Bytes\n".format(flash_size))

    # Show VBAT and VBUS sense
    print("VBAT Voltage {}V\n".format(tinys2.get_battery_voltage()))
    print("VBUS Sense - 5V present? {}\n".format("Yes" if tinys2.get_vbus_present() else "No"))

    # Create a colour wheel index int
    color_index = 0

    print("RGB time!")

    # Rainbow colours on the Dotstar
    while True:
        # Get the R,G,B values of the next colour
        r,g,b = tinys2.rgb_color_wheel( color_index )
        # Set the colour on the dotstar
        pixel[0] = ( r, g, b, 0.5)
        pixel.write()
        # Increase the wheel index
        color_index += 1
        # Sleep for 20ms so the colour cycle isn't too fast
        time.sleep_ms(20)
        
..
