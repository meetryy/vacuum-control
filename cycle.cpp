#include "cycle.h"
#include "arduino.h"
#include "rotary.h"
#include "menu.h"
#include "display.h"
#include "pneumatic.h"
#include "pwm.h"
#include "debug.h"
#include "stepper.h"

Cycle_class Cycle;

uint64_t SoftDelayTime;

void Cycle_class::Init(void){
  Mode = MODE_MANUAL;
  LastCycleTime = 0;
  Stage = C_DONE;
  for(int i = 0; i<STAGE_NR; i++){
    StartTime[C_START] = 0;
  }
  SemiDone = 1;
  SoftDelayTime = millis();
};


void Cycle_class::SpinModes(void){
  if (Mode < MODE_NR-1) Mode++;
  else {Mode = 0;}
  Display.PendUpd = 1; 
  CycRun = 0;
}

void Cycle_class::Run(void){
  if (((Mode == MODE_AUTO)||(Mode == MODE_SEMI))&&CycRun){
    switch (Stage){
      case C_START: {
        StartTime[C_START] = millis(); 
        if (Mode == MODE_SEMI) {SemiDone = 0;}
        StartTime[C_FRAME_DOWN] = millis(); 
        Stage = C_FRAME_DOWN; 
        break;}
        
      case C_FRAME_DOWN: {
        Pneumo.Set(DIST_FRAME, DOWN); 
        Display.SetState(ACT_FRAME_DOWN);
        //delay(1000); /*some delay here*/
        if (CheckTime(StartTime[C_FRAME_DOWN], (uint64_t(Menu.Ext[EXT_PNEUM_TIME].Value * 1000.0)))){
          Stage = C_HEATER_WORK; 
        }
        break;}
        
      case C_HEATER_WORK: {
        Display.SetState(ACT_HEATER_WORK);
        //if (Motor.Done(MOTOR_HEATER) == 0)
       // if (!Motor.GoToActivated) {
          Motor.GoTo(MOTOR_HEATER, WORK);
        //  }
        
        if (Motor.Done(MOTOR_HEATER) == 0) {
          Motor.Stop(MOTOR_HEATER); 
          StartTime[C_HEATING] = millis(); 
          if (Menu.Items[M_ACCUM_ON].Value > 0.0){
            HeatingEndTime = (uint64_t(Menu.Items[M_HEATER_TIME].Value * 1000.0) + StartTime[C_HEATING]);
            ChgBeginTime = HeatingEndTime - (uint64_t(Menu.Items[M_ACCUM_CHARGE_TIME].Value * 1000.0));
          }
          Stage = C_HEATING;}
        break;}
        
      case C_HEATING: {
        PWMc.HeatingOn = 1;
        if (!AutoChg) Display.SetState(ACT_HEATING_ON);
        
        if (Menu.Items[M_ACCUM_ON].Value > 0.0){
          if (!AutoChg&&(millis()>=ChgBeginTime)){
            AutoChg = 1;
            Pneumo.Charge();
            Display.SetState(ACT_ACC_CHG);
          }
        }
        
        if (CheckTime(StartTime[C_HEATING], (uint64_t(Menu.Items[M_HEATER_TIME].Value * 1000.0)))){
          if (Menu.Items[M_HEATER_PERM].Value < 1.0) PWMc.HeatingOn = 0;
          if (Menu.Items[M_ACCUM_ON].Value > 0.0)  {Pneumo.BlowOff(); AutoChg = 0;}
          Stage = C_HEATER_HOME;
        }

        
        break;}
        
      case C_HEATER_HOME: {
        //Serial.println("C_HEATER_HOME ");
        Display.SetState(ACT_HEATER_HOME);
        Motor.GoTo(MOTOR_HEATER, HOME);/* blocking move heater to work*/ 
        if (Motor.Done(MOTOR_HEATER) == 0) {
          Motor.Stop(MOTOR_HEATER); 
          if(Menu.Items[M_HEATER_PERM].Value<1.0){PWMc.HeatingOn = 1;} 
          StartTime[C_BLOW] = millis(); 
          Stage = C_BLOW;}  
        break;}
        
      case C_BLOW: {
        //Serial.println("C_BLOW ");
        if (Menu.Items[M_BLOW_ON].Value>0.0){
          Pneumo.BlowTable();
          Display.SetState(ACT_BLOW);
          if (CheckTime(StartTime[C_BLOW], (uint64_t(Menu.Items[M_BLOW_TIME].Value * 1000.0)))){
            Pneumo.BlowOff();
            StartTime[C_TABLE_UP] = millis(); 
            Stage = C_TABLE_UP;
          }
        }
        else {StartTime[C_TABLE_UP] = millis(); Stage = C_TABLE_UP;}
        break;}
        
      case C_TABLE_UP: {
        //Serial.println("C_TABLE_UP ");
        Pneumo.Set(DIST_TABLE, UP); 
        Display.SetState(ACT_TABLE_UP);
        if (CheckTime(StartTime[C_TABLE_UP], (uint64_t(Menu.Ext[EXT_PNEUM_TIME].Value * 1000.0)))){
          Stage = C_DELAY_VAC; 
        }
        break;}

      case C_DELAY_VAC: {
        //Serial.println("C_VACUUM");
        Display.SetState(ACT_DELAY_VAC);
        if (!DelayRun) {
          // delay start
          DelayRun = 1; 
          StartTime[C_DELAY_VAC] = millis();
          }
   
        else{
          if (CheckTime(StartTime[C_DELAY_VAC], (uint64_t(Menu.Items[M_DELAY_BEFORE_VACUUM].Value * 1000.0)))){
            // delay done
            DelayRun = 0;
            if (Menu.Items[M_VAC_COOL].Value > 0.0){
              StartTime[C_COOLING] = millis() + (uint64_t(Menu.Items[M_VAC_COOL_DELAY].Value * 1000.0));
            }
            StartTime[C_VACUUM] = millis();
            Stage = C_VACUUM; 
          }
        }
        break;}
        
      case C_VACUUM: {
        if (Menu.Items[M_VAC_COOL].Value > 0.0){
          // vac + cool simult
          // vacuum
          if (!VacEnd) {Pneumo.VacuumTable();}
          if (CheckTime(StartTime[C_VACUUM], (uint64_t(Menu.Items[M_VACUUM_TIME].Value * 1000.0)))) {Pneumo.BlowOff(); VacEnd = 1; }
          //cooling
          PWMc.FanOn = (!CoolEnd && (millis()>=StartTime[C_COOLING]));
          if (CheckTime(StartTime[C_COOLING], (uint64_t(Menu.Items[M_COOL_TIME].Value * 1000.0)))) {CoolEnd = 1; PWMc.FanOn = 0;}
          
          if (!CoolEnd&&!VacEnd) Display.SetState(ACT_VAC_COOL);
          if (!PWMc.FanOn&&!VacEnd) Display.SetState(ACT_VACUUM);
          if (VacEnd&&!CoolEnd) Display.SetState(ACT_COOL_ON);
          if (CoolEnd && VacEnd){
            CoolEnd = 0; 
            VacEnd = 0;
            StartTime[C_TABLE_DOWN] = millis(); 
            Stage = C_TABLE_DOWN;
          }  
        }
        
        else{
          Pneumo.VacuumTable();
          Display.SetState(ACT_VACUUM);
          if (CheckTime(StartTime[C_VACUUM], (uint64_t(Menu.Items[M_VACUUM_TIME].Value * 1000.0)))){
            Pneumo.BlowOff();
            StartTime[C_COOLING] = millis(); 
            Stage = C_COOLING;
          }
        }
        break;}
        
      case C_COOLING: {
        //Serial.println("C_COOLING ");
        PWMc.FanOn = 1;
        Display.SetState(ACT_COOL_ON);
        if (CheckTime(StartTime[C_COOLING], (uint64_t(Menu.Items[M_COOL_TIME].Value * 1000.0)))){
          PWMc.FanOn = 0;
          Display.SetState(ACT_COOL_OFF);
          StartTime[C_TABLE_DOWN] = millis(); 
          Stage = C_TABLE_DOWN;
        }
        break;}
        
      case C_TABLE_DOWN: {
        //Serial.println("C_TABLE_DOWN ");
        Pneumo.Set(DIST_TABLE, DOWN); 
        Display.SetState(ACT_TABLE_DOWN);
        if (CheckTime(StartTime[C_TABLE_DOWN], (uint64_t(Menu.Ext[EXT_PNEUM_TIME].Value * 1000.0)))){
          StartTime[C_FRAME_UP] = millis();
          Stage = C_FRAME_UP; 
        }
        break;}
        
      case C_FRAME_UP: {
        //Serial.println("C_FRAME_UP ");
        if(Motor.Position(MOTOR_HEATER) <= Menu.Ext[EXT_SAFE_POS].Value){
          Pneumo.Set(DIST_FRAME, UP); 
          Display.SetState(ACT_FRAME_UP);
          if (CheckTime(StartTime[C_FRAME_UP], (uint64_t(Menu.Ext[EXT_PNEUM_TIME].Value * 1000.0)))){
            Stage = C_DELAY_PULL; 
          }
        }
        else{
          StopError(ACT_ERR_FRAMEUP);
        }
        break;}
        
        case C_DELAY_PULL: {
        //Serial.println("C_VACUUM");
        Display.SetState(ACT_DELAY_PULL);
        if (!DelayRun) {
          DelayRun = 1; 
          StartTime[C_DELAY_PULL] = millis();
          }
   
        else{
          if (CheckTime(StartTime[C_DELAY_PULL], (uint64_t(Menu.Items[M_DELAY_BEFORE_PULLING].Value * 1000.0)))){
            DelayRun = 0;
            Stage = C_PULL; 
          }
        }
        break;}
        
      case C_PULL: {
        //Serial.println("C_PULL ");
        if (Menu.Items[M_PULLER_ON].Value > 0.0){
          Motor.Move(MOTOR_PULLER, Menu.Items[M_PULLER_DISTANCE].Value);/* blocking move heater to work*/ 
          Display.SetState(ACT_PULL_RIGHT);
          StartTime[C_BLOW] = millis();      
          Serial.println(Motor.Position(MOTOR_PULLER));   
          if (Motor.MoveDone(MOTOR_PULLER)) {
            Display.SetState(ACT_PULL_STOP);
            Motor.Stop(MOTOR_PULLER);
            StartTime[C_DONE] = millis(); 
            LastCycleTime = StartTime[C_DONE] - StartTime[C_START]; 
            Stage = C_DONE;  
          }
        }
        else {
          StartTime[C_DONE] = millis(); 
          LastCycleTime = StartTime[C_DONE] - StartTime[C_START];   
         
          Stage = C_DONE; }
        break;}
      case C_DONE: {
        //Serial.println("C_DONE");
        Display.SetState(ACT_READY);
        if (Mode == MODE_AUTO) {
          /*some delay*/
          CycCounter++;
          Display.CycTimeUpdated = 1;
          Stage = C_START;}
        if (Mode == MODE_SEMI) {SemiDone = 1; 
          Display.SetState(ACT_SEMI_DONE);
          Display.CycTimeUpdated = 1;
          /*Mode = MODE_MANUAL; some delay*/
          }
        break;}
    }
  }
};

