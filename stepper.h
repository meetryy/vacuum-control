#ifndef Stepper_h
#define Stepper_h

#include <stdint.h>
#include "arduino.h"
#include "pins.h"
enum MotorEnum{MOTOR_HEATER, MOTOR_PULLER, MOTOR_NR};
enum StepperState {RUNNING, DONE};
enum StepperSpinType {CONT, NOT_CONT, STOP};
enum StepperPos {WORK, HOME};

class Motor_class{  
public: 
  void Init(void);
  //void Print(char* in);
  //void Println(char* in); 
  int StepPins[MOTOR_NR] = {HEATER_STEP_PIN, PULLER_STEP_PIN};
  int DirPins[MOTOR_NR]  = {HEATER_DIR_PIN, PULLER_DIR_PIN};
  uint8_t SpinType[MOTOR_NR] = {STOP, STOP};
  int32_t LastPos[MOTOR_NR] = {0,0};
  int32_t DesiredPos[MOTOR_NR] = {0,0};
  bool GoToActivated = 0;
  
  void Spin(uint8_t motor, uint16_t speed);
  void Rotate(uint8_t motor, uint32_t distance, bool dir);
  void Stop(uint8_t motor);
  uint32_t Done(uint8_t motor);
  void Run(void);
  void GoTo(uint8_t motor, bool pos);
  uint32_t Position(uint8_t motor);
  void Move(uint8_t motor, int32_t pos);
  bool MoveDone(uint8_t motor);
  void BlockingRun(uint8_t motor, uint32_t pos);
  };
  
  extern Motor_class Motor;

#endif
