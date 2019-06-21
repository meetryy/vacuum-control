#include "menu.h"
#include "memory.h"
#include "arduino.h"
#include "buttons.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Menu_class Menu;

void allocStr (char **pStr, char* name) {
    free (*pStr);
    *pStr = malloc(((unsigned)strlen(name) + 1));
    strcpy(*pStr, name);
}

void Menu_class::LoadDefaults(void){
  Items[M_HEATER0_POWER].Value = 10.0;
  Items[M_HEATER1_POWER].Value = 20.0;
  Items[M_HEATER2_POWER].Value = 30.0;
  Items[M_HEATER3_POWER].Value = 40.0;
  Items[M_HEATER4_POWER].Value = 50.0;
  Items[M_HEATER5_POWER].Value = 60.0;
  Items[M_HEATER6_POWER].Value = 70.0;
  Items[M_HEATER7_POWER].Value = 80.0;
  Items[M_HEATER8_POWER].Value = 90.0;
  Items[M_HEATER_POWER_GLOBAL].Value = 100.0;
  Items[M_HEATER_PERM].Value = 1.0;
  
  Items[M_HEATER_TRAVEL].Value = 500.0;
  Items[M_HEATER_TIME].Value = 5.0;
  Items[M_COOL_ON].Value = 1.0;
  Items[M_COOL_POWER].Value = 100.0;
  Items[M_COOL_TIME].Value = 5.0;
  Items[M_PULLER_ON].Value = 1.0;
  Items[M_PULLER_DISTANCE].Value = 200.0;
  Items[M_BLOW_ON].Value = 1.0;
  Items[M_BLOW_TIME].Value = 5.0;
  Items[M_VACUUM_TIME].Value = 5.0;
  Items[M_ACCUM_ON].Value = 1.0;
  Items[M_ACCUM_CHARGE_TIME].Value = 5.0;
  Items[M_DELAY_BEFORE_VACUUM].Value = 2.0;
  Items[M_DELAY_BEFORE_PULLING].Value = 2.0;
  Items[M_EEP_SAVE].Value = 0.0;
  Items[M_EEP_LOAD].Value = 0.0;
  Items[M_VAC_COOL].Value = 1.0;
  Items[M_VAC_COOL_DELAY].Value = 1.0;
  Ext[EXT_HEATER_STEP].Value = 1.0;
  Ext[EXT_HEATER_SPEED].Value = 300.0;
  Ext[EXT_HEATER_ACCEL].Value = 400.0;
  Ext[EXT_PULLER_STEP].Value = 1.0;
  Ext[EXT_PULLER_SPEED].Value = 300.0;
  Ext[EXT_PULLER_ACCEL].Value = 1000.0;
  Ext[EXT_INV_BLOW].Value = 0.0;
  Ext[EXT_INV_TABLE].Value = 0.0;
  Ext[EXT_INV_FRAME].Value = 0.0;
  Ext[EXT_INV_ACC].Value = 0.0;
  Ext[EXT_INV_VAC].Value = 0.0;
  Ext[EXT_INV_VBLOW].Value = 0.0;
  Ext[EXT_INV_MAIN].Value = 0.0;
  Ext[EXT_SAFE_POS].Value = 500.0;
  Ext[EXT_PNEUM_TIME].Value = 1.0;
 // Ext[EXT_HEATER_POWERLIMIT].Value = 1.0;
  
}


