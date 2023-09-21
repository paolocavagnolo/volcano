'''
List of sensors:

A0      Potentiometer   INPUT
A2      LDR             INPUT
A5      Mic             INPUT
I2C     LIS3DH_I2C      INPUT
D2      Led             OUTPUT
D4      Buzzer          OUTPUT
D6      Button          OUTPUT
'''

from machine import Pin, I2C, ADC, PWM

ADC.WIDTH_12BIT

pot = ADC(1)

ldr = ADC(3)

mic = ADC(13)

btn = Pin(5, Pin.IN)

led = PWM(Pin(7))
led.freq(1047)

import lis3dh, time, math, random

i2c = I2C(sda=Pin(11), scl=Pin(12))
imu = lis3dh.LIS3DH_I2C(i2c, address=0x19)

# global variables

last_convert_time = 0
convert_interval = 100 #ms
pitch = 0
roll = 0

firstOn = True
firstOff = True

tPOT = 0
tLDR = 0
tIMU = 0
tMIC = 0

def readImu():

    global last_convert_time, convert_interval, roll, pitch

    if last_convert_time < time.ticks_ms():
        last_convert_time = time.ticks_ms() + convert_interval

        if imu.device_check():

            imu.range = lis3dh.RANGE_2_G
            x_Buff = imu.acceleration[0]
            y_Buff = imu.acceleration[1] 
            z_Buff = imu.acceleration[2] 

            roll = math.atan2(y_Buff , z_Buff) * 57.3
            pitch = math.atan2((- x_Buff) , math.sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3

            return (pitch,roll)

state = 0       
minVal = 0
maxVal = 4095

while True:

    if btn.value() == 1:   #if button is pressed
        if firstOn:
            firstOff = True
            firstOn = False
            led.duty_u16(65535)
            state = state + 1
            if (state > 3):
                state = 0
            print(state)
            time.sleep(0.1) #debounce
    else:
        if firstOff:
            firstOn = True
            firstOff = False
            time.sleep(0.1) #debounce

    if state == 0:
        if (time.ticks_ms() - tPOT) > 50:
            tPOT = time.ticks_ms()
            vPOT = pot.read()
            led.duty_u16(int(vPOT/maxVal*65535))
            print("%d,%d,%d" % (vPOT,minVal,maxVal))
    elif state == 1:
        if (time.ticks_ms() - tLDR) > 100:
            tLDR = time.ticks_ms()
            vLDR = ldr.read()*ldr.read()/1000
            if vLDR > maxVal:
                vLDR = maxVal
            led.duty_u16(int(vLDR/maxVal*65535))
            print("%.1f,%d,%d" % (vLDR,minVal,maxVal))
    elif state == 2:
        if (time.ticks_ms() - tMIC) > 50:
            tMIC = time.ticks_ms()
            vMIC = mic.read()
            led.duty_u16(int(vMIC/maxVal*65535))
            print("%d,%d,%d" % (vMIC,minVal,maxVal))
    elif state == 3:
        if (time.ticks_ms() - tIMU) > 110:
            tIMU = time.ticks_ms()
            p,r = readImu()
            p = int((p + 180)/360*maxVal)
            r = int((r + 180)/360*maxVal)
            led.duty_u16(int((r/maxVal)*65535))
            print("%d,%d,%d,%d" % (p,r,minVal,maxVal))
        
