#ifndef DigitalSensor_H
#define DigitalSensor_H

#include "Arduino.h"

class DigitalSensor {
  public:
    DigitalSensor(int pin, bool activeHigh);
    void setMotionCallback(void (*callback)());
    void setNoMotionCallback(void (*callback)());
    void initInterrupt(void (*isrFunction)(), bool rising);
    bool read();
    void start();

  private:
    int _pin;
    bool _activeHigh;
    void (*_motionCallback)();
    void (*_noMotionCallback)();
    void (*_isrFunction)();
    static void _isrWrapper();
};

#endif

