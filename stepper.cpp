#include "stepper.h"
#include "arduino.h"
#include "menu.h"

#include <AccelStepper.h>

Motor_class Motor;

AccelStepper puller(1, A7, A6);
AccelStepper heater(1, A5, A4);

void Motor_class::Init(void){
  //puller.setMaxSpeed(Menu.Ext[EXT_PULLER_SPEED].Value*(int)Menu.Ext[EXT_PULLER_STEP].Value);
  puller.setAcceleration(Menu.Ext[EXT_PULLER_ACCEL].Value);
  //heater.setMaxSpeed((int)Menu.Ext[EXT_HEATER_SPEED].Value*(int)Menu.Ext[EXT_PULLER_STEP].Value);
  heater.setAcceleration(Menu.Ext[EXT_HEATER_ACCEL].Value);
  puller.setMaxSpeed(Menu.Ext[EXT_PULLER_SPEED].Value*(int)Menu.Ext[EXT_PULLER_STEP].Value);
  heater.setMaxSpeed(Menu.Ext[EXT_HEATER_SPEED].Value*(int)Menu.Ext[EXT_HEATER_STEP].Value);
 
};

void Motor_class::Spin(uint8_t motor, uint16_t speed){
  Init();
  SpinType[motor] = CONT;
  if (motor == MOTOR_PULLER) puller.setSpeed((int)Menu.Ext[EXT_PULLER_SPEED].Value*(int)Menu.Ext[EXT_PULLER_STEP].Value);     
  if (motor == MOTOR_HEATER) heater.setSpeed((int)Menu.Ext[EXT_HEATER_SPEED].Value*(int)Menu.Ext[EXT_HEATER_STEP].Value); 
}

void Motor_class::Run(void){
  if (SpinType[MOTOR_HEATER] == CONT) heater.runSpeed();
  else heater.run();
 
  if (SpinType[MOTOR_PULLER] == CONT) puller.runSpeed(); 
  else puller.run(); 
}

void Motor_class::Rotate(uint8_t motor, uint32_t distance, bool dir){

}

void Motor_class::GoTo(uint8_t motor, bool pos){
  if (SpinType[motor] == STOP) Init();
  SpinType[motor] = NOT_CONT;
  if (motor == MOTOR_HEATER) {
    uint64_t NewPos = (pos == WORK) * ((int)Menu.Items[M_HEATER_TRAVEL].Value * (int)Menu.Ext[EXT_HEATER_STEP].Value);
    heater.moveTo(NewPos);
    GoToActivated = 1;
  } 
}

void Motor_class::Move(uint8_t motor, int32_t pos){
  Init();
  if (SpinType[motor] == STOP) {
    if (motor == MOTOR_HEATER) {
      DesiredPos[motor] = heater.currentPosition() + (pos * (uint32_t)Menu.Ext[EXT_HEATER_STEP].Value);
      heater.move(pos*(int)Menu.Ext[EXT_HEATER_STEP].Value); 
    }  
    if (motor == MOTOR_PULLER) { 
      DesiredPos[motor] = puller.currentPosition() + (pos * (uint32_t)Menu.Ext[EXT_PULLER_STEP].Value);
      puller.move(pos*(int)Menu.Ext[EXT_PULLER_STEP].Value);
    } 
    SpinType[motor] = NOT_CONT;
    
  } 
}

void Motor_class::Stop(uint8_t motor){
  Init();
  SpinType[motor] = STOP;
  if (motor == MOTOR_PULLER){ puller.stop(); puller.runToPosition();}
  if (motor == MOTOR_HEATER){heater.stop(); heater.runToPosition(); }  
  GoToActivated = 0;
}

uint32_t Motor_class::Done(uint8_t motor){
  uint32_t to_return = 0 ;
  if (motor == MOTOR_PULLER){to_return = puller.distanceToGo();}
  if (motor == MOTOR_HEATER){to_return = heater.distanceToGo();}
  if (to_return == 0) SpinType[motor] = STOP;
  return to_return;
}

bool Motor_class::MoveDone(uint8_t motor){
  /*
  uint32_t currPos = 0;//currentPosition();
  bool to_return = 0;
  if (motor == MOTOR_PULLER) currPos = puller.currentPosition();
  if (motor == MOTOR_HEATER) currPos = heater.currentPosition();
  if (currPos >= DesiredPos[motor]){
    Stop(motor);
    LastPos[motor] = DesiredPos[motor];
    to_return = 1;
  }
  return to_return;
  */
  uint32_t dtg = 0;//currentPosition();
  bool to_return = 0;
  if (motor == MOTOR_PULLER) dtg = puller.distanceToGo();
  if (motor == MOTOR_HEATER) dtg = heater.distanceToGo();
  if (dtg == 0) {to_return = 1;}
  return to_return;
}

void Motor_class::BlockingRun(uint8_t motor, uint32_t pos){
  if (motor == MOTOR_PULLER) {
    puller.moveTo(pos);
    puller.runToPosition();
  }
  if (motor == MOTOR_HEATER){
    heater.moveTo(pos);
    heater.runToPosition();
  }
 

}


uint32_t Motor_class::Position(uint8_t motor){
   Init();
  uint32_t to_return = 0 ;
  if (motor == MOTOR_PULLER){to_return = puller.currentPosition();}
  if (motor == MOTOR_HEATER){to_return = heater.currentPosition();}  
  return to_return;
}
