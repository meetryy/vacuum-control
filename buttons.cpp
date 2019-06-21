#include "buttons.h"
#include "arduino.h"
#include "rotary.h"
#include "menu.h"
#include "display.h"
#include "debug.h"
#include "pwm.h"
#include "cycle.h"
#include "pneumatic.h"
#include "stepper.h"

Button_class Buttons;

Rotary encoder = Rotary(37, 35);

void Button_class::Init(void){
  
  for (int i =0; i<NUMBER_OF_BUTTONS;i++){
    Button[i].Unlocked = 1;
    Button[i].HasLongPress = 1;
  }

  for (int i = 0; i<COLUMNS;i++) {pinMode(ColPins[i], OUTPUT);}
  for (int i = 0; i<ROWS;i++) {pinMode(RowPins[i], INPUT);}

  ColPins[0] = 28; ColPins[1] = 26; ColPins[2] = 24; ColPins[3] = 22;
  RowPins[0] = 30; RowPins[1] = 32; RowPins[2] = 34; RowPins[3] = 36;
  
  // encoder
  EncMode = SCROLL;
  EncBtnPin = 39;
  EncModeBlinkerTime = millis();
  pinMode(EncBtnPin, INPUT_PULLUP);       
  encoder.begin();
  EncModeBlinker = 1;
};

  
void Button_class::Scan(void){
  for (int col = 0; col < COLUMNS; col++){
    for (int i=0; i<COLUMNS; i++)digitalWrite(ColPins[i], (col == i));
      for (int row = 0; row < ROWS; row++){ 
        for (int i=0; i<ROWS; i++)Button[i*ROWS + col].State = digitalRead(RowPins[i]); 
        
       }
  //delay(1);
  }

  
  for (uint8_t i = 0; i<NUMBER_OF_BUTTONS; i++){
    if (Button[i].Unlocked){
      Button[i].LongPress = 0;
      /*
      if ((Button[i].State == PRESSED)&&(Button[i].OldState == RELEASED)){
        Debug.Println("PRESSED");
        
        Button[i].StartTimer = millis();
        Button[i].OldState = PRESSED;
        break;
      } 
      */  

      // pressed
      if ((Button[i].OldState == RELEASED)&&(Button[i].State == PRESSED)){
        //Debug.Println("PRESSED");
        Button[i].StartTimer = millis();
        Button[i].OldState = PRESSED;
        Button[i].WasPressed = 1;
        Button[i].ProcessingPending = 1;
        break;
      }
      
      if  ((Button[i].State == PRESSED)&&(Button[i].OldState == PRESSED)&&(Button[i].ProcessingPending == 0)){
        Button[i].EndTimer = millis();
        //Debug.Println("STILL PRESSEED");
        if ((Button[i].EndTimer - Button[i].StartTimer) >= LONGPRESS_MS){
          if (Button[i].HasLongPress) Button[i].LongPress = 1;
          else Button[i].LongPress = 0;
          Button[i].WasPressed = 1;
          Button[i].ProcessingPending = 1;
          Button[i].OldState = PRESSED;
          break;
        }    
      }
      
      if  ((Button[i].OldState == PRESSED)&&(Button[i].State == RELEASED)){
        Button[i].EndTimer = millis();
        Button[i].WasPressed = 0;
        Button[i].WasReleased = 1;
        //Debug.Println("RELEASED");
        if ((Button[i].EndTimer-Button[i].StartTimer) >= LONGPRESS_MS){
          if (Button[i].HasLongPress) Button[i].LongPress = 1; 
          else Button[i].LongPress = 0;
        }
        else{Button[i].LongPress = 0;}
        Button[i].ProcessingPending = 1;
        Button[i].OldState = RELEASED;
        break;
      }
    }
  }
  Matrix_Scan_Allowed = 0;
  };