void Cycle_class::Stop(void){
  CycRun = 0;
  Display.SetState(ACT_STOP);
  Pneumo.BlowOff();
  Cycle.SwitchToManual();
  Cycle.Stage = C_DONE;
  
 // MotorStop(MOTOR_PULLER);
  //MotorStop(MOTOR_HEATER);
};

void Cycle_class::StopError(uint8_t err){
  Stop();
  Display.SetState(err);
  
 // MotorStop(MOTOR_PULLER);
  //MotorStop(MOTOR_HEATER);
};

void Cycle_class::SwitchToManual(void){Mode = MODE_MANUAL;};
void Cycle_class::SwitchToAuto(void){Mode = MODE_AUTO;};
void Cycle_class::SwitchToSemi(void){Mode = MODE_SEMI;};

bool Cycle_class::CheckTime(uint64_t Start, uint64_t Time2Pass){
  bool to_return = (millis() >= (Start + Time2Pass));
  return to_return;
}

void Cycle_class::Start(void){
  if ((Stage == C_DONE)&&((Mode==MODE_AUTO)||(Mode==MODE_SEMI))){
    GetReady();
    /* some delay*/
    Stage = C_START;
    CycRun = 1;
  }
};

void Cycle_class::GetReady(void){
  Display.SetState(ACT_GETREADY);
  Motor.GoTo(MOTOR_HEATER, HOME);
  Motor.BlockingRun(MOTOR_HEATER, 0);
  Pneumo.Set(DIST_TABLE, DOWN);
  //Pneumo.Set(DIST_FRAME, UP);
  Pneumo.BlowOff();
  
  PWMc.FanOn = 0;
  //PWMc.HeatingOn = 1;

  //delay(500);
  // here move heater to home
  
};
