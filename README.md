## LUFA Switch/XInput hybrid Fightstick

I was really interested in creating a box smash ultimate controller using [Crane's DIY instructions](https://github.com/Crane1195/DIYB0XX). I wanted to make one of mine work both on nintendo switch and PC and without the use of a game cube controller cable. I fused Crane's and CrazyRedMachine's code and added my own to create a controller usable on switch and PC via USB with and Arduino Leonardo.

## Acknowledgments

This work is based on [Crane's USB DIYBOX](https://github.com/Crane1195/DIYB0XX/tree/master/code/DIYGCC_USB_Switch_Beta) and [CrazyRedMachine LUFA stick](https://github.com/CrazyRedMachine/LUFAHybridFightstick). 
## Features

### Gamepad modes

Hold Nothing or "select" as you plug the controller to make it a Nintendo Switch controller with smash ultimate mode.

Hold "start" as you plug the controller to make it an XInput controller fighting game style with cstick added.

### Layout modes

You can switch between the 2 stick modes on switch by pressing START+SELECT+ mode key:

- FGC_SWITCH ( C stick left)
- SMASH_ULTIMATE_SWITCH (C stick up)

For PC you only have access to an xinput controller with hitbox style layout + c stick
- FGC_PC

### Ultimate mode

To make the ultimate mode work as basically intended you will have to create a profile for the pro controller and change the L button to shield.

### SOCD modes

You can switch on both nintendo switch and pc between 3 socd modes pressing START+SELECT+MY+ mode key:
- 2IP ( C stick up)
- 2IPNoReactivation ( C stick right)
- Neutral ( C stick left)

Note that SOCD are defaulted on connecting the device:
- 2IP for SMASH_ULTIMATE_SWITCH
- Neutral for FGC_PC

### Simulated home button

I kept CrazyRedMachine code so that holding start+select during more than 1 second presses the home button. You can customize the delay with #define HOME_DELAY 1000 in the .ino file.

## Building Instructions

- Download Arduino IDE, 
- Download the Bounce2 Library inside the Arduino IDE
- Download Arduino Lufa from https://github.com/Palatis/Arduino-Lufa and follow the instructions to install and activate it (Manual install if the first doesn't work)
- Build and Flash for your ATMEGA32U4 Board
- Have Fun

## Pinout

Configurable in the .ino

```
/* PINOUT (follows Nintendo naming (X=up, B=down)) */
#define PIN_UP    5   //frame 1 UP
#define PIN_DOWN  A1  //frame 1 DOWN  
#define PIN_LEFT  A0  //frame 1 LEFT
#define PIN_RIGHT A2  //frame 1 RIGHT
#define PIN_A     9   //frame 1 A
#define PIN_B     2   //frame 1 B
#define PIN_X     3   //frame 1 X
#define PIN_Y     14   //frame 1 Y
#define PIN_L     A3   //frame 1 L
#define PIN_R     4    //frame 1 Z
#define PIN_ZL    0    //frame 1 LS
#define PIN_ZR    16   //frame 1 R
#define PIN_PLUS  11   //frame 1 Start
#define PIN_MINUS 13   // add a select button 
#define PIN_HOME  12   // add a home button
#define PIN_MX  A4 //MX modifier for range of left stick //frame1 MX
#define PIN_MY  A5 //MY modifier for range of left stick //frame1 MY
#define PIN_CUP    7 // frame 1 C stick UP
#define PIN_CDOWN  10 // frame 1 C stick DOWN
#define PIN_CLEFT  8 // frame 1 C stick LEFT
#define PIN_CRIGHT 6 // frame 1 C stick RIGHT
#define PIN_EXTRA 1 //frame 1 MS
```
