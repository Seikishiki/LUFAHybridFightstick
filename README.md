## LUFA Switch/XInput hybrid Fightstick (PSX Edition)

This code can be used to make a PSX Gamepad work on Nintendo Switch and XBox360 (or PC in XInput mode).

I've put an arduino pro micro directly inside the controller, but you could also go non-destructive and use this code to make an external usb adapter.

## Demo

https://www.instagram.com/p/CFIj8BZFxf4/

## Acknowledgments

This work is based on [fluffymadness' ATMega32U4-Switch-Fightstick](https://github.com/fluffymadness/ATMega32U4-Switch-Fightstick)
 and [bootsector's XInputPadMicro](https://github.com/bootsector/XInputPadMicro), with a couple QoL improvements I needed.

The PSX pad read code is based on [PSX(PS1/2) pad library by AZO](https://github.com/AZO234/Arduino_PSXPad/).

## Features

### Gamepad modes

Hold "select" as you plug the controller to make it a Nintendo Switch controller.

Hold "start" as you plug the controller to make it an XInput controller.

Gamepad mode is persistent across plugging and unplugging the controller, so if you don't press anything while plugging the controller, it will stay in the last used mode. 

### DPAD modes

You can switch seamlessly between the 3 stick modes by pressing START+SELECT+(LEFT/UP/RIGHT).

- UP: DPAD
- LEFT: Fake Left Analog (DPad is mapped to L-Analog)
- RIGHT: Fake Right Analog (DPad is mapped to R-Analog)

DPAD mode is also persistent.

### Simulated home button

Because the PSX pad doesn't have a home button, I also added some code so that holding start+select during more than 1 second presses the home button. You can customize the delay with `#define HOME_DELAY 1000` in the .ino file.

## Building Instructions

- Download Arduino IDE, 
- Download Arduino Lufa from https://github.com/Palatis/Arduino-Lufa and follow the instructions to install and activate it
- Build and Flash for your ATMEGA32U4 Board
- Have Fun

## Pinout

When looking at the plug of the *controller cable* :
```
  1  2  3   4  5  6   7  8  9
-------------------------------
| o  o  o | o  o  o | o  o  o |  (at the Controller)
\_____________________________/

1 : DAT        -> to arduino MISO (on ICSP header) (with 1k ohm pullup resistor*)
2 : CMD        -> to arduino MOSI (on ICSP header)
4 : GND        -> to arduino GND
5 : 3.3V       -> to arduino 3.3V
6 : Attention  -> to arduino digital pin 2
7 : SCK        -> to arduino SCK (on ICSP header)

* Note: 1k ohm pullup resistor to 5V means you also have to put a 1k ohm resistor between 5V and MISO.
See the demo pic.

 ```

