#include "LUFAConfig.h"
#include <LUFA.h>
#include "XS_HID.h"
#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h>
#include <EEPROM.h>
#include <inttypes.h>

/* in case you want to disable one type of gamepad */
//#define DISABLE_NSWITCH 
//#define DISABLE_XINPUT

//make it so holding start+select triggers the HOME button
//#define HOME_HOTKEY
//delay in ms for start+select to become HOME in HOME_HOTKEY mode
#define HOME_DELAY 1000

/* PINOUT (follows Nintendo naming (X=up, B=down)) */
#define PIN_UP    5   //frame 1 UP
#define PIN_DOWN  A1  //frame 1 DOWN  
#define PIN_LEFT  A0  //frame 1 LEFT
#define PIN_RIGHT A2  //frame 1 RIGHT
#define PIN_A     9            //XBOX B //frame 1 A
#define PIN_B     2            //XBOX A //frame 1 B
#define PIN_X     3            //XBOX Y //frame 1 X
#define PIN_Y     14           //XBOX X //frame 1 Y
#define PIN_L     A3           //XBOX LB  //frame 1 L
#define PIN_R     4            //XBOX RB  //frame 1 Z
#define PIN_ZL    0            //XBOX LT  //frame 1 LS
#define PIN_ZR    16            //XBOX RT //frame 1 R
#define PIN_PLUS  11           //XBOX START //frame 1 Start
#define PIN_MINUS 13           //XBOX BACK // add a select button 
#define PIN_HOME  12   // add a home button
#define PIN_MX  A4 //MX modifier for range of left stick //frame1 MX
#define PIN_MY  A5 //MY modifier for range of left stick //frame1 MY
#define PIN_CUP    7 // frame 1 C stick UP
#define PIN_CDOWN  10 // frame 1 C stick DOWN
#define PIN_CLEFT  8 // frame 1 C stick LEFT
#define PIN_CRIGHT 6 // frame 1 C stick RIGHT
#define PIN_EXTRA 1 //frame 1 MS



/* Buttons declarations */
#define MILLIDEBOUNCE 1 //Debounce time in milliseconds
unsigned long startAndSelTime = 0;
unsigned long currTime = 0;

byte internalButtonStatus[4];


Bounce button1 = Bounce();
Bounce button2 = Bounce();
Bounce button3 = Bounce();
Bounce button4 = Bounce();
Bounce button5 = Bounce();
Bounce button6 = Bounce();
Bounce button7 = Bounce();
Bounce button8 = Bounce();
Bounce button9 = Bounce();
Bounce button10 = Bounce();
Bounce button11 = Bounce();
Bounce button12 = Bounce();
Bounce button13 = Bounce();
Bounce button14 = Bounce();
Bounce button15 = Bounce();
Bounce button16 = Bounce();
Bounce button17 = Bounce();
Bounce button18 = Bounce();
Bounce button19 = Bounce();
Bounce button20 = Bounce();
Bounce button21 = Bounce();
Bounce button22 = Bounce();

bool wasLEFT = false;
bool wasRIGHT = false;
bool wasUP = false;
bool wasDOWN = false;

bool wasCLEFT = false;
bool wasCRIGHT = false;
bool wasCUP = false;
bool wasCDOWN = false;

bool lockLEFT = false;
bool lockRIGHT = false;
bool lockUP = false;
bool lockDOWN = false;

bool lockCLEFT = false;
bool lockCRIGHT = false;
bool lockCUP = false;
bool lockCDOWN = false;

const uint8_t minValue = 0;
const uint8_t maxValue = 255;

/* MODE DECLARATIONS */
typedef enum {
  FGC_PC,
  NORMAL_PC,
  KEYBOARD_PC,
  FGC_SWITCH,
  SMASH_ULTIMATE_SWITCH
} State_t;
State_t state;

/* mode selectors */
bool xinput;
bool modeChanged;
bool modeChange;

/* SOCD */
enum SOCD
{
  Neutral,
  TwoIP,
  TwoIPNoReactivate,
  NormalKeyboard,
};

SOCD currentSOCD = TwoIPNoReactivate;


