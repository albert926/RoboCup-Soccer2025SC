/*
Make robot start with pushbutton and then go in square four times each time increasing the speed to max.
www.alberttalkstech.com
https://git.alberttalkstech.com/albert/RoboCup-Soccer
*/

#include <Arduino.h>

const uint8_t button = 4; // Change 11 to your actual button pin
const uint8_t ma1 = 3;
const uint8_t ma2 = 2;
const uint8_t mb1 = 5;
const uint8_t mb2 = 6;
const uint8_t speedIncrement = 10; // Speed increment for each square
uint8_t speed = 80; // Initial speed
const uint8_t turnDelay = 450; // Delay for turning

void Move(int left, int right);
void forward();
void turnRight();
void Stop();

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP); // Set button pin as input with pull-up resistor
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
  for (int i = 0; i < 4; i++) {
      forward(); // Move forward
      delay(1000); // Move forward for 1 seconds
      Stop(); // Stop before turning
      delay(1000); // Wait for a moment before turning
      turnRight();
      Stop(); // Stop before increasing speed
      Serial.print("Completed square ");
      speed += speedIncrement;
  }
  // Serial.println(digitalRead(button));
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

void turnRight() {
  Move(150, 0); // Left motor at speed, right motor stopped
  delay(turnDelay); // Adjust delay for turning speed
  Stop();
  delay(1000); // Wait before next action
}