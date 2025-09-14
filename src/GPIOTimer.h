#pragma once
#include <Arduino.h>

class GPIOStateTimer {
  private:
    int pin;           // GPIO pin to monitor
    unsigned long duration; // Duration to wait before setting pin to LOW
    unsigned long lastHighTime; // Timestamp of when pin was last HIGH

  public:
    // Constructor to initialize the pin and duration
    GPIOStateTimer(int pin, unsigned long duration) {
      this->pin = pin;
      this->duration = duration;
      this->lastHighTime = 0;

      pinMode(pin, INPUT); // Set the pin as input (could be INPUT_PULLUP depending on the circuit)
    }

    // Function to check pin state and handle timing logic
    void update() {
      if (digitalRead(pin) == HIGH) { // Check if pin is HIGH
        if (lastHighTime == 0) {
          // Pin just went HIGH, record the time
          lastHighTime = millis();
        } else if (millis() - lastHighTime >= duration) {
          // Pin has been HIGH for the required duration, set it LOW
          digitalWrite(pin, LOW);
          lastHighTime = 0; // Reset timer
        }
      } else {
        lastHighTime = 0; // Reset the timer if pin goes LOW
      }
    }
};
