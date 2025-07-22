#include <Arduino.h>

const uint8_t button = A1; // Change 13 to your actual button pin
const uint8_t light = 2; // Change 6 to your actual light pin
const unsigned long delayTime = 20; // Delay time in milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP); // Set button pin as input with pull-up resistor
  pinMode(light, OUTPUT);
  Serial.println("Setup complete. Press the button to toggle the light.");
  while(digitalRead(button) == HIGH) {
    // Wait for the button to be pressed
  }
}

void loop() {
  digitalWrite(light, HIGH); // Turn on the light
  delay(delayTime); // Wait for delayTime
  digitalWrite(light, LOW); // Turn off the light
  delay(delayTime); // Wait for delayTime
  Serial.println("Light toggled");
}