void checkModeChange(){
    if (buttonStatus[BUTTONSTART] && buttonStatus[BUTTONSELECT])
    {
      if (buttonStatus[BUTTONMY]){ //changesocd
          if (buttonStatus[BUTTONCDOWN])
            currentSOCD = TwoIP;
          else if (buttonStatus[BUTTONCLEFT])
            currentSOCD = Neutral;
          else if (buttonStatus[BUTTONCRIGHT])
            currentSOCD == TwoIPNoReactivate;
        }
       else{
        if (xinput) {
          if (buttonStatus[BUTTONCLEFT]){
            state = FGC_PC;
          }
          //else if (internalButtonStatus[BUTTONUP])
          //  state = KEYBOARD_PC;
         //else need_update = false;
        }
        else {
          if (buttonStatus[BUTTONCLEFT]){
            state = FGC_SWITCH;
          }
          else if (buttonStatus[BUTTONCDOWN]){
            state = SMASH_ULTIMATE_SWITCH;
          }
          //else need_update = false;
        }
       }
    }
}

void setupStartPins(){

    button13.attach(PIN_PLUS,INPUT_PULLUP);  // XBOX START
    button14.attach(PIN_MINUS,INPUT_PULLUP); // XBOX BACK
    button15.attach(PIN_HOME,INPUT_PULLUP);

    button13.interval(MILLIDEBOUNCE);
    button14.interval(MILLIDEBOUNCE);
    button15.interval(MILLIDEBOUNCE);
}

void setupPins(){
    button1.attach(PIN_DOWN,INPUT_PULLUP);
    button2.attach(PIN_UP,INPUT_PULLUP);
    button3.attach(PIN_LEFT,INPUT_PULLUP);
    button4.attach(PIN_RIGHT,INPUT_PULLUP);
    button5.attach(PIN_X,INPUT_PULLUP);      // XBOX B
    button6.attach(PIN_B,INPUT_PULLUP);      // XBOX A
    button7.attach(PIN_Y,INPUT_PULLUP);      // XBOX Y
    button8.attach(PIN_ZR,INPUT_PULLUP);     // XBOX X
    button9.attach(PIN_EXTRA,INPUT_PULLUP);     // XBOX LB
    button10.attach(PIN_ZL,INPUT_PULLUP);      // XBOX RB
    button11.attach(PIN_L,INPUT_PULLUP);     // XBOX LT
    button12.attach(PIN_R,INPUT_PULLUP);     // XBOX RT
    button13.attach(PIN_PLUS,INPUT_PULLUP);  // XBOX START
    button14.attach(PIN_MINUS,INPUT_PULLUP); // XBOX BACK
    button15.attach(PIN_HOME,INPUT_PULLUP);
    button16.attach(PIN_MX,INPUT_PULLUP);
    button17.attach(PIN_MY,INPUT_PULLUP);
    button18.attach(PIN_CUP,INPUT_PULLUP);
    button19.attach(PIN_CDOWN,INPUT_PULLUP);
    button20.attach(PIN_CLEFT,INPUT_PULLUP);
    button21.attach(PIN_CRIGHT,INPUT_PULLUP);
    button22.attach(PIN_A,INPUT_PULLUP);

    button1.interval(MILLIDEBOUNCE);
    button2.interval(MILLIDEBOUNCE);
    button3.interval(MILLIDEBOUNCE);
    button4.interval(MILLIDEBOUNCE);
    button5.interval(MILLIDEBOUNCE);      // XBOX B
    button6.interval(MILLIDEBOUNCE);     // XBOX A
    button7.interval(MILLIDEBOUNCE);      // XBOX Y
    button8.interval(MILLIDEBOUNCE);     // XBOX X
    button9.interval(MILLIDEBOUNCE);     // XBOX LB
    button10.interval(MILLIDEBOUNCE);      // XBOX RB
    button11.interval(MILLIDEBOUNCE);     // XBOX LT
    button12.interval(MILLIDEBOUNCE);    // XBOX RT
    button13.interval(MILLIDEBOUNCE);  // XBOX START
    button14.interval(MILLIDEBOUNCE); // XBOX BACK
    button15.interval(MILLIDEBOUNCE);
    button16.interval(MILLIDEBOUNCE);
    button17.interval(MILLIDEBOUNCE);
    button18.interval(MILLIDEBOUNCE);
    button19.interval(MILLIDEBOUNCE);
    button20.interval(MILLIDEBOUNCE);
    button21.interval(MILLIDEBOUNCE);
    button22.interval(MILLIDEBOUNCE);
}
void setup() {

  modeChanged = false;
  EEPROM.get(0, state);
  EEPROM.get(2, xinput);
  setupStartPins();
  delay(500);

#ifdef DISABLE_NSWITCH
#warning "NSWITCH mode is disabled, will act only as an XInput controller"
/* force Xinput */
  xinput = true;
#else
#ifdef DISABLE_XINPUT
#warning "XINPUT mode is disabled, will act only as a Nintendo switch controller"
/* force nswitch */
  xinput = false;
#else
/* set xinput mode according to held button */
// if select is held on boot, NSWitch mode
  int value = digitalRead(PIN_MINUS);
  if (value == LOW)
  {
    state = SMASH_ULTIMATE_SWITCH;
    currentSOCD = TwoIP;
    xinput = false;
    EEPROM.put(2, xinput);
  }
// if start is held on boot, XInput mode
  else {
    value = digitalRead(PIN_PLUS);
    if (value == LOW)
      {
        state = FGC_PC;
        currentSOCD = Neutral;
        xinput = true;
        EEPROM.put(2, xinput);
      }
   else {
    state = SMASH_ULTIMATE_SWITCH;
    currentSOCD = TwoIP;
    xinput = false;
    EEPROM.put(2, xinput);
   }
  }
  setupPins();
  
#endif
#endif  
  SetupHardware(xinput);
  GlobalInterruptEnable();
}


