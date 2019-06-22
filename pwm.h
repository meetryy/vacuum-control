#ifndef Pwm_h
#define Pwm_h

#include <stdint.h>

class PWM_class{ 
  public:
  uint8_t   Depth;
  uint8_t   ImpulseMs;
  uint8_t   ImpulseCounter;
  uint64_t  OldTime;

  int HeaterPins[9];
  bool HeatingOn = 0;
  
  int FanPin;
  bool FanOn = 0;
  uint64_t BeepEnd;
  int BeepPin;
  bool PowerLimitHalf = 0;
  bool Heaters[9] = {0};
  
  void Init(void);
  void Spin(void);
  void Beep(bool length);
  };
  
extern PWM_class PWMc;

#endif
