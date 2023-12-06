#ifndef BUZZER_H
#define BUZZER_H

#include "Arduino.h"

class Buzzer {
private:
  int melody[10] = { 262, 294, 330, 349, 392, 440, 494, 523, 0, 0 }; // Define the frequencies for the notes
  int noteDurations[10] = { 4, 4, 4, 4, 4, 4, 4, 4, 1, 1 };
  
  unsigned long previousMillis = 0;
  const long noteDuration = 1000;
  const long pauseBetweenNotes = 50;
  int noteIndex = 0;
  bool isPlaying = false;
  int buzzerPin;
  bool playing;

public:
  Buzzer(int pin);
  void play();
  void offBuzzer();
  void onBuzzer();
  void setTune();
};

#endif


