#include "pwm.h"
#include "arduino.h"
#include "menu.h"

#include <Adafruit_NeoPixel.h>
#define PIN 13
#define count_led 1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(count_led, PIN, NEO_GRB + NEO_KHZ800); //first number change does distance between colors


#define PERIOD_MS 5000

PWM_class PWM;

void PWM_class::Init(void){

  pixels.begin();
  pixels.show(); // Устанавливаем все светодиоды в состояние "Выключено"
  pixels.setPixelColor(0, pixels.Color(0,150,0)); 
  pixels.show();

  ImpulseCounter = 0;
  Depth = 100;
  ImpulseMs = PERIOD_MS / Depth;
  
  HeaterPins[0] = 3;  HeaterPins[1] = 4;  HeaterPins[2] = 5;
  HeaterPins[3] = 6;  HeaterPins[4] = 7;  HeaterPins[5] = 8;
  HeaterPins[6] = 9;  HeaterPins[7] = 10;  HeaterPins[8] = 11;
  
  //FanPin = 45;
  BeepPin = A0;
  
  for (int i=0; i<9; i++){
    pinMode(HeaterPins[i], OUTPUT);
  }
  
  pinMode(FanPin, OUTPUT);
  FanOn = 0;
  HeatingOn = 0;
  pinMode(BeepPin, OUTPUT);
};

  
void PWM_class::Spin(void){
  bool half = 0;
  digitalWrite(BeepPin, millis() < BeepEnd);
  if (millis() > OldTime + ImpulseMs){
    
    if (ImpulseCounter < Depth) ImpulseCounter++;
    else ImpulseCounter = 0;
    for (int i=0; i<9; i++) {
      Heaters[i] = HeatingOn*((uint8_t)(Menu.Items[M_HEATER0_POWER + i].Value*Menu.Items[M_HEATER_POWER_GLOBAL].Value/(float)Depth) >= ImpulseCounter);
      digitalWrite(HeaterPins[i], Heaters[i]);
    }
      
    OldTime = millis();
    uint8_t cool = (uint8_t)(Menu.Items[M_COOL_POWER].Value/Menu.Items[M_COOL_POWER].Max*255.0);
    analogWrite(FanPin, FanOn * cool);
  }
};

void PWM_class::Beep(bool length){
  BeepEnd = 20+length*200+millis();
}
  
