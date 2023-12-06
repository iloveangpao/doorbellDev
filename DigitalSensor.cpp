#include "DigitalSensor.h"

DigitalSensor::DigitalSensor(int pin, bool activeHigh) {
  _pin = pin;
  _activeHigh = activeHigh;
  _motionCallback = NULL;
  _noMotionCallback = NULL;
  _isrFunction = NULL;
  pinMode(_pin, INPUT); // Set pin mode as INPUT during object initialization
}

void DigitalSensor::setMotionCallback(void (*callback)()) {
  _motionCallback = callback;
}

void DigitalSensor::setNoMotionCallback(void (*callback)()) {
  _noMotionCallback = callback;
}

void DigitalSensor::initInterrupt(void (*isrFunction)(), bool rising) {
  _isrFunction = isrFunction;
  if(rising) attachInterrupt(digitalPinToInterrupt(_pin), _isrFunction, RISING);
  else attachInterrupt(digitalPinToInterrupt(_pin), _isrFunction, FALLING);
}

bool DigitalSensor::read() {
  // if (_activeHigh) return digitalRead(_pin) == HIGH; 
  // else return digitalRead(_pin) == LOW;
  return digitalRead(_pin) == HIGH; 
}

void DigitalSensor::start() {
  while (true) {
    if (read()) {
      if (_motionCallback != NULL) {
        _motionCallback();
      }
      delay(100); // Debounce time
    } else {
      if (_noMotionCallback != NULL) {
        _noMotionCallback();
      }
      delay(100); // Debounce time
    }
  }
}
