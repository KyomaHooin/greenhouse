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
 -CD4021B + SPI + ShiftIn

ILI9325:

 -240x320 ID 0x9325
 -PROGMEM 8-bit bitmap / 16-bit "565" bitmap
 -Touch pinout

  YP A2
  XM A3
  YM 8
  XP 9

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
 mcufriend_kbv.zip - TFT 2.4 LCD library by David Prentice.
   TouchScreen.zip - TouchScreen libraary by Adafruit.
  adafruit_GFX.zip - Graphic library by Adarfruit.

 greenhouse.ino - Main program.
     openhard.h - PROGMEM logo.
</pre>

CONTACT

Author: richard.bruna@protonmail.com<br>
Source: https://github.com/KyomaHooin/greenhouse

