#ifndef Buttons_h
#define Buttons_h

#include "rotary.h"
#include "pins.h"

#include <stdint.h>
#define LONGPRESS_MS      400
#define COLUMNS           4
#define ROWS              4
#define NUMBER_OF_BUTTONS COLUMNS*ROWS

enum  ButtonAlias { B_HEATER_LEFT,  B_HEATER_RIGHT, B_FRAME,  B_COOLING,
                    B_PULLER_LEFT,  B_PULLER_RIGHT, B_TABLE,  B_HEATING,
                    B_VACUUM,       B_BLOW,         B_CHARGE,   B_MENU,
                    B_MODE, B_START,        B_STOP};

enum  ReadModes {CAPTURE, COMPARE};

class Button_class{  
public:
  typedef struct {
    bool     RawState;
    bool     State;
    bool     OldState;
    bool     Unlocked;
    bool     HasLongPress;
    bool     LongPress;  
    bool     WasPressed;  
    bool     WasReleased;  
    bool     ProcessingPending;
    uint16_t  StartTimer;
    uint16_t  EndTimer;
  } ButtonStruct_t;
  
  ButtonStruct_t Button[NUMBER_OF_BUTTONS];
  enum  ButtonStateAlias {RELEASED, PRESSED};
  enum EncModes {SCROLL, TUNE};
  
  int EncBtnPin = ENCB_PIN;
  bool EncBtnState;
  bool EncBtnOldState;
  bool Matrix_Scan_Allowed;
  bool EncMode;
  bool EncModeBlinker;
  uint64_t EncModeBlinkerTime;
  uint64_t ModeSwitchTime;
  bool  ReadMode = 0;
  uint32_t NextCapTime = 0;
  
  const int ColPins[COLUMNS] = {COL0_PIN, COL1_PIN, COL2_PIN, COL3_PIN};
  const int RowPins[ROWS] = {ROW0_PIN, ROW1_PIN, ROW2_PIN, ROW3_PIN};
   
  void Init(void);
  void Scan(void);
  void Proc(void);
  void ProcEncoder(void);
  
  };
  
  extern Button_class Buttons;

#endif
