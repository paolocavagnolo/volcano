from machine import Pin, I2C
from utime import sleep

from dht20 import DHT20


i2c0_sda = Pin(12)
i2c0_scl = Pin(11)
i2c0 = I2C(0, sda=i2c0_sda, scl=i2c0_scl)

dht20 = DHT20(0x38, i2c0)

while True:
    measurements = dht20.measurements
    print(f"Temperature: {measurements['t']} °C, humidity: {measurements['rh']} %RH")
    sleep(1)

from machine import Pin, I2C
import ssd1306
from time import sleep
 
# ESP32 Pin assignment 
i2c = I2C(-1, scl=Pin(11), sda=Pin(12))
 
oled_width = 128
oled_height = 64
oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)
 
oled.text('Welcome', 0, 0)
oled.text('OLED Display', 0, 10)
oled.text('how2electronics', 0, 20)
oled.text('Makerfabs', 0, 30)
        
oled.show()

from machine import I2C, Pin
from bmp280 import *

bus = I2C(sda=Pin(12),scl=Pin(11))
print(bus.scan())
bmp = BMP280(bus, addr=0x77)

print(bmp.temperature)
print(bmp.pressure)
