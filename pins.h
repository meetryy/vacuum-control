#ifndef Pins_h
#define Pins_h

#define MEGA
  
  #ifdef BLUEPILL
  #include "arduino.h"
  
  #include "board.h"
  
  #define PULLER_STEP_PIN PA7
  #define PULLER_DIR_PIN  PA6
  
  #define HEATER_STEP_PIN PA5
  #define HEATER_DIR_PIN  PA4
  
  #define BEEP_PIN        PA0
  
  #define COL0_PIN        28
  #define COL1_PIN        26
  #define COL2_PIN        24
  #define COL3_PIN        22
  #define ROW0_PIN        30
  #define ROW1_PIN        32
  #define ROW2_PIN        34
  #define ROW3_PIN        36
  
  #define ENC0_PIN        37
  #define ENC1_PIN        35
  #define ENCB_PIN        39
  
  #define PIN_LCD_RS      33
  #define PIN_LCD_EN      27
  #define PIN_LCD_D4      25
  #define PIN_LCD_D5      23
  #define PIN_LCD_D6      31
  #define PIN_LCD_D7      29
  
  #define  PIN_DIST_MAIN     PA15
  #define  PIN_DIST_TABLE    PA14
  #define  PIN_DIST_FRAME    PA13
  #define  PIN_DIST_BLOW     PA12
  #define  PIN_VALVE_BLOW    PA11
  #define  PIN_VALVE_VACUUM  PA10
  #define  PIN_VALVE_ACC     PA9
  #endif

  
  
  #ifdef MEGA
  #include "arduino.h"
  
  #define PULLER_STEP_PIN A7
  #define PULLER_DIR_PIN  A6
  
  #define HEATER_STEP_PIN A5
  #define HEATER_DIR_PIN  A4
  
  #define BEEP_PIN        A0
  
  #define COL0_PIN        28
  #define COL1_PIN        26
  #define COL2_PIN        24
  #define COL3_PIN        22
  #define ROW0_PIN        30
  #define ROW1_PIN        32
  #define ROW2_PIN        34
  #define ROW3_PIN        36
  
  #define ENC0_PIN        37
  #define ENC1_PIN        35
  #define ENCB_PIN        39
  
  
  #define PIN_LCD_RS      33
  #define PIN_LCD_EN      27
  #define PIN_LCD_D4      25
  #define PIN_LCD_D5      23
  #define PIN_LCD_D6      31
  #define PIN_LCD_D7      29
  
  #define  PIN_DIST_MAIN     A15
  #define  PIN_DIST_TABLE    A14
  #define  PIN_DIST_FRAME    A13
  #define  PIN_DIST_BLOW     A12
  #define  PIN_VALVE_BLOW    A11
  #define  PIN_VALVE_VACUUM  A10
  #define  PIN_VALVE_ACC     A9
  #endif



#endif
