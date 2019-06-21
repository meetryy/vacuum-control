#ifndef Cycle_h
#define Cycle_h

#include <stdint.h>
  enum Stages{C_START, C_FRAME_DOWN, C_HEATER_WORK, C_HEATING, C_HEATER_HOME, C_BLOW, C_TABLE_UP, 
              C_DELAY_VAC, C_VACUUM, C_COOLING, C_TABLE_DOWN, C_FRAME_UP, C_DELAY_PULL, C_PULL, C_DONE, STAGE_NR};
              
  enum modes{MODE_MANUAL, MODE_SEMI, MODE_AUTO, MODE_NR};
  
  enum ManualAct{
                  ACT_FRAME_DOWN, ACT_FRAME_UP,
                  ACT_TABLE_DOWN,ACT_TABLE_UP,
                  ACT_HEATER_HOME,ACT_HEATER_WORK,ACT_HEATER_STOP,
                  ACT_HEATING_ON,ACT_HEATING_OFF,
                  ACT_BLOW, ACT_VACUUM, ACT_VACUUM_OFF, ACT_BLOW_OFF,ACT_ACC_CHG,ACT_ACC_CHG_OFF,
                  ACT_COOL_ON,ACT_COOL_OFF,
                  ACT_VAC_COOL,
                  ACT_PULL_RIGHT,ACT_PULL_LEFT,ACT_PULL_STOP,
                  ACT_GETREADY, ACT_READY, ACT_SEMI_DONE, ACT_STOP,
                  ACT_DELAY_VAC, ACT_DELAY_PULL,
                  ACT_ERR_FRAMEUP,
                  MAN_NR};                
class Cycle_class{  
public:   

  uint8_t Mode;
  uint8_t Stage;

  bool CheckTime(uint64_t Start, uint64_t Time2Pass);
  uint64_t LastCycleTime;
  uint64_t StartTime[STAGE_NR];
  bool SemiDone = 0;
  uint8_t LastAction = 0;
  uint8_t OldLastAction = 255;
  uint8_t OldMode = 255;
  bool CycRun = 0;
  bool AutoPendStop = 0;
  uint32_t CycCounter = 0;
  bool DelayRun = 0;
  bool VacEnd = 0;
  bool CoolEnd = 0;
  uint32_t HeatingEndTime = 0;
  uint32_t ChgBeginTime = 0;
  bool AutoChg = 0;
  
  void Init(void);
  void Run(void);
  void Start(void);
  void Stop(void);  
  void GetReady(void); 
  void SwitchToManual(void); 
  void SwitchToAuto(void); 
  void SwitchToSemi(void); 
  void SpinModes(void);
  void StopError(uint8_t err);
  };
  
  extern Cycle_class Cycle;

#endif
