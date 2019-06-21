#ifndef Menu_h
#define Menu_h

#include <stdbool.h>
#include <stdint.h>

enum Items{ M_HEATER_POWER_GLOBAL, M_HEATER_PERM, M_HEATER_TIME, 
            M_HEATER0_POWER, M_HEATER1_POWER, M_HEATER2_POWER, 
            M_HEATER3_POWER, M_HEATER4_POWER, M_HEATER5_POWER,
            M_HEATER6_POWER, M_HEATER7_POWER, M_HEATER8_POWER,
            M_COOL_ON,  M_COOL_POWER, M_COOL_TIME,
            M_HEATER_TRAVEL, //M_HEATER_STEP, M_HEATER_SPEED, M_HEATER_ACCEL,
            M_PULLER_ON, M_PULLER_DISTANCE, //M_PULLER_STEP, M_PULLER_SPEED, M_PULLER_ACCEL,
            M_BLOW_ON, M_BLOW_TIME,
            M_VACUUM_TIME, M_ACCUM_ON, M_ACCUM_CHARGE_TIME,
            M_VAC_COOL, M_VAC_COOL_DELAY,
            M_DELAY_BEFORE_VACUUM, M_DELAY_BEFORE_PULLING,
            //M_INV_BLOW, M_INV_TABLE, M_INV_FRAME, M_INV_ACC, M_INV_VAC, M_INV_VBLOW,     
            //M_MODE_AUTO, M_MODE_SEMI, M_MODE_MANUAL,
            M_EEP_SAVE, M_EEP_LOAD,
            M_NR};
            
  enum ExtItems{
      EXT_HEATER_STEP, EXT_HEATER_SPEED, EXT_HEATER_ACCEL, 
      EXT_PULLER_STEP, EXT_PULLER_SPEED, EXT_PULLER_ACCEL,
      EXT_INV_BLOW, EXT_INV_TABLE, EXT_INV_FRAME, EXT_INV_ACC, EXT_INV_VAC, EXT_INV_VBLOW, EXT_INV_MAIN,
      EXT_SAFE_POS, EXT_PNEUM_TIME, //EXT_HEATER_POWERLIMIT, 
      EXT_NR
  };
  
class Menu_class{  
public:
  typedef struct MenuItem_t{
  float Value, Min, Max, Increment;
  char* Name, Unit;
  bool IsBool;
  bool Changeable;
  bool Extended;
};

  struct MenuItem_t Items[M_NR];
  struct MenuItem_t Ext[EXT_NR];
  
  uint8_t Marker;
  uint8_t ExtMarker;
  
  void Init(void);
  void LoadDefaults(void);
  
};

extern Menu_class Menu;
#endif
