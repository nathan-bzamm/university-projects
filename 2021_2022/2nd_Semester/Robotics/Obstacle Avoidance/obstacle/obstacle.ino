#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"

#define PIN_Motor_STBY 3
#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 7
#define PIN_Motor_AIN_1 8

DeviceDriverSet_ULTRASONIC myUltrasonic;

void forward() {
  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, 50);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, 50);
}

void stopMov() {
  digitalWrite(PIN_Motor_STBY, LOW);
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 0);
}

void setup()
{
  Serial.begin(9600);
  myUltrasonic.DeviceDriverSet_ULTRASONIC_Init();
}

void loop()
{ 
  if (myUltrasonic.DeviceDriverSet_ULTRASONIC_Test() >= 15) {
    forward();
  } else {
    stopMov();
  }
}
