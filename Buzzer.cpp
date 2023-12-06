#include "Buzzer.h"

Buzzer::Buzzer(int pin) : buzzerPin(pin) {
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as an output
}

void Buzzer::play() {
  if (!playing) {
  return; // No tune set or not playing
  }
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= noteDuration) {
    previousMillis = currentMillis;

    if (noteIndex < 8) {
      int duration = 1000 / noteDurations[noteIndex];
      tone(buzzerPin, melody[noteIndex], duration);
      previousMillis += duration * 1.3;
      isPlaying = true;
      noteIndex++;
    } else {
      noteIndex = 0;
      isPlaying = false;
    }
  }

  if (!isPlaying && currentMillis - previousMillis >= pauseBetweenNotes) {
    noTone(buzzerPin);
    previousMillis = currentMillis;
    isPlaying = false;
    playing = false;
  }
}

void Buzzer::offBuzzer() {
  noTone(buzzerPin);
}

void Buzzer::onBuzzer() {
  tone(buzzerPin, 700);
}

void Buzzer::setTune() {
//   noInterrupts();
  playing = true;
//   interrupts();
}

