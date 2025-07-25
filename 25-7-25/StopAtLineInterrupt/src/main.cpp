/*
This is a code
*/

#include "Arduino.h"
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "PID.h"

const uint8_t ma1 = 3;
const uint8_t ma2 = 2;
const uint8_t mb1 = 5;
const uint8_t mb2 = 6;

const uint8_t abortLED = 28;

const uint8_t startInterruptPin = 9;
uint8_t roboStat = 1; // 1 for running, 0 for stopped

volatile bool InterStat = 0;

void Move(int left, int right);
void Stop();
void readHusky();

void toggleInter();

void setup() {
    Serial.begin(115200);
    pinMode(ma1, OUTPUT);
    pinMode(ma2, OUTPUT);
    pinMode(mb1, OUTPUT);
    pinMode(mb2, OUTPUT);
    pinMode(abortLED, OUTPUT);
    pinMode(startInterruptPin, INPUT_PULLUP);

    while(digitalRead(startInterruptPin) == HIGH){
    }
    Serial.println("HI!");

    attachInterrupt(9, toggleInter, FALLING);
}

void loop() {
    if(InterStat) {
      roboStat = 0; // Stop the robot
      digitalWrite(abortLED, HIGH);
      Serial.println("Abort!!!");
      Stop();
    } else {
      roboStat = 1; // Continue running
      digitalWrite(abortLED, LOW);
      Serial.println("Abort terminated.");
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
  digitalWrite(ma1, HIGH);
  digitalWrite(ma2, HIGH);
  digitalWrite(mb1, HIGH);
  digitalWrite(mb2, HIGH);
}

void toggleInter()
{
  InterStat = !InterStat;
}