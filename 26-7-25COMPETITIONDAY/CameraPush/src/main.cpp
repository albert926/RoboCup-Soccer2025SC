/*
This is a code
*/

bool debug = true; // Set to true to enable debug

#include "Arduino.h"
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "PID.h"

const uint8_t ma1 = 3;
const uint8_t ma2 = 2;
const uint8_t mb1 = 5;
const uint8_t mb2 = 6;

const uint8_t line = 8; // Line sensor pin

const uint8_t abortLED = 28;
const uint8_t custLED = 29;

const uint8_t startInterruptPin = 9;
uint8_t roboStat = 1; // 1 for running, 0 for stopped

HUSKYLENS huskylens;
SoftwareSerial mySerial(1, 0); // RX, TX
void printResult(HUSKYLENSResult result);

const uint8_t setpoint = 160; // Example: center of 320px image

volatile bool InterStat = 0;

void Move(int left, int right);
void Stop();
void readHusky();

uint8_t speed = 50;
unsigned long lastSpeedUpdate = millis();

bool timeMove = true; // Set to true to enable speed increase on line sensor trigger

void toggleInter();

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    pinMode(ma1, OUTPUT);
    pinMode(ma2, OUTPUT);
    pinMode(mb1, OUTPUT);
    pinMode(mb2, OUTPUT);
    pinMode(line, INPUT);
    pinMode(abortLED, OUTPUT);
    pinMode(custLED, OUTPUT);
    pinMode(startInterruptPin, INPUT_PULLUP);

    if(debug) {
      while (!huskylens.begin(mySerial))
      {
          Serial.println(F("Begin failed!"));
          Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
          Serial.println(F("2.Please recheck the connection."));
          delay(100);
      }
    }

    while(digitalRead(startInterruptPin) == HIGH){ 
    }
    Serial.println("HI!");

    attachInterrupt(9, toggleInter, FALLING);
}

void loop() {
  while(!roboStat){delay(10); digitalWrite(custLED, LOW); Stop();}

  static bool seeBallFlag = 0;
  huskylens.requestBlocks(1); //Only pull the correct ID
  if (huskylens.available()) { //Make sure its available
    huskylens.read();
    seeBallFlag = 1;
  }

  if (seeBallFlag) {
    digitalWrite(custLED, HIGH); // Make it high to indicate a sensed a ball
    seeBallFlag = 0; // Set it low, because we indicated it, INTER!
    Serial.println("SAW BALL");
    delay(100); // Wait for a bit to stabilize
    Stop();
    Move(50, 50); // Move forward

    if(!timeMove) {
      while(digitalRead(line) == LOW) { // Wait for the line sensor to be triggered
        delay(10);
      }
    }

    if(timeMove) {
      while(digitalRead(line) == LOW) { // Wait for the line sensor to be triggered
        if (millis() - lastSpeedUpdate >= 1000) { // Every second
          speed += 10;
          if (speed > 255) speed = 255; // Cap at max PWM value
          Move(speed, speed);
          lastSpeedUpdate = millis();
        }
        delay(10);
      }
    }
    
    Serial.println("Line sensor triggered, stopping robot.");
    Stop(); // Stop the robot when the line sensor is triggered
    digitalWrite(custLED, LOW); // Turn off the custom LED
    toggleInter(); // Toggle the interrupt state

    //Wait an eternity for the robot to push the ball
  } else {
    // Add find ball program here

    digitalWrite(custLED, LOW);  // Make it low, because it didn't see it
    Serial.println("Couldn't find ball");
    Stop();
    Move(60, -60); // Find ball by turning
  }
}

void Move(int left, int right) {
  if (left > 0) {
    analogWrite(ma1, left);
    digitalWrite(ma2, LOW);
  } else if (left < 0) {
    analogWrite(ma2, left);
    digitalWrite(ma1, LOW);
  }
  
  if (right > 0) {
    digitalWrite(mb1, LOW);
    analogWrite(mb2, right);
  } else if (right < 0) {
    digitalWrite(mb2, LOW);
    analogWrite(mb1, right);
  }
}

void Stop() {
  digitalWrite(ma1, HIGH);
  digitalWrite(ma2, HIGH);
  digitalWrite(mb1, HIGH);
  digitalWrite(mb2, HIGH);
}

void toggleInter()
{
  InterStat = !InterStat;
  if(InterStat) {
      Serial.println("Abort!!!");
      roboStat = 0; // Stop the robot
      digitalWrite(abortLED, HIGH);
      digitalWrite(custLED, LOW);
      Stop();  
  } else {
      Serial.println("Abort terminated.");
      roboStat = 1; // Continue running
      digitalWrite(abortLED, LOW); 
  }
}