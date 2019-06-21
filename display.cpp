#include "arduino.h"
#include "display.h"
#include "menu.h"
#include "buttons.h"
#include "debug.h"
#include "cycle.h"
#include "pwm.h"
#include "stepper.h"
#include "pneumatic.h"

#include <string.h>
#include <stdbool.h>

#include <LiquidCrystal.h>
const int rs = 33, en = 27, d4 = 25, d5 = 23, d6 = 31, d7 = 29;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Display_class Display;

void Display_class::Init(void){
  OldRefreshTime = millis();
  RefreshMs = 1000 / LCD_FPS;
  ItemsPerPage = 4;
  Page = 0;
  Menu.Marker = 0;
  PendUpd = 1;
  OldPage = 255;
  SetMode(DISP_MAIN);
  SetState(ACT_READY);
  
  lcd.begin(20, 4);
  delay(100);
  lcd.setCursor(0, 0);
  lcd.noCursor();
  //lcd.write("hello!");
};

void Display_class::ShowSplash(void){
  lcd.setCursor(7, 0); lcd.write("Hello!");
  lcd.setCursor(2, 1); lcd.write("Petrov, Tregubov");
  lcd.setCursor(6, 2); lcd.write("Nikolaev");
  lcd.setCursor(2, 3); lcd.write("ORPP, 2018 - 2019");
  delay(1500);
}

void Display_class::SetMode(uint8_t mode){
  DispMode = mode;
  //lcd.clear();
  OldMode = 255;
  Cycle.OldLastAction = 255;
  PendUpd = 1;
}

void Display_class::SetState(uint8_t state){
  Cycle.LastAction = state;
  Display.PendUpd = 1;
}

void ShowPneumo(void){
  lcd.setCursor(11, 2); lcd.write("(");
  for (int i = 0; i < DIST_NR; i++){
    uint8_t pos = 19 - DIST_NR + i;
    lcd.setCursor(pos, 2);
    lcd.write((char)219+(255-219)*(Pneumo.GetState(i)==1));
  }
  lcd.setCursor(19, 2); lcd.write(")");
  /*
  for (int i = 0; i < DIST_NR; i++){
    //uint8_t pos = 20 - DIST_NR + i;
    lcd.setCursor(i, 2);
    lcd.write((char)32+(255-32)*(PWM.Heaters[i]==1));
  }
  */

}

void ShowAction(void){
  if (Cycle.LastAction != Cycle.OldLastAction){
    lcd.setCursor(0, 3);
    lcd.write("                    "); 
    
  
  
  lcd.setCursor(0, 3);
  switch (Cycle.LastAction){
    case ACT_FRAME_DOWN: {lcd.write("Frame down"); break;}
    case ACT_FRAME_UP: {lcd.write("Frame up"); break;}
    case ACT_TABLE_DOWN: {lcd.write("Table down"); break;}
    case ACT_TABLE_UP: {lcd.write("Table up"); break;}
    case ACT_HEATER_HOME: {lcd.write("Heater right");  break;}
    case ACT_HEATER_WORK: {lcd.write("Heater left");  break;}
    
    case ACT_HEATER_STOP: {lcd.write("Heater stop"); break;}
    case ACT_HEATING_ON: {lcd.write("Heater on"); break;}
    case ACT_HEATING_OFF: {lcd.write("Heater off"); break;}
    case ACT_BLOW: {lcd.write("Blow"); break;}
    case ACT_BLOW_OFF: {lcd.write("Blow stopped"); break;}
    case ACT_VACUUM: {lcd.write("Vacuuming..."); break;}
    case ACT_VACUUM_OFF: {lcd.write("Vacuuming stopped"); break;}
    case ACT_COOL_ON: {lcd.write("Fan on"); break;}
    case ACT_COOL_OFF: {lcd.write("Fan off"); break;}
    case ACT_PULL_RIGHT: {lcd.write("Pull"); break;}
    case ACT_PULL_LEFT: {lcd.write("Pull reverse");  break;}
    case ACT_PULL_STOP: {lcd.write("Pull stop"); break;}
    case ACT_ACC_CHG: {lcd.write("Accum chg"); break;}
    case ACT_ACC_CHG_OFF: {lcd.write("Accum chg stopped"); break;}
    case ACT_GETREADY: {lcd.write("Getting ready..."); break;}
    case ACT_READY: {lcd.write("Ready!"); break;}
    case ACT_SEMI_DONE: {lcd.write("Cycle done!"); break;}
    case ACT_STOP: {lcd.write("Stopped!"); break;}
    case ACT_ERR_FRAMEUP: {lcd.write("Heater blocks frame!"); break;}
    case ACT_DELAY_VAC: {lcd.write("Delay before vacuum"); break;}
    case ACT_DELAY_PULL: {lcd.write("Delay before pulling"); break;}
    case ACT_VAC_COOL: {lcd.write("Vacuum+cooling"); break;}
    }
  Cycle.OldLastAction = Cycle.LastAction;
  }
}

