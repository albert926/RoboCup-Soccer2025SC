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

void toggleInter();

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    pinMode(ma1, OUTPUT);
    pinMode(ma2, OUTPUT);
    pinMode(mb1, OUTPUT);
    pinMode(mb2, OUTPUT);
    pinMode(abortLED, OUTPUT);
    pinMode(custLED, OUTPUT);
    pinMode(startInterruptPin, INPUT_PULLUP);

    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }

    while(digitalRead(startInterruptPin) == HIGH){ 
    }
    Serial.println("HI!");

    attachInterrupt(9, toggleInter, FALLING);
}

void loop() {
  while(!roboStat){delay(10); digitalWrite(custLED, LOW);}

  static bool seeBallFlag = 0;
  huskylens.requestBlocks(1); //Only pull the correct ID
  if (huskylens.available()) { //Make sure its available
    huskylens.read();
    seeBallFlag = 1;
  }

  if (seeBallFlag) {
    // Move(50, 50);
    digitalWrite(custLED, HIGH); // Make it high to indicate a sensed a ball
    seeBallFlag = 0; // Set it low, because we indicated it, INTER!
    Serial.println("SAW BALL");
    Stop();
    delay(1000); // Wait for a second
    Move(50, 50); // Move forward
    
    //Wait an eternity for the robot to push the ball
  } else {
    // Add find ball program here

    Move(40, -40); //Find ball by turning

    digitalWrite(custLED, LOW);  // Make it low, because it didn't see it
    Serial.println("Couldn't find ball");
    Stop();
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