void loop() {
    currTime = millis();
    buttonRead();
    checkModeChange();
    if (state == SMASH_ULTIMATE_SWITCH){
       convert_smash_pads();
    }
    else{
      convert_dpad();
    }
    send_pad_state();
}
uint8_t fTwoIPNoReactivate(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH, bool& lockLOW, bool& lockHIGH){
  uint8_t control = 128;
  if (isLOW && isHIGH) {
    if (wasHIGH) {
      control = minValue;
      lockHIGH = true; }
    if (wasLOW) {
      control = maxValue;
      lockLOW = true; }
  }
  if (!isLOW && isHIGH && (lockHIGH == false)) {
    control = maxValue;
    wasHIGH = true;
    wasLOW = false;
    lockLOW = false;
  }
  if (isLOW && !isHIGH && (lockLOW == false)) {
    control = minValue;
    wasLOW = true;
    wasHIGH = false;
    lockHIGH = false;
  }
  if (!isLOW && !isHIGH) {
    wasHIGH = false;
    wasLOW = false;
    lockLOW = false;
    lockHIGH = false;
  }
  return control;
}

uint8_t fNeutral(bool isLOW, bool isHIGH){
  uint8_t control = 128;
  if (!isLOW && isHIGH)
    control = maxValue;
  if (isLOW && !isHIGH)
    control = minValue;
  return control;
}

uint8_t fTwoIP(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH){
  uint8_t control = 128;
  if (isLOW && wasHIGH)
    control = minValue;
  if (isHIGH && wasLOW)
    control = maxValue;
  if (!isLOW && isHIGH) {
    control = maxValue;
    wasHIGH = true;
    wasLOW = false; }
  if (isLOW && !isHIGH) {
    control = minValue;
    wasLOW = true;
    wasHIGH = false;
  }
  return control;
}

