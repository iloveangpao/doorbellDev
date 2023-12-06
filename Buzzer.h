#ifndef BUZZER_H
#define BUZZER_H

#include "Arduino.h"

class Buzzer {
private:
  int melody[8] = { 262, 294, 330, 349, 392, 440, 494, 523 }; // Define the frequencies for the notes
  int noteDurations[8] = { 4, 4, 4, 4, 4, 4, 4, 4 };
  
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


