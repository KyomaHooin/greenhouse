![Greenhouse Logo](https://github.com/KyomaHooin/greenhouse/raw/master/greenhouse_2_screen.jpg "screenshot")
![Greenhouse Menu](https://github.com/KyomaHooin/greenhouse/raw/master/greenhouse_1_screen.jpg "screenshot")

DESCRIPTION

Automatic "Greenhouse" light system.

NOTE

<pre>
UNO R3:

 -USB B
 -PWM 3,5,6,9,10,11
 -74HC595 + SPI + ShiftPWM
 -CD4021B + SPIN + ShiftIn

ILI9325:

 -240x320 ID 0x9325
 -PROGMEM 8-bit bitmap / 16-bit "565" bitmap
 -Touch pinout

  #define YP A2
  #define XM A3
  #define YM 8
  #define XP 9

 -"shared pins" => reset(OUTPUT)

TTP223:

 -5V board = 5V logic
 -15s press duration
 -high sensitivity

Sensor shield v4.0:

 -AO..A5 = SVG

IRF540N:

 -servo cable => 3x1 BLS/PLS
 -9-12V min.
 -DC only
</pre>

FILE

<pre>
 mcufriend_kbv - TFT 2.4 LCD library by David Prentice.
   TouchScreen - TouchScreen libraary by Adafruit.
  adafruit_GFX - Graphic library by Adarfruit.

greenhouse.ino - Main program.
     touch.ino - TP223 test code.
       fet.ino - MOSFET test code.
       pwm.ino - Soft PWM test code
</pre>

CONTACT

Author: richard_bruna@nm.cz<br>
Source: https://github.com/KyomaHooin/greenhouse

