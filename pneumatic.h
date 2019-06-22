#ifndef Pneumo_h
#define Pneumo_h

#include <stdint.h>
#include "pins.h"

enum Distributors{DIST_MAIN, DIST_TABLE, DIST_FRAME, VALVE_ACC, DIST_BLOW, VALVE_VACUUM, VALVE_BLOW, DIST_NR};
enum DistStates {DOWN, UP};
enum ValveStates {CLOSED, OPEN};
enum DistState {VACUUM, BLOW};

class Pneumo_class{ 
public:
  const int PneumoPins[8] = {PIN_DIST_MAIN, PIN_DIST_TABLE, PIN_DIST_FRAME, PIN_DIST_BLOW, PIN_VALVE_BLOW,  PIN_VALVE_VACUUM,  PIN_VALVE_ACC}; 
  int PneumoState[8];
  int PneumoDefaultState[8];
  bool Inverted[8];
  bool Update = 1;
  
  void Init(void);
  void Set(uint8_t Dist, uint8_t State);
  void Charge(void);
  void VacuumTable(void);
  void BlowTable(void);
  void BlowOff(void);
  void Flip(uint8_t Dist);
  void Power(bool state);
  void Run(void);
  uint8_t GetState(uint8_t dist);
  };
  
extern Pneumo_class Pneumo;

#endif
