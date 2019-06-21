#include "menu.h"
#include "buttons.h"
#include "display.h"
#include "pwm.h"
#include "cycle.h"
#include "debug.h"
#include "pneumatic.h"
#include "memory.h"
#include "stepper.h"

void setup() {
  Debug.Init();
  Display.Init();
  Display.ShowSplash();
  Buttons.Init();
  Menu.Init();
  Menu.LoadDefaults();
  
  Memory.Init();
  Memory.Load();
  PWM.Init();
  Cycle.Init();
  Pneumo.Init();
  Motor.Init();
  
  PWM.Beep(0);
  Cycle.SwitchToManual();
  
  Pneumo.Power(OPEN);
}

void loop() {
  Buttons.Scan();
  Buttons.Proc();
  Display.Refresh();
  Buttons.ProcEncoder();
  PWM.Spin();
  Cycle.Run();
  Memory.Worker();
  Motor.Run();
  //Pneumo.Run();
}
