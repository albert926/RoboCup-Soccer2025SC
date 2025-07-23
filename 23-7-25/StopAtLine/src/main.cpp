/*
Make robot start with pushbutton and then go in square four times each time increasing the speed to max.
www.alberttalkstech.com
https://git.alberttalkstech.com/albert/RoboCup-Soccer
*/

#include <Arduino.h>

const uint8_t button = 4; // Change 4 to your actual button pin
const uint8_t ma1 = 3;
const uint8_t ma2 = 2;
const uint8_t mb1 = 5;
const uint8_t mb2 = 6;
const uint8_t lineSensor = 7;
uint8_t speed = 80; // Initial speed


void Move(int left, int right);
void forward();
void Stop();

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP); // Set button pin as input with pull-up resistor
  pinMode(lineSensor, INPUT);
  // Set motor pins as outputs
  pinMode(ma1, OUTPUT);
  pinMode(ma2, OUTPUT);
  pinMode(mb1, OUTPUT);
  pinMode(mb2, OUTPUT);
  Serial.println("Setup complete.");
  while(digitalRead(button)) {
    // Wait for the button to be pressed
  }
}

void loop() {
  forward();
  while(digitalRead(lineSensor) == LOW) {
    // Continue moving forward until the line sensor detects a line
  }
  Stop(); // Stop when the line is detected
  Serial.println("Line detected, stopping.");
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
  digitalWrite(ma1, HIGH);
  digitalWrite(ma2, HIGH);
  digitalWrite(mb1, HIGH);
  digitalWrite(mb2, HIGH);
}

void forward() {
  Move(speed, speed); // Move both motors forward at the current speed
  Serial.print("Moving forward at speed: ");
  Serial.println(speed);
}