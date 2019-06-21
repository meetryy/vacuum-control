#include "pneumatic.h"
#include "menu.h"
#include "arduino.h"

Pneumo_class Pneumo;

void Pneumo_class::Init(void){
  PneumoPins[DIST_MAIN] = A15;  
  PneumoPins[DIST_TABLE] = A14;  
  PneumoPins[DIST_FRAME] = A13;
  PneumoPins[DIST_BLOW] = A12; 
  PneumoPins[VALVE_BLOW] = A11;
  PneumoPins[VALVE_VACUUM] = A10;  
  PneumoPins[VALVE_ACC] = A9;  
  for (int i=0; i<DIST_NR; i++) pinMode(PneumoPins[i], OUTPUT); 
  
  Pneumo.Set(DIST_TABLE, DOWN);
  Pneumo.Set(DIST_FRAME, DOWN);
  Set(VALVE_ACC,    CLOSED);
  Set(VALVE_VACUUM, CLOSED);
  Set(VALVE_BLOW,   CLOSED);
};

uint8_t Pneumo_class::GetState(uint8_t dist){
  bool State = PneumoState[dist];
  if (Menu.Ext[EXT_INV_BLOW + dist].Value>0.0) State = !State;
  return State;
}

void Pneumo_class::Set(uint8_t Dist, uint8_t State){
  bool NewState = State;
  if (Menu.Ext[EXT_INV_BLOW+Dist].Value > 0.0) NewState = !NewState;
  PneumoState[Dist] = NewState;
  for (int i=0; i<DIST_NR; i++) digitalWrite(PneumoPins[i], PneumoState[i]); 
}

void Pneumo_class::Flip(uint8_t Dist){
  PneumoState[Dist] = (!PneumoState[Dist] & 0x01);
  for (int i=0; i<DIST_NR; i++) digitalWrite(PneumoPins[i], PneumoState[i]); 
}

void Pneumo_class::Power(bool state){
  Set(DIST_MAIN, state * OPEN);
  
}

void Pneumo_class::VacuumTable(void){
  Set(DIST_BLOW,    VACUUM);
  Set(VALVE_BLOW,   CLOSED);
  Set(VALVE_VACUUM, OPEN);
  if (Menu.Items[M_ACCUM_ON].Value >0.0) Set(VALVE_ACC, OPEN); 
}

void Pneumo_class::Charge(void){
  Set(DIST_BLOW,    VACUUM);
  Set(VALVE_BLOW,   CLOSED);
  Set(VALVE_ACC,    OPEN);
  Set(VALVE_VACUUM, CLOSED);
}

void Pneumo_class::BlowTable(void){
  Set(DIST_BLOW,    BLOW);
  Set(VALVE_BLOW,   OPEN);
  Set(VALVE_ACC,    CLOSED);
  Set(VALVE_VACUUM, CLOSED);
}

void Pneumo_class::BlowOff(void){
  Set(DIST_BLOW,    BLOW);
  Set(VALVE_BLOW,   CLOSED);
  Set(VALVE_ACC,    CLOSED);
  Set(VALVE_VACUUM, CLOSED);
}
