#include "debug.h"
#include "arduino.h"

Debug_class Debug;

void Debug_class::Init(void){
  Serial.begin(9600);
  Serial.println("start"); 
  
};

void Debug_class::Print(char* in){
  Serial.print(in);
}
void Debug_class::Println(char* in){
  Serial.print("\r\n");
  Serial.print(in);
}

 