void convert_smash_pads(){

      uint8_t controlX = 128;
      uint8_t controlY = 128;
      uint8_t cstickX = 128;
      uint8_t cstickY = 128;
      uint8_t RLight = 0;
      uint8_t LLight = 0;

      int8_t positionX = 0;
      int8_t positionY = 0;
      int8_t positionCX = 0;
      int8_t positionCY = 0;

      bool HORIZONTAL = false;
      bool VERTICAL = false;
      bool DIAGONAL = false;

    /********* SOCD *********/
    if (currentSOCD == TwoIP){
      controlX = fTwoIP(internalButtonStatus[BUTTONLEFT], internalButtonStatus[BUTTONRIGHT], wasLEFT, wasRIGHT);
      controlY = fTwoIP(internalButtonStatus[BUTTONDOWN], internalButtonStatus[BUTTONUP], wasDOWN, wasUP);
      cstickX = fTwoIP(buttonStatus[BUTTONCLEFT], buttonStatus[BUTTONCRIGHT], wasCLEFT, wasCRIGHT);
      cstickY = fTwoIP(buttonStatus[BUTTONCDOWN], buttonStatus[BUTTONCUP], wasCDOWN, wasCUP);
    }
    if (currentSOCD == Neutral){
      controlX = fNeutral(internalButtonStatus[BUTTONLEFT], internalButtonStatus[BUTTONRIGHT]);
      controlY = fNeutral(internalButtonStatus[BUTTONDOWN], internalButtonStatus[BUTTONUP]);
      cstickX = fNeutral(buttonStatus[BUTTONCLEFT], buttonStatus[BUTTONCRIGHT]);
      cstickY = fNeutral(buttonStatus[BUTTONCDOWN], buttonStatus[BUTTONCUP]);
    }
    if (currentSOCD == TwoIPNoReactivate){
      controlX = fTwoIPNoReactivate(internalButtonStatus[BUTTONLEFT], internalButtonStatus[BUTTONRIGHT], wasLEFT, wasRIGHT, lockLEFT, lockRIGHT);
      controlY = fTwoIPNoReactivate(internalButtonStatus[BUTTONDOWN], internalButtonStatus[BUTTONUP], wasDOWN, wasUP, lockDOWN, lockUP);
      cstickX = fTwoIPNoReactivate(buttonStatus[BUTTONCLEFT], buttonStatus[BUTTONCRIGHT], wasCLEFT, wasCRIGHT, lockCLEFT, lockCRIGHT);
      cstickY = fTwoIPNoReactivate(buttonStatus[BUTTONCDOWN], buttonStatus[BUTTONCUP], wasCDOWN, wasCUP, lockCDOWN, lockCUP);
    }


    if (controlX != 128) {
      HORIZONTAL = true;
      if (controlX == minValue) positionX = -1;
      else positionX = 1;
    }
    if (controlY != 128) {
      VERTICAL = true;
      if (controlY == minValue) positionY = -1;
      else positionY = 1;
    }
    if (HORIZONTAL && VERTICAL) DIAGONAL = true;

    if (cstickX != 128) {
      if (cstickX == minValue) positionCX = -1;
      else positionCX = 1;
    }
    if (cstickY != 128) {
      if (cstickY == minValue) positionCY = -1;
      else positionCY = 1;
    }
    /********* Modifiers *********/
    if (buttonStatus[BUTTONMX]) {
      if (HORIZONTAL) {
        controlX = 128 + (positionX * 90); //102 max value 80
      }
      if (VERTICAL) {
        controlY = 128 + (positionY * 49);
      }

      if (buttonStatus[BUTTONB]) {
          controlX = 128 + (positionX * 80);//47 base too low
          controlY = 128 + (positionY * 80);//41 base too low
      }
      if (positionCX != 0){
        cstickX = 128 + (positionCX * 85);//65
        cstickY = 128 + (positionY * 40);//23
      }

      if (DIAGONAL) {
        controlX = 128 + (positionX * 60);//40
        controlY = 128 + (positionY * 40);//26
        if (buttonStatus[BUTTONB]) controlX = 128 + (positionX * 53);
        if (buttonStatus[BUTTONCUP]) {
          controlX = 128 + (positionX * 71);
          controlY = 128 + (positionY * 35);
        }
        if (buttonStatus[BUTTONCDOWN]) {
            controlX = 128 + (positionX * 61);
            controlY = 128 + (positionY * 49);
        }
        if (buttonStatus[BUTTONCLEFT]) {
            controlX = 128 + (positionX * 66);
            controlY = 128 + (positionY * 42);
        }

        if (buttonStatus[BUTTONCRIGHT]) {
            controlX = 128 + (positionX * 75);
            controlY = 128 + (positionY * 27);
        }
      }
    }

    if (buttonStatus[BUTTONMY]) {
      if (HORIZONTAL) {
        controlX = 128 + (positionX * 40);
      }
      if (VERTICAL) {
        controlY = 128 + (positionY * 51);
      }
      if (buttonStatus[BUTTONB]) {
          //controlX = 128 + (positionX * 41);
          //controlY = 128 + (positionY * 61);
          controlX = 128 + (positionX * 80);//47 base too low
          controlY = 128 + (positionY * 80);//41 base too low
      }

      if (DIAGONAL) {
          controlX = 128 + (positionX * 38);
          controlY = 128 + (positionY * 49);

        if (buttonStatus[BUTTONCUP]) {
            controlX = 128 + (positionX * 35);
            controlY = 128 + (positionY * 71);
        }

        if (buttonStatus[BUTTONCDOWN]) {
            controlX = 128 + (positionX * 49);
            controlY = 128 + (positionY * 61);
        }

        if (buttonStatus[BUTTONCLEFT]) {
            controlX = 128 + (positionX * 42);
            controlY = 128 + (positionY * 66);
        }

        if (buttonStatus[BUTTONCRIGHT]) {
            controlX = 128 + (positionX * 27);
            controlY = 128 + (positionY * 75);
        }
      }
    }

    if (buttonStatus[BUTTONLB]) {
      LLight = 140;
      if (HORIZONTAL) controlX = 128 + (positionX * 100);
      if (VERTICAL) controlY = 128 + (positionY * 100);
      if (buttonStatus[BUTTONMX]) {
          if (HORIZONTAL) {
              controlX = 128 + (positionX * 85);//51
          }
          if (VERTICAL) {
              controlY = 128 + (positionY * 85);
          }
          if (DIAGONAL){
              controlX = 128 + (positionX * 85);
              controlY = 128 + (positionY * 85);
          }
        }
        if (buttonStatus[BUTTONMY]) {
        if (HORIZONTAL) {
            controlX = 128 + (positionX * 85);
        }
        if (VERTICAL) {
            controlY = 128 + (positionY * 85);
        }
        if (DIAGONAL) {
          controlX = 128 + (positionX * 85);
          controlY = 128 + (positionY * 85);
          }
        }
     
    }

    if (buttonStatus[BUTTONRT]) {
      RLight = 140;
      if (buttonStatus[BUTTONMX]) {
        if (HORIZONTAL) {
            controlX = 128 + (positionX * 85);//51
        }
        if (VERTICAL) {
            controlY = 128 + (positionY * 85);//51
        }
        if (DIAGONAL) {
            controlX = 128 + (positionX * 85);
            controlY = 128 + (positionY * 85);
          }
       }
      if (buttonStatus[BUTTONMY]) {
        if (HORIZONTAL) {
            controlX = 128 + (positionX * 85);
        }
        if (VERTICAL) {
            controlY = 128 + (positionY * 85);
        }
        if (DIAGONAL) {
          controlX = 128 + (positionX * 85);
          controlY = 128 + (positionY * 85);
          }
        }
    }

  buttonStatus[AXISLX] = controlX;
  buttonStatus[AXISLY] = controlY;
  buttonStatus[AXISRX] = cstickX;
  buttonStatus[AXISRY] = cstickY;

  buttonStatus[BUTTONUP] = 0;
  buttonStatus[BUTTONDOWN] = 0;
  buttonStatus[BUTTONLEFT] = 0;
  buttonStatus[BUTTONRIGHT] = 0;

  /********* DPAD *********/
  if (buttonStatus[BUTTONMX] && buttonStatus[BUTTONMY]) {
    if (buttonStatus[BUTTONCUP]) buttonStatus[BUTTONUP] = buttonStatus[BUTTONCUP];
    if (buttonStatus[BUTTONCDOWN]) buttonStatus[BUTTONDOWN] = buttonStatus[BUTTONCDOWN];
    if (buttonStatus[BUTTONCLEFT]) buttonStatus[BUTTONLEFT] = buttonStatus[BUTTONCLEFT];
    if (buttonStatus[BUTTONCRIGHT]) buttonStatus[BUTTONRIGHT] = buttonStatus[BUTTONCRIGHT];
  }

}


