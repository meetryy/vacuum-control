#include "debug.h"
#include "memory.h"
#include "menu.h"
#include "arduino.h"
#include "display.h"
#include "PWM.h"

#include <EEPROM.h>  
#include <stdio.h>  

Memory_class Memory;
              
void Memory_class::Init(void){
  EEPstate = EEP_READY;
  OldStateTime = millis();
  //CheckNumber = 1.2345f;
};

void Memory_class::Worker(void){
  if ((Count) && (millis() > OldStateTime + 2000)){
    if ((EEPstate == EEP_FAIL)||(EEPstate == EEP_OK)){
      EEPstate = EEP_READY;
    }
    Count = 0;
  } 
  
  if ((Menu.Items[M_EEP_LOAD].Value > 0.0)&&(EEPstate == EEP_READY)) {
    Load(); 
    Menu.Items[M_EEP_LOAD].Value = 0.0;
  }
 
  if ((Menu.Items[M_EEP_SAVE].Value > 0.0)&&(EEPstate == EEP_READY)) {
    Save(); 
    Menu.Items[M_EEP_SAVE].Value = 0.0;}
  
}

void Memory_class::Save(void){
  EEPstate = EEP_SAVING;
  float LightMenu[M_NR+EXT_NR+1];
  EEPaddr = 0;
  
  // filling struct
  for (int i=0; i < M_NR; i++){
    LightMenu[i] = Menu.Items[i].Value;
  }
  LightMenu[M_NR] = CheckNumber;
  
  for (int i=0; i< EXT_NR; i++){
    LightMenu[M_NR+i+1] = Menu.Ext[i].Value;
  }
  
  // writing
  for (int i = 0; i < M_NR + EXT_NR + 1; i++) {
    EEPROM.put(EEPaddr, LightMenu[i]); 
    EEPaddr += sizeof(LightMenu); 
    //Serial.println("writing @ " + String(EEPaddr) + " = " + String(LightMenu[i]));
  }
  
  // verifying
    EEPaddr = (M_NR)*sizeof(LightMenu);// + sizeof(LightMenu); ;
    float ReadCheckNumber = 0.0;
    EEPROM.get(EEPaddr, ReadCheckNumber);
    //Serial.println("ReadCheckNumber = "+String(ReadCheckNumber));
   
  if (ReadCheckNumber != CheckNumber) {
    Count = 1;
    OldStateTime = millis();
    EEPstate = EEP_FAIL;
    PWM.Beep(1);
    //sprintf(Menu.Items[M_EEP_SAVE].Name, "Saving fail!");
  }
  else {
    Count = 1; 
    OldStateTime = millis();
    EEPstate = EEP_OK; 
    PWM.Beep(0);

    //sprintf(Menu.Items[M_EEP_SAVE].Name, "Saving OK");
    //Serial.println("EEP save ok");
    }
};


void Memory_class::Load(void){
  EEPstate = EEP_LOADING;
  EEPaddr = 0;
  float LightMenu[M_NR+EXT_NR+1]; 
  
  // reading
  for (int i = 0; i < M_NR+EXT_NR+1; i++) {
    EEPROM.get(EEPaddr, LightMenu[i]);    
    //Serial.println("read addr: " + String(EEPaddr));
    //Serial.println((LightMenu[i].Value));
    EEPaddr += sizeof(LightMenu); 
    //Serial.print("reading @ "); 
    //Serial.print(EEPaddr);
    //Serial.print(" = ");
    //Serial.println(LightMenu[i]);
  }
  // checking
  float ReadCheckNumber = 0.0;
  EEPaddr = (M_NR)*sizeof(LightMenu);// + sizeof(LightMenu); ;
  EEPROM.get(EEPaddr, ReadCheckNumber);
  
  
  if (ReadCheckNumber != CheckNumber) {
    Count = 1;
    OldStateTime = millis();
    EEPstate = EEP_FAIL;
    PWM.Beep(1);
    //Serial.println("check failed!!!");
  }
  
  if (EEPstate != EEP_FAIL){
    //Serial.println("check ok!!!");
    Count = 1;
    OldStateTime = millis();
 
    for (int i = 0; i < M_NR; i++) {Menu.Items[i].Value = LightMenu[i];}
    for (int i=0; i < EXT_NR; i++) {Menu.Ext[i].Value = LightMenu[M_NR + i +1];
      //Serial.println("reading extmenu @ " + String(i) + " = " + String(Menu.Ext[i].Value));
    }
    
    PWM.Beep(0);
    EEPstate = EEP_OK;
  }
}
 