void Display_class::Refresh(void){
  if ((PendUpd) && (millis() > OldRefreshTime + RefreshMs)){
    if (DispMode != OldMode){     
        OldPage = 255;
        OldMode = DispMode;
        Cycle.OldMode = 255;
        Cycle.OldLastAction = 255;
        lcd.clear();
      }

    if (DispMode == DISP_MAIN){
      //mode
      if (Cycle.Mode != Cycle.OldMode){  
        lcd.setCursor(0, 0);
        lcd.write("                    "); 
        lcd.setCursor(0, 0);
        switch (Cycle.Mode){
          case MODE_AUTO: { lcd.write("M:AUTO"); break;}
          case MODE_SEMI: {lcd.write("M:SEMI AUTO");break;}
          case MODE_MANUAL: {lcd.write("M:MANUAL");break;}
        }
        Cycle.OldMode = Cycle.Mode;
      }

      //if (HeatFanUpdated){
        lcd.setCursor(0, 1);
        if (PWM.HeatingOn) lcd.write("Heater: ON");
        else lcd.write("Heater:OFF");
        
        lcd.setCursor(13, 1);
        if (PWM.FanOn) lcd.write("Fan: ON");
        else lcd.write("Fan:OFF");
        //HeatFanUpdated =0;
      //}

        if (((Cycle.Mode==MODE_AUTO)||(Cycle.Mode==MODE_SEMI))&&CycTimeUpdated){
          char CycTime[7];
          char str_temp[7];
          dtostrf(((float)Cycle.LastCycleTime/(float)1000.0), 5, 1, str_temp);
          sprintf(CycTime, "%ss",str_temp);
          uint8_t NumberLen = (uint8_t)strlen(CycTime);
          lcd.setCursor(20 - NumberLen, 0);
          lcd.write(CycTime);

          if (Cycle.Mode==MODE_AUTO){
            sprintf(str_temp, "(%u)",Cycle.CycCounter);
            lcd.setCursor(7, 0);
            lcd.write(str_temp);
          }
          
          CycTimeUpdated = 0;
        }  

        
         
      ShowAction();
      ShowPneumo();
    }
      
    else {
      if (DispMode == DISP_MENU) Display.Page = Menu.Marker / Display.ItemsPerPage;     
      if (DispMode == DISP_EXT) Display.Page = Menu.ExtMarker / Display.ItemsPerPage;     
      uint8_t FirstItem = Page * ItemsPerPage;
      uint8_t LastItem = FirstItem + ItemsPerPage;
      uint8_t OnThisScreen = 0;

      if (Page != OldPage){lcd.clear(); OldPage = Page;}
      
      for(int i = FirstItem; i < LastItem; i++){
        if (((DispMode == DISP_MENU)&&(i >= M_NR))||((DispMode == DISP_EXT)&&(i >= EXT_NR))) break;
    
        // marker
        lcd.setCursor(0, OnThisScreen);
        uint8_t Marker = (DispMode == DISP_MENU)*Menu.Marker + (DispMode == DISP_EXT)*Menu.ExtMarker;
    
          if   ((Marker == i)&&(Buttons.EncModeBlinker)) {lcd.write((char)126);} 
          else lcd.write(" ");
      
        //if (((DispMode == DISP_Ext)&&(Menu.ExtMarker == i)||
         //   (DispMode == DISP_MENU)&&(Menu.Marker == i))&&(Buttons.EncModeBlinker)) {lcd.write((char)126);} 
            
        //name
        char CurrentRow[20];
        if (DispMode == DISP_MENU) strcpy(CurrentRow, Menu.Items[i].Name);
        if (DispMode == DISP_EXT) strcpy(CurrentRow, Menu.Ext[i].Name);
        
        lcd.setCursor(1, OnThisScreen);
        lcd.write(CurrentRow);
        
        // number and unit
        char CurrentNumber[7];
        bool IsBoolean = (DispMode == DISP_MENU)*Menu.Items[i].IsBool + (DispMode == DISP_EXT)*Menu.Ext[i].IsBool;
        
        if (IsBoolean){
          if (DispMode == DISP_MENU){
            if (Menu.Items[i].Value > 0.0) sprintf(CurrentNumber, " ON");
            else {sprintf(CurrentNumber, "OFF");}
          }
          if (DispMode == DISP_EXT){
            if (Menu.Ext[i].Value > 0.0) sprintf(CurrentNumber, " ON");
            else {sprintf(CurrentNumber, "OFF");}
          }
        }
        else{
          // number values
          char str_temp[7];
          if (DispMode == DISP_MENU){
            dtostrf(Menu.Items[i].Value, 5, 1, str_temp);
          sprintf(CurrentNumber, "%s",str_temp);
          }
          if (DispMode == DISP_EXT){
            dtostrf(Menu.Ext[i].Value, 5, 1, str_temp);
          sprintf(CurrentNumber, "%s",str_temp);
          }
          
        }
        uint8_t NumberLen = (uint8_t)strlen(CurrentNumber);
        lcd.setCursor(20 - NumberLen, OnThisScreen);
        lcd.write(CurrentNumber);
  
        OnThisScreen++;
      }
    }
  
    PendUpd = 0;
    OldRefreshTime = millis();
  }
};

