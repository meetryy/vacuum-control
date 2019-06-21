#ifndef Debug_h
#define Debug_h

#include <stdint.h>

class Debug_class{  
public: 
  void Init(void);
  void Print(char* in);
  void Println(char* in); 
  };
  
  extern Debug_class Debug;

#endif