void convert_dpad(){

      uint8_t controlX = 128;
      uint8_t controlY = 128;
      uint8_t cstickX = 128;
      uint8_t cstickY = 128;
      uint8_t RLight = 0;
      uint8_t LLight = 0;

      int8_t positionX = 0;
      int8_t positionY = 0;
      int8_t positionCX = 0;
      int8_t positionCY = 0;

      bool HORIZONTAL = false;
      bool VERTICAL = false;
      bool DIAGONAL = false;

    /********* SOCD *********/
    if (currentSOCD == TwoIP){
      controlX = fTwoIP(internalButtonStatus[BUTTONLEFT], internalButtonStatus[BUTTONRIGHT], wasLEFT, wasRIGHT);
      controlY = fTwoIP(internalButtonStatus[BUTTONDOWN], internalButtonStatus[BUTTONUP], wasDOWN, wasUP);
      cstickX = fTwoIP(buttonStatus[BUTTONCLEFT], buttonStatus[BUTTONCRIGHT], wasCLEFT, wasCRIGHT);
      cstickY = fTwoIP(buttonStatus[BUTTONCDOWN], buttonStatus[BUTTONCUP], wasCDOWN, wasCUP);
    }
    if (currentSOCD == Neutral){
      controlX = fNeutral(internalButtonStatus[BUTTONLEFT], internalButtonStatus[BUTTONRIGHT]);
      controlY = fNeutral(internalButtonStatus[BUTTONDOWN], internalButtonStatus[BUTTONUP]);
      cstickX = fNeutral(buttonStatus[BUTTONCLEFT], buttonStatus[BUTTONCRIGHT]);
      cstickY = fNeutral(buttonStatus[BUTTONCDOWN], buttonStatus[BUTTONCUP]);
    }
    if (currentSOCD == TwoIPNoReactivate){
      controlX = fTwoIPNoReactivate(internalButtonStatus[BUTTONLEFT], internalButtonStatus[BUTTONRIGHT], wasLEFT, wasRIGHT, lockLEFT, lockRIGHT);
      controlY = fTwoIPNoReactivate(internalButtonStatus[BUTTONDOWN], internalButtonStatus[BUTTONUP], wasDOWN, wasUP, lockDOWN, lockUP);
      cstickX = fTwoIPNoReactivate(buttonStatus[BUTTONCLEFT], buttonStatus[BUTTONCRIGHT], wasCLEFT, wasCRIGHT, lockCLEFT, lockCRIGHT);
      cstickY = fTwoIPNoReactivate(buttonStatus[BUTTONCDOWN], buttonStatus[BUTTONCUP], wasCDOWN, wasCUP, lockCDOWN, lockCUP);
    }

    buttonStatus[AXISLX] = controlX;
    buttonStatus[AXISLY] = controlY;
    buttonStatus[AXISRX] = cstickX;
    buttonStatus[AXISRY] = cstickY;
    buttonStatus[BUTTONUP] = 0;
    buttonStatus[BUTTONDOWN] = 0;
    buttonStatus[BUTTONLEFT] = 0;
    buttonStatus[BUTTONRIGHT] = 0;

    if (buttonStatus[BUTTONMY]) {
      if (buttonStatus[BUTTONCUP]) buttonStatus[BUTTONUP] = internalButtonStatus[BUTTONUP];
      if (buttonStatus[BUTTONCDOWN]) buttonStatus[BUTTONDOWN] = internalButtonStatus[BUTTONDOWN];
      if (buttonStatus[BUTTONCLEFT]) buttonStatus[BUTTONLEFT] = internalButtonStatus[BUTTONLEFT];
      if (buttonStatus[BUTTONCRIGHT]) buttonStatus[BUTTONRIGHT] = internalButtonStatus[BUTTONRIGHT];
  }
}