void Menu_class::Init(void){
  
  uint8_t i = M_HEATER0_POWER;
  allocStr(&Items[i].Name, "Heater 1 power");
  Items[i].IsBool = 0;  Items[i].Increment = 1.0; Items[i].Max = 100.0; Items[i].Min = 0.0;
  Items[i].Unit = "%%"; Items[i].Changeable = 1; Items[i].Extended = 0;

  i = M_HEATER1_POWER;
  allocStr(&Items[i].Name, "Heater 2 power");
  Items[i].IsBool = 0;  Items[i].Increment = 1.0; Items[i].Max = 100.0;Items[i].Min = 0.0;
  Items[i].Unit = "%%";Items[i].Changeable = 1;Items[i].Extended = 0;
  
  i = M_HEATER2_POWER;
  allocStr(&Items[i].Name, "Heater 3 power");
  Items[i].IsBool = 0;Items[i].Increment = 1.0;Items[i].Max = 100.0;Items[i].Min = 0.0;Items[i].Unit = "%%";
  Items[i].Changeable = 1;Items[i].Extended = 0;

  i = M_HEATER3_POWER;
  allocStr(&Items[i].Name, "Heater 4 power");
  Items[i].IsBool = 0;Items[i].Increment = 1.0;Items[i].Max = 100.0;Items[i].Min = 0.0;
  Items[i].Unit = "%%";Items[i].Changeable = 1;Items[i].Extended = 0;

  i = M_HEATER4_POWER;
  allocStr(&Items[i].Name, "Heater 5 power");
  Items[i].IsBool = 0;Items[i].Increment = 1.0;
  Items[i].Max = 100.0;Items[i].Min = 0.0;Items[i].Unit = "%%";Items[i].Changeable = 1;Items[i].Extended = 0;

  i = M_HEATER5_POWER;
  allocStr(&Items[i].Name, "Heater 6 power");
  Items[i].IsBool = 0; Items[i].Increment = 1.0; Items[i].Max = 100.0;
  Items[i].Min = 0.0; Items[i].Unit = "%%"; Items[i].Changeable = 1; Items[i].Extended = 0;

  i = M_HEATER6_POWER;
  allocStr(&Items[i].Name, "Heater 7 power");
  Items[i].IsBool = 0;   Items[i].Increment = 1.0; Items[i].Max = 100.0;
  Items[i].Min = 0.0;  Items[i].Unit = "%%";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_HEATER7_POWER;
  allocStr(&Items[i].Name, "Heater 8 power");
  Items[i].IsBool = 0;   Items[i].Increment = 1.0;  Items[i].Max = 100.0;
  Items[i].Min = 0.0;  Items[i].Unit = "%%";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_HEATER8_POWER;
  allocStr(&Items[i].Name, "Heater 9 power");
  Items[i].IsBool = 0; Items[i].Increment = 1.0;  Items[i].Max = 100.0;
  Items[i].Min = 0.0;  Items[i].Unit = "%%";  Items[i].Changeable = 1;  Items[i].Extended = 0;
  
  i = M_HEATER_POWER_GLOBAL;
  allocStr(&Items[i].Name, "Heater scale");
  Items[i].IsBool = 0;  Items[i].Increment = 1.0;  Items[i].Max = 100.0;
  Items[i].Min = 0.0;  Items[i].Unit = "%%\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;
  
  i = M_HEATER_PERM;
  allocStr(&Items[i].Name, "Heater is perm");
  Items[i].IsBool = 1;   Items[i].Increment = 1.0;  Items[i].Max = 1.0;
  Items[i].Min = 0.0;  Items[i].Unit = "%%";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_COOL_ON;
  allocStr(&Items[i].Name, "Cooling");
  Items[i].IsBool = 1;   Items[i].Increment = 1.0;  Items[i].Max = 1.0;
  Items[i].Min = 0.0;  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_COOL_POWER;
  allocStr(&Items[i].Name, "Cooling power");
  Items[i].IsBool = 0;   Items[i].Increment = 1.0;  Items[i].Max = 100.0;
  Items[i].Min = 0.0;  Items[i].Unit = "%%\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_COOL_TIME;
  allocStr(&Items[i].Name, "Cooling time");
  Items[i].IsBool = 0;   Items[i].Increment = 0.1;  Items[i].Max = 300.0;
  Items[i].Min = 0.0;  Items[i].Unit = "sec\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_HEATER_TIME;
  allocStr(&Items[i].Name, "Heating time");
  Items[i].IsBool = 0;  Items[i].Increment = 0.1;  Items[i].Max = 300.0;
  Items[i].Min = 0.0;  Items[i].Unit = "sec\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;
  
  i = M_HEATER_TRAVEL;
  allocStr(&Items[i].Name, "Heater travel");
  Items[i].IsBool = 0; Items[i].Increment = 1.0;  Items[i].Max = 5000.0;
  Items[i].Min = 0.0;  Items[i].Unit = "s\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;
/*
  i = M_HEATER_STEP;
  allocStr(&Items[i].Name, "Heater micro");
  Items[i].IsBool = 0;  Items[i].Value = 1.0;  Items[i].Increment = 1.0;  Items[i].Max = 32.0;
  Items[i].Min = 1.0;  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 1;
  
  i = M_HEATER_SPEED;
  allocStr(&Items[i].Name,"Heater speed");
  Items[i].IsBool = 0;  Items[i].Value = 100.0;  Items[i].Increment = 1.0;  Items[i].Max = 1000.0;
  Items[i].Min = 1.0;  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_HEATER_ACCEL;
  allocStr(&Items[i].Name,"Heater acc");
  Items[i].IsBool = 0;  Items[i].Value = 40.0;  Items[i].Increment = 1.0;  Items[i].Max = 300.0;
  Items[i].Min = 1.0;  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 1;
*/
  i = M_PULLER_ON;
  allocStr(&Items[i].Name,"Puller");
  Items[i].IsBool = 1;    Items[i].Increment = 1.0;  Items[i].Max = 1;
  Items[i].Min = 0;  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_PULLER_DISTANCE;
  allocStr(&Items[i].Name,"Puller travel");
  Items[i].IsBool = 0;   Items[i].Increment = 1.0;  Items[i].Max = 5000.0;
  Items[i].Min = 0.0;  Items[i].Unit = "s\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;
/*
  i = M_PULLER_STEP;
  allocStr(&Items[i].Name,"Puller micro");
  Items[i].IsBool = 0;  Items[i].Value = 1.0;  Items[i].Increment = 1.0;  Items[i].Max = 32.0;
  Items[i].Min = 1.0;  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 1;
  
  i = M_PULLER_SPEED;
  allocStr(&Items[i].Name,"Puller speed");
  Items[i].IsBool = 0;  Items[i].Value = 100.0;  Items[i].Increment = 1.0;  Items[i].Max = 1000.0;  Items[i].Min = 1.0;
  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;
  
  i = M_PULLER_ACCEL;
  allocStr(&Items[i].Name,"Puller accel");
  Items[i].IsBool = 0;  Items[i].Value = 40.0;  Items[i].Increment = 1.0;  Items[i].Max = 300.0;
  Items[i].Min = 1.0;  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 1;
*/
  i = M_BLOW_ON;
  allocStr(&Items[i].Name,"Blow");
  Items[i].IsBool = 1;   Items[i].Increment = 1.0;  Items[i].Max = 1.0;
  Items[i].Min = 0.0;  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_BLOW_TIME;
  allocStr(&Items[i].Name,"Blow time");
  Items[i].IsBool = 0;   Items[i].Increment = 0.1;  Items[i].Max = 30.0;
  Items[i].Min = 0.0;  Items[i].Unit = "sec\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  
  i = M_VACUUM_TIME;
  allocStr(&Items[i].Name,"Vacuum time");
  Items[i].IsBool = 0;    Items[i].Increment = 0.1;  Items[i].Max = 10.0;
  Items[i].Min = 0.0;  Items[i].Unit = "sec\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_ACCUM_ON;
  allocStr(&Items[i].Name,"Use accum");
  Items[i].IsBool = 1;   Items[i].Increment = 1.0;  Items[i].Max = 1.0;
  Items[i].Min = 0.0;  Items[i].Unit = "\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_ACCUM_CHARGE_TIME;
  allocStr(&Items[i].Name,"Accum chg time");
  Items[i].IsBool = 0;    Items[i].Increment = 0.1;  Items[i].Max = 30.0;
  Items[i].Min = 0.0;  Items[i].Unit = "sec\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;
  
  i = M_DELAY_BEFORE_VACUUM;
  allocStr(&Items[i].Name,"Delay vacuum");
  Items[i].IsBool = 0;  Items[i].Increment = 0.1;  Items[i].Max = 30.0;
  Items[i].Min = 0.0;  Items[i].Unit = "sec\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;

  i = M_DELAY_BEFORE_PULLING;
  allocStr(&Items[i].Name,"Delay end");
  Items[i].IsBool = 0;   Items[i].Increment = 0.1;  Items[i].Max = 30.0;
  Items[i].Min = 0.0;  Items[i].Unit = "sec\0";  Items[i].Changeable = 1;  Items[i].Extended = 0;
  
  i = M_EEP_SAVE;
  allocStr(&Items[i].Name,"Save settings");
  Items[i].IsBool = 1;  Items[i].Value = 0.0;  Items[i].Increment = 1.0;  Items[i].Max = 1.0;
  Items[i].Min = 0.0;  Items[i].Changeable = 1;  Items[i].Extended = 1;  
  
  i = M_EEP_LOAD; 
  allocStr(&Items[i].Name,"Load settings");
  Items[i].IsBool = 1;  Items[i].Value = 0.0;  Items[i].Increment =1.0;  Items[i].Max = 1.0;
  Items[i].Min = 0.0;  Items[i].Changeable = 1;  Items[i].Extended = 1;

  i = M_VAC_COOL; 
  allocStr(&Items[i].Name,"Vacuum + cooling");
  Items[i].IsBool = 1;  Items[i].Increment =1;  Items[i].Max = 1;
  Items[i].Min = 0.0;  Items[i].Changeable = 1;  Items[i].Extended = 1;

  i = M_VAC_COOL_DELAY; 
  allocStr(&Items[i].Name,"V+C cool delay");
  Items[i].IsBool = 0;   Items[i].Increment =0.1;  Items[i].Max = 10.0;
  Items[i].Min = 0.0;  Items[i].Changeable = 1;  Items[i].Extended = 1;
  
  
/*
  i = M_INV_BLOW; 
  allocStr(&Items[i].Name,"Inv blow dist");
  Items[i].IsBool = 1;  Items[i].Value = 0.0;  Items[i].Increment =1.0;  Items[i].Max = 1.0;  Items[i].Min = 0.0;  Items[i].Changeable = 1;
  
  i = M_INV_TABLE; 
  allocStr(&Items[i].Name,"Inv table dist");
  Items[i].IsBool = 1;  Items[i].Value = 0.0;  Items[i].Increment =1.0;  Items[i].Max = 1.0;  Items[i].Min = 0.0;  Items[i].Changeable = 1;
  
   i = M_INV_FRAME; 
  allocStr(&Items[i].Name,"Inv frame dist");
  Items[i].IsBool = 1;  Items[i].Value = 0.0;  Items[i].Increment =1.0;  Items[i].Max = 1.0;  Items[i].Min = 0.0;  Items[i].Changeable = 1;

  i = M_INV_ACC; 
  allocStr(&Items[i].Name,"Inv acc valve");
  Items[i].IsBool = 1;  Items[i].Value = 0.0;  Items[i].Increment =1.0;  Items[i].Max = 1.0;  Items[i].Min = 0.0;  Items[i].Changeable = 1;

  i = M_INV_VAC; 
  allocStr(&Items[i].Name,"Inv vaccum valve");
  Items[i].IsBool = 1;  Items[i].Value = 0.0;  Items[i].Increment =1.0;  Items[i].Max = 1.0;  Items[i].Min = 0.0;  Items[i].Changeable = 1;
  
   i = M_INV_VBLOW; 
  allocStr(&Items[i].Name,"Inv blow valve");
  Items[i].IsBool = 1;  Items[i].Value = 0.0;  Items[i].Increment =1.0;  Items[i].Max = 1.0;  Items[i].Min = 0.0;  Items[i].Changeable = 1;
  
  i = M_NR;
  Items[i].Value = Memory.CheckNumber;
/*
  EXT_HEATER_STEP, EXT_HEATER_SPEED, EXT_HEATER_ACCEL, 
      EXT_PULLER_STEP, EXT_PULLER_SPEED, EXT_PULLER_ACCEL,
      EXT_INV_BLOW, EXT_INV_TABLE, EXT_INV_FRAME, EXT_INV_ACC, EXT_INV_VAC, EXT_INV_VBLOW,
      EXT_NR
  */
  // ===============================================   i = EXT_HEATER_TRAVEL;

  i = EXT_HEATER_STEP;
  allocStr(&Ext[i].Name, "Heater micro");
  Ext[i].IsBool = 0;   Ext[i].Increment = 1.0;  Ext[i].Max = 32.0;
  Ext[i].Min = 1.0;  Ext[i].Unit = "\0";  Ext[i].Changeable = 1;  Ext[i].Extended = 1;
  
  i = EXT_HEATER_SPEED;
  allocStr(&Ext[i].Name,"Heater speed");
  Ext[i].IsBool = 0;  Ext[i].Increment = 1.0;  Ext[i].Max = 1000.0;
  Ext[i].Min = 1.0;  Ext[i].Unit = "\0";  Ext[i].Changeable = 1;  Ext[i].Extended = 0;

  i = EXT_HEATER_ACCEL;
  allocStr(&Ext[i].Name,"Heater acc");
  Ext[i].IsBool = 0; Ext[i].Increment = 1.0;  Ext[i].Max = 3000.0;
  Ext[i].Min = 1.0;  Ext[i].Unit = "\0";  Ext[i].Changeable = 1;  Ext[i].Extended = 1;
i = EXT_PULLER_STEP;
  allocStr(&Ext[i].Name,"Puller micro");
  Ext[i].IsBool = 0;   Ext[i].Increment = 1.0;  Ext[i].Max = 32.0;
  Ext[i].Min = 1.0;  Ext[i].Unit = "\0";  Ext[i].Changeable = 1;  Ext[i].Extended = 1;
  
  i = EXT_PULLER_SPEED;
  allocStr(&Ext[i].Name,"Puller speed");
  Ext[i].IsBool = 0;  Ext[i].Increment = 1.0;  Ext[i].Max = 1000.0;  Ext[i].Min = 1.0;
  Ext[i].Unit = "\0";  Ext[i].Changeable = 1;  Ext[i].Extended = 0;
  
  i = EXT_PULLER_ACCEL;
  allocStr(&Ext[i].Name,"Puller accel");
  Ext[i].IsBool = 0;  Ext[i].Increment = 1.0;  Ext[i].Max = 3000.0;
  Ext[i].Min = 1.0;  Ext[i].Unit = "\0";  Ext[i].Changeable = 1;  Ext[i].Extended = 1;
 i = EXT_INV_BLOW; 
  allocStr(&Ext[i].Name,"Inv blow dist");
  Ext[i].IsBool = 1;  Ext[i].Increment =1.0;  Ext[i].Max = 1.0;  Ext[i].Min = 0.0;  Ext[i].Changeable = 1;
  
  i = EXT_INV_TABLE; 
  allocStr(&Ext[i].Name,"Inv table dist");
  Ext[i].IsBool = 1;   Ext[i].Increment =1.0;  Ext[i].Max = 1.0;  Ext[i].Min = 0.0;  Ext[i].Changeable = 1;
  
   i = EXT_INV_FRAME; 
  allocStr(&Ext[i].Name,"Inv frame dist");
  Ext[i].IsBool = 1;   Ext[i].Increment =1.0;  Ext[i].Max = 1.0;  Ext[i].Min = 0.0;  Ext[i].Changeable = 1;

  i = EXT_INV_ACC; 
  allocStr(&Ext[i].Name,"Inv acc valve");
  Ext[i].IsBool = 1;   Ext[i].Increment =1.0;  Ext[i].Max = 1.0;  Ext[i].Min = 0.0;  Ext[i].Changeable = 1;

  i = EXT_INV_VAC; 
  allocStr(&Ext[i].Name,"Inv vaccum valve");
  Ext[i].IsBool = 1;  Ext[i].Increment =1.0;  Ext[i].Max = 1.0;  Ext[i].Min = 0.0;  Ext[i].Changeable = 1;

  
  i = EXT_INV_VBLOW; 
  allocStr(&Ext[i].Name,"Inv blow valve");
  Ext[i].IsBool = 1;   Ext[i].Increment =1.0;  Ext[i].Max = 1.0;  Ext[i].Min = 0.0;  Ext[i].Changeable = 1;
  
  i = EXT_SAFE_POS; 
  allocStr(&Ext[i].Name,"Min heater pos");
  Ext[i].IsBool = 0;  Ext[i].Increment =10.0;  Ext[i].Max = 10000.0;  Ext[i].Min = 0.0;  Ext[i].Changeable = 1;

  i = EXT_PNEUM_TIME; 
  allocStr(&Ext[i].Name,"Pneum trig time");
  Ext[i].IsBool = 0;  Ext[i].Increment =0.1;  Ext[i].Max = 20.0;  Ext[i].Min = 0.0;  Ext[i].Changeable = 1;

  i = EXT_INV_MAIN; 
  allocStr(&Ext[i].Name,"Inv main dist");
  Ext[i].IsBool = 1;   Ext[i].Increment =1.0;  Ext[i].Max = 1.0;  Ext[i].Min = 0.0;  Ext[i].Changeable = 1;
  
  if (!Buttons.EncBtnState) LoadDefaults();
};
