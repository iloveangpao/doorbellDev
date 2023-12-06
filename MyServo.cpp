#include "MyServo.h"

MyServo::MyServo(int pin) {
  _pin = pin;
  _servo.attach(_pin);
}

void MyServo::setAngle(int deg) {
    if (deg >= 0 && deg <= 180) {
        _servo.write(deg);
    } else {
        // Handle out-of-range error
        Serial.println("Error: Angle should be between 0 and 180 degrees.");
        // You can choose to set the angle to the closest valid value or ignore the command
        // For now, I'll set it to the nearest valid value (0 or 180 degrees)
        if (deg < 0) {
            _servo.write(0);
        } else {
            _servo.write(180);
        }
    }
}