void Display_class::Scroll(int result){
  if (Display.DispMode==DISP_MENU){
        if (result == DIR_CW){
        int8_t NewMarker = Menu.Marker + 1;
        //while (Menu.Items[NewMarker].Extended) NewMarker++;
        if (NewMarker < M_NR) {Menu.Marker = NewMarker;}
        else {Menu.Marker = 0; PWM.Beep(0);}
      }   
      if (result == DIR_CCW){
        int8_t NewMarker = Menu.Marker - 1;
        //while (Menu.Items[NewMarker].Extended) NewMarker--;
        if (NewMarker > -1) {Menu.Marker = NewMarker;}
        else {Menu.Marker = M_NR-1; PWM.Beep(0);}
      }    
      Display.Page = Menu.Marker / Display.ItemsPerPage;
      //Display.PendUpd = 1; 
      }

      if (Display.DispMode==DISP_EXT){
        if (result == DIR_CW){
        int8_t NewMarker = Menu.ExtMarker + 1;
        //while (Menu.Items[NewMarker].Extended) NewMarker++;
        if (NewMarker < EXT_NR) {Menu.ExtMarker = NewMarker;}
        else {Menu.ExtMarker = 0; PWM.Beep(0);}
        }   
        if (result == DIR_CCW){
          int8_t NewMarker = Menu.ExtMarker - 1;
          //while (Menu.Items[NewMarker].Extended) NewMarker--;
          if (NewMarker > -1) {Menu.ExtMarker = NewMarker;}
          else {Menu.ExtMarker = EXT_NR-1; PWM.Beep(0);}
        }    
      Display.Page = Menu.ExtMarker / Display.ItemsPerPage;     
      }
      Display.PendUpd = 1; 
}

void Display_class::Tune(int result){
  if (Display.DispMode==DISP_MENU){
        if (Menu.Items[Menu.Marker].Changeable){
          if (result == DIR_CW){
            if (Menu.Items[Menu.Marker].Value < Menu.Items[Menu.Marker].Max)
              Menu.Items[Menu.Marker].Value += Menu.Items[Menu.Marker].Increment;
            else {Menu.Items[Menu.Marker].Value = Menu.Items[Menu.Marker].Min; PWM.Beep(0);}    
          }
        
        if (result == DIR_CCW){
          if (Menu.Items[Menu.Marker].Value > Menu.Items[Menu.Marker].Min)
            Menu.Items[Menu.Marker].Value -= Menu.Items[Menu.Marker].Increment;
          else {Menu.Items[Menu.Marker].Value = Menu.Items[Menu.Marker].Max; PWM.Beep(0);}
        }
        Display.PendUpd = 1; 
        }
      }
      
      if (Display.DispMode==DISP_EXT){
        if (Menu.Ext[Menu.ExtMarker].Changeable){
        if (result == DIR_CW){
          if (  Menu.Ext[Menu.ExtMarker].Value < Menu.Ext[Menu.ExtMarker].Max)
                Menu.Ext[Menu.ExtMarker].Value +=Menu.Ext[Menu.ExtMarker].Increment;
          else {Menu.Ext[Menu.ExtMarker].Value = Menu.Ext[Menu.ExtMarker].Min; PWM.Beep(0);}    
        }
        
        if (result == DIR_CCW){
          if (  Menu.Ext[Menu.ExtMarker].Value > Menu.Ext[Menu.ExtMarker].Min)
                Menu.Ext[Menu.ExtMarker].Value -=Menu.Ext[Menu.ExtMarker].Increment;
          else {Menu.Ext[Menu.ExtMarker].Value = Menu.Ext[Menu.ExtMarker].Max; PWM.Beep(0);}
        }
        Display.PendUpd = 1; 
      }
      }
      
      
}

void Display_class::SwitchModes(void){
  uint8_t NewMode = Display.DispMode + 1;
  if (NewMode < DISP_NR) Display.SetMode(NewMode);
  else Display.SetMode(0);
}