void Button_class::Proc(void){
  for (uint8_t i = 0; i<NUMBER_OF_BUTTONS; i++){
    if ((Button[i].Unlocked==1)&&(Button[i].ProcessingPending == 1)){
        // ======= короткие нажатия ========
        if (Button[i].WasReleased){
          switch (i){
          case B_VACUUM: {
              if(Cycle.Mode == MODE_MANUAL){
                 Pneumo.BlowOff();
                Display.SetState(ACT_VACUUM_OFF);
              }
              break; }
           case B_BLOW: {
              if(Cycle.Mode == MODE_MANUAL){
                Pneumo.BlowOff();
                Display.SetState(ACT_BLOW_OFF);
              }
              break; }

          case B_CHARGE: {
          if(Cycle.Mode == MODE_MANUAL){
            Pneumo.BlowOff();
            Display.SetState(ACT_ACC_CHG_OFF);
          }
          break; }

          case B_PULLER_LEFT: {
            Motor.Stop(MOTOR_PULLER);/* blocking move heater to work*/         
            Display.SetState(ACT_PULL_STOP); 
            break;}
            
          case B_PULLER_RIGHT: {
            Motor.Stop(MOTOR_PULLER);/* blocking move heater to work*/    
            Display.SetState(ACT_PULL_STOP);
            break;}

          }
          Button[i].WasReleased=0;
        }
        
        if ((!Button[i].LongPress)&&(Button[i].WasPressed)){
         // Serial.println("short" + String(i));
          switch (i){
            case B_HEATER_LEFT: {Display.SetState(ACT_HEATER_WORK); Motor.GoTo(MOTOR_HEATER, WORK); break;}
            case B_HEATER_RIGHT: {Display.SetState(ACT_HEATER_HOME); Motor.GoTo(MOTOR_HEATER, HOME); break;}
            case B_FRAME: {
              if(Cycle.Mode == MODE_MANUAL){
                Serial.println(Motor.Position(MOTOR_HEATER));
                if(Pneumo.GetState(DIST_FRAME) == DOWN){
                  if((Motor.Position(MOTOR_HEATER) <= Menu.Ext[EXT_SAFE_POS].Value)){
                    Pneumo.Flip(DIST_FRAME);
                    if (Pneumo.GetState(DIST_FRAME) == UP) Display.SetState(ACT_FRAME_UP);
                    else Display.SetState(ACT_FRAME_DOWN);
                  }
                  else Display.SetState(ACT_ERR_FRAMEUP);   
                }
                else{
                  Pneumo.Flip(DIST_FRAME);
                  if (Pneumo.GetState(DIST_FRAME) == UP) Display.SetState(ACT_FRAME_UP);
                  else Display.SetState(ACT_FRAME_DOWN);
                }
                
              }
              break; }
            case B_COOLING: {
              PWM.FanOn = !PWM.FanOn;
              if (PWM.FanOn) Display.SetState(ACT_COOL_ON);
              else Display.SetState(ACT_COOL_OFF);
              break; }
              
            case B_PULLER_LEFT: {
              Motor.Move(MOTOR_PULLER, 100000);/* blocking move heater to work*/    
              //Motor.MoveDone(MOTOR_PULLER)'
              Display.SetState(ACT_PULL_LEFT); 
              //Motor.Move(MOTOR_PULLER, -10); 
              break;}
            case B_PULLER_RIGHT: {
              Motor.Move(MOTOR_PULLER, -100000);/* blocking move heater to work*/    
              Display.SetState(ACT_PULL_RIGHT); 
              //Motor.Move(MOTOR_PULLER, 10);
              break;}
            case B_TABLE: {
              if(Cycle.Mode == MODE_MANUAL){
                Pneumo.Flip(DIST_TABLE);
                if (Pneumo.GetState(DIST_TABLE) == UP) Display.SetState(ACT_TABLE_UP);
                else Display.SetState(ACT_TABLE_DOWN);
              }
              break; }
            case B_HEATING: {
              PWM.HeatingOn = !PWM.HeatingOn;
              if (PWM.HeatingOn) Display.SetState(ACT_HEATING_ON);
              else Display.SetState(ACT_HEATING_OFF);
              break; }

              case B_CHARGE:{
              if(Cycle.Mode == MODE_MANUAL){
                Pneumo.Charge();;
                Display.SetState(ACT_ACC_CHG);
              }
              break;}
            case B_VACUUM: {
              if(Cycle.Mode == MODE_MANUAL){
                Pneumo.VacuumTable();;
                Display.SetState(ACT_VACUUM);
              }
              break; }
            case B_BLOW: {
              if(Cycle.Mode == MODE_MANUAL){
                Pneumo.BlowTable();;
                Display.SetState(ACT_BLOW);
              }
              break; }
            
            case B_MENU: {
              if (Buttons.EncMode != TUNE){Display.SwitchModes();}
              }

              case B_MODE: {
                //Serial.println("we here");
                if ((Cycle.Mode==MODE_MANUAL)||(Cycle.Stage==C_DONE)) {
                Cycle.SpinModes();
              }
              break;}
              
              
            case B_START: {
              if ((Cycle.Mode == MODE_AUTO) || (Cycle.Mode == MODE_SEMI)) Cycle.Start();
              //Serial.println("we here");
              break; }
              
            case B_STOP: {Cycle.Stop(); break; }
            
            case 15: {break; }
            }

            Button[i].WasPressed = 0;           
        }

        if ((Button[i].LongPress)&&(Button[i].WasPressed)){
        // ======= длинные нажатия ======== 
         //Serial.println("long" + String(i));
          switch (i){
            
            case B_HEATER_LEFT: {break;}
            case B_HEATER_RIGHT: {break;}
            case B_FRAME: {break; }
            case B_COOLING: {break; }
            case B_PULLER_LEFT: {
              //Display.SetState(ACT_PULL_LEFT); 
              //Motor.Move(MOTOR_PULLER, -20); 
              break;}
            case B_PULLER_RIGHT: {
             // Display.SetState(ACT_PULL_RIGHT); 
              //Motor.Move(MOTOR_PULLER, 20);
              break;}
            case B_TABLE:{break; }
            case B_HEATING:{break;}
            case B_CHARGE:{
              if(Cycle.Mode == MODE_MANUAL){
                Pneumo.Charge();;
                Display.SetState(ACT_ACC_CHG);
              }
              break;}
            case B_VACUUM: {
              if(Cycle.Mode == MODE_MANUAL){
                Pneumo.VacuumTable();;
                Display.SetState(ACT_VACUUM);
              }
              break; }
            case B_BLOW: {
              if(Cycle.Mode == MODE_MANUAL){
                Pneumo.BlowTable();;
                Display.SetState(ACT_BLOW);
              }
              break; }
            case B_MODE: {
             }
            case B_MENU: {break;}
            case B_START: {break; }
            case B_STOP: {break; }
       
            case 15: {break; }
            }
             Button[i].WasPressed = 0;
        }
      
      
      Button[i].ProcessingPending = 0;
    }
  }  
};

void Button_class::ProcEncoder(void){
  // encoder button
  EncBtnState = digitalRead(EncBtnPin);
  if ((EncBtnState == 0) && (EncBtnOldState == 1)){
    EncMode = !EncMode;
    //Debug.Println("EncMode changed");
    EncModeBlinker = 1;
    Display.PendUpd=1;
  }
  EncBtnOldState = EncBtnState;

   // blinker
  if (EncMode == SCROLL) {
    EncModeBlinker = 1;
    }
  if (EncMode == TUNE){
    if (millis() > (EncModeBlinkerTime + 400)) {
      EncModeBlinker = !EncModeBlinker; 
      Display.PendUpd = 1;
      EncModeBlinkerTime = millis();}
  }
  
  uint8_t result = encoder.process();
  if (result) {
    if (EncMode == SCROLL){Display.Scroll(result); }
    if (EncMode == TUNE){Display.Tune(result); }
    
  }
}

 