void buttonRead()
{
  if (state == SMASH_ULTIMATE_SWITCH){
    if (button2.update()) {internalButtonStatus[BUTTONDOWN] = button2.fell();}
    if (button1.update()) {internalButtonStatus[BUTTONUP] = button1.fell();}
    if (button3.update()) {internalButtonStatus[BUTTONLEFT] = button3.fell();}
    if (button4.update()) {internalButtonStatus[BUTTONRIGHT] = button4.fell();}
    if (button22.update()) {buttonStatus[BUTTONA] = button22.fell();}
    if (button6.update()) {buttonStatus[BUTTONB] = button6.fell();}
    if (button5.update()) {buttonStatus[BUTTONX] = button5.fell();}
    if (button7.update()) {buttonStatus[BUTTONY] = button7.fell();}
    if (button11.update()) {buttonStatus[BUTTONLB] = button11.fell();}
    if (button12.update()) {buttonStatus[BUTTONRB] = button12.fell();}
    if (button10.update()) {buttonStatus[BUTTONLT] = button10.fell();}
    if (button8.update()) {buttonStatus[BUTTONRT] = button8.fell();}
    if (button13.update()) {buttonStatus[BUTTONSTART] = button13.fell();}
    if (button14.update()) {buttonStatus[BUTTONSELECT] = button14.fell();}
    if (button15.update()) { buttonStatus[BUTTONHOME] = button15.fell();}
    if (button16.update()) { buttonStatus[BUTTONMX] = button16.fell();}
    if (button17.update()) { buttonStatus[BUTTONMY] = button17.fell();}
    if (button19.update()) { buttonStatus[BUTTONCUP] = button19.fell();}
    if (button18.update()) { buttonStatus[BUTTONCDOWN] = button18.fell();}
    if (button20.update()) { buttonStatus[BUTTONCLEFT] = button20.fell();}
    if (button21.update()) { buttonStatus[BUTTONCRIGHT] = button21.fell();}
  }
  else if ( state == FGC_PC || state == FGC_SWITCH ){
      if (button16.update()) {internalButtonStatus[BUTTONDOWN] = button16.fell();}
      if (button1.update()) {internalButtonStatus[BUTTONUP] = button1.fell();}
      if (button3.update()) {internalButtonStatus[BUTTONLEFT] = button3.fell();}
      if (button4.update()) {internalButtonStatus[BUTTONRIGHT] = button4.fell();}
      if (button5.update()) {buttonStatus[BUTTONA] = button5.fell();}
      if (button6.update()) {buttonStatus[BUTTONB] = button6.fell();}
      if (button7.update()) {buttonStatus[BUTTONX] = button7.fell();}
      if (button8.update()) {buttonStatus[BUTTONY] = button8.fell();}
      if (button9.update()) {buttonStatus[BUTTONLB] = button9.fell();}
      if (button10.update()) {buttonStatus[BUTTONRB] = button10.fell();}
      if (button2.update()) {buttonStatus[BUTTONLT] = button2.fell();}
      if (button12.update()) {buttonStatus[BUTTONRT] = button12.fell();}
      if (button13.update()) {buttonStatus[BUTTONSTART] = button13.fell();}
      if (button14.update()) {buttonStatus[BUTTONSELECT] = button14.fell();}
      if (button15.update()) { buttonStatus[BUTTONHOME] = button15.fell();}
      if (button17.update()) { buttonStatus[BUTTONMY] = button17.fell();}
      if (button11.update()) { buttonStatus[BUTTONMX] = button11.fell();}
      if (button19.update()) { buttonStatus[BUTTONCUP] = button19.fell();}
      if (button18.update()) { buttonStatus[BUTTONCDOWN] = button18.fell();}
      if (button20.update()) { buttonStatus[BUTTONCLEFT] = button20.fell();}
      if (button21.update()) { buttonStatus[BUTTONCRIGHT] = button21.fell();}

  }
  else if ( state == NORMAL_PC ){
      if (button16.update()) {internalButtonStatus[BUTTONDOWN] = button16.fell();}
      if (button1.update()) {internalButtonStatus[BUTTONUP] = button1.fell();}
      if (button3.update()) {internalButtonStatus[BUTTONLEFT] = button3.fell();}
      if (button4.update()) {internalButtonStatus[BUTTONRIGHT] = button4.fell();}
      if (button5.update()) {buttonStatus[BUTTONA] = button5.fell();}
      if (button6.update()) {buttonStatus[BUTTONB] = button6.fell();}
      if (button7.update()) {buttonStatus[BUTTONX] = button7.fell();}
      if (button8.update()) {buttonStatus[BUTTONY] = button8.fell();}
      if (button9.update()) {buttonStatus[BUTTONLB] = button9.fell();}
      if (button10.update()) {buttonStatus[BUTTONRB] = button10.fell();}
      if (button2.update()) {buttonStatus[BUTTONLT] = button2.fell();}
      if (button12.update()) {buttonStatus[BUTTONRT] = button12.fell();}
      if (button13.update()) {buttonStatus[BUTTONSTART] = button13.fell();}
      if (button14.update()) {buttonStatus[BUTTONSELECT] = button14.fell();}
      if (button15.update()) { buttonStatus[BUTTONHOME] = button15.fell();}
      if (button11.update()) { buttonStatus[BUTTONMX] = button11.fell();}
      if (button17.update()) { buttonStatus[BUTTONMY] = button17.fell();}
      if (button19.update()) { buttonStatus[BUTTONCUP] = button19.fell();}
      if (button18.update()) { buttonStatus[BUTTONCDOWN] = button18.fell();}
      if (button20.update()) { buttonStatus[BUTTONCLEFT] = button20.fell();}
      if (button21.update()) { buttonStatus[BUTTONCRIGHT] = button21.fell();}

  }
  

#ifdef HOME_HOTKEY  
  if(buttonStatus[BUTTONSTART] && buttonStatus[BUTTONSELECT]) {
   if (startAndSelTime == 0)
    startAndSelTime = millis();
   else if (currTime - startAndSelTime > HOME_DELAY)
   {
      buttonStatus[BUTTONHOME] = 1;
   }
 } else {
  startAndSelTime = 0;
  buttonStatus[BUTTONHOME] = 0;
 }
#endif
  
}
