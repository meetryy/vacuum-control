#ifndef Mem_h
#define Mem_h

#include <stdint.h>

enum EepStates{EEP_READY, EEP_LOADING, EEP_SAVING, EEP_OK, EEP_FAIL};

class Memory_class{  
public: 
struct LightMenu_t{
    float Value;
    bool IsBool;
  };

  int EEPaddr = 0;     
  float CheckNumber = 123456.0;
  uint8_t EEPstate;
  uint64_t OldStateTime = 0;
  bool Count = 0;
  
  void Init(void);
  void Save(void);
  void Load(void);
  void Worker(void);
  };

  
extern Memory_class Memory;

#endif
