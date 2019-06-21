#ifndef Pneumo_h
#define Pneumo_h

#include <stdint.h>

enum Distributors{DIST_MAIN, DIST_TABLE, DIST_FRAME, VALVE_ACC, DIST_BLOW, VALVE_VACUUM, VALVE_BLOW, DIST_NR};
enum DistStates {DOWN, UP};
enum ValveStates {CLOSED, OPEN};
enum DistState {VACUUM, BLOW};

class Pneumo_class{ 
public:
  int PneumoPins[8];
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
