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
    