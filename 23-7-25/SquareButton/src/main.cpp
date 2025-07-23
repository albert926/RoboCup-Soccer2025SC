/*
Make robot start with pushbutton and then go in square four times each time increasing the speed to max.
www.alberttalkstech.com
*/

#include <Arduino.h>

const uint8_t button = A1; // Change 11 to your actual button pin
const uint8_t light = 2; // Change 6 to your actual light pin
const uint8_t ma1 = 3;
const uint8_t ma2 = 2;
const uint8_t mb1 = 5;
const uint8_t mb2 = 6;
const uint8_t speedIncrement = 10; // Speed increment for each square
uint8_t speed = 80; // Initial speed

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP); // Set button pin as input with pull-up resistor
  pinMode(light, OUTPUT);
  pinMode(ma1, OUTPUT);
  pinMode(ma2, OUTPUT);
  pinMode(mb1, OUTPUT);
  pinMode(mb2, OUTPUT);
  Serial.println("Setup complete. Press the button to toggle the light.");
  while(digitalRead(button) == HIGH) {
    // Wait for the button to be pressed
  }
}

void loop() {
  for (int i = 0; i < 4; i++) {
      Move(speed, speed); // Move forward at current speed
      delay(1000); // Move forward for 1 seconds
      Stop(); // Stop before turning
      delay(500); // Wait for a moment before turning
      Move(150, 0); // Turn right
      delay(500);
      Stop(); // Stop before increasing speed
      speed += speedIncrement;
  }
}

void Move(int left, int right) {
  if (left > 0) {
    analogWrite(ma1, left);
    digitalWrite(ma2, LOW);
  } else if (left < 0) {
    analogWrite(ma2, -left);
    digitalWrite(ma1, LOW);
  } else {
    digitalWrite(ma1, LOW);
    digitalWrite(ma2, LOW);
  } 
  
  if (right > 0) {
    analogWrite(mb1, right);
    digitalWrite(mb2, LOW);
  } else if (right < 0) {
    analogWrite(mb2, -right);
    digitalWrite(mb1, LOW);
  } else {
    digitalWrite(mb1, LOW);
    digitalWrite(mb2, LOW);
  }
}

void Stop() {
  Move(0, 0);
}