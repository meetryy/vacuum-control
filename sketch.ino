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
  PWMc.Init();
  Cycle.Init();
  Pneumo.Init();
  Motor.Init();
  
  PWMc.Beep(0);
  Cycle.SwitchToManual();
  
  Pneumo.Power(OPEN);
}

void loop() {
  Buttons.Scan();
  Buttons.Proc();
  Display.Refresh();
  Buttons.ProcEncoder();
  PWMc.Spin();
  Cycle.Run();
  Memory.Worker();
  Motor.Run();
  //Pneumo.Run();
}
