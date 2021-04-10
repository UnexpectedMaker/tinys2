# TinyS2 Hello World Code
# 2021 Seon Rozenblum, Unexpected Maker
#
# Project home:
#   https://tinys2.io
#
import time, gc, os
import neopixel
import board, digitalio
import tinys2

# Create a NeoPixel instance
# Brightness of 0.3 is ample for the 1515 sized LED
pixel = neopixel.NeoPixel(board.NEOPIXEL, 1, brightness=0.3, auto_write=True, pixel_order=neopixel.GRB)

# Say hello
print("\nHello from TinyS2!")
print("------------------\n")

# Show available memory
print("Memory Info - gc.mem_free()")
print("---------------------------")
print("{} Bytes\n".format(gc.mem_free()))

flash = os.statvfs('/')
flash_size = flash[0] * flash[2]
flash_free = flash[0] * flash[3]
# Show flash size
print("Flash - os.statvfs('/')")
print("---------------------------")
print("Size: {} Bytes\nFree: {} Bytes\n".format(flash_size, flash_free))

# Show VBAT and VBUS sense
print("VBAT Voltage {}V\n".format(tinys2.get_battery_voltage()))
print("VBUS Sense - 5V present? {}\n".format("Yes" if tinys2.get_vbus_present() else "No"))

print("Pixel Time!\n")

# Create a colour wheel index int
color_index = 0

# Turn on the power to the NeoPixel
tinys2.set_pixel_power(True)

# Rainbow colours on the NeoPixel
while True:
    # Get the R,G,B values of the next colour
    r,g,b = tinys2.rgb_color_wheel( color_index )
    # Set the colour on the NeoPixel
    pixel[0] = ( r, g, b, 0.5)
    # Increase the wheel index
    color_index += 1
        
    # Sleep for 15ms so the colour cycle isn't too fast
    time.sleep(0.015)
    
