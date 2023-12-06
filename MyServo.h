#ifndef MyServo_H
#define MyServo_H

#include "Arduino.h"
#include "ESP32Servo.h"

class MyServo {
  public:
    MyServo(int pin);
    void setAngle(int deg);

  private:
    int _pin;
    Servo _servo;
};

#endif
