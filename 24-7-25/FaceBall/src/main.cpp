/*
This is a code
*/

#include "Arduino.h"
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "PID.h"

PID xPID = PID();
HUSKYLENS huskylens;
SoftwareSerial mySerial(1, 0); // RX, TX
void printResult(HUSKYLENSResult result);

const uint8_t ma1 = 3;
const uint8_t ma2 = 2;
const uint8_t mb1 = 5;
const uint8_t mb2 = 6;

const uint8_t startInterruptPin = 9;

uint8_t x_husky;
uint8_t y_husky;
uint8_t w_husky;
uint8_t h_husky;
uint8_t id_husky;
uint8_t x0_husky;
uint8_t y0_husky;
uint8_t x1_husky;
uint8_t y1_husky;

volatile bool InterStat = 0;

const uint8_t setpoint = 160; // Example: center of 320px image

void Move(int left, int right);
void Stop();
void readHusky();

void toggleInter();

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    // while (!huskylens.begin(mySerial))
    // {
    //     Serial.println(F("Begin failed!"));
    //     Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
    //     Serial.println(F("2.Please recheck the connection."));
    //     delay(100);
    // }
    xPID.setTunings(1.0, 0.0, 0.0); // Kp, Ki, Kd (tune as needed)
    xPID.setOutputLimits(-30, 30); // Example: for motor PWM
    xPID.setSampleTime(20); // 10 ms sample time
    pinMode(ma1, OUTPUT);
    pinMode(ma2, OUTPUT);
    pinMode(mb1, OUTPUT);
    pinMode(mb2, OUTPUT);
    pinMode(startInterruptPin, INPUT_PULLUP);

    while(digitalRead(startInterruptPin) == LOW){
    }
    Serial.println("HI!");

    attachInterrupt(0, toggleInter, FALLING);
}

void loop() {
    // // if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    // // else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    // // else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    // // else
    // // {
    // //     Serial.println(F("###########"));
    // //     while (huskylens.available())
    // //     {
    // //         HUSKYLENSResult result = huskylens.read();
    // //         // printResult(result);
    // //     }    
    // // }

    // // readHusky();

    // HUSKYLENSResult result = huskylens.read();
    // float pidOutput = xPID.compute(setpoint, result.xCenter);
    // Serial.print("PID Output: ");
    // Serial.print(pidOutput);
    // Serial.print("   x: ");
    // Serial.println(x_husky);
    // // Move(-pidOutput, pidOutput);
    // delay(20);
    Serial.println(F("Waiting for interrupt..."));
    while(!InterStat) {
      if (InterStat) {
          InterStat = false;
          Serial.println(F("Interrupt triggered!"));
      }
    }
  }

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
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


void readHusky() {
    HUSKYLENSResult result = huskylens.read();
    if (result.command == COMMAND_RETURN_BLOCK) {
        x_husky = result.xCenter;
        y_husky = result.yCenter;
        w_husky = result.width;
        h_husky = result.height;
        id_husky = result.ID;
    } else if (result.command == COMMAND_RETURN_ARROW) {
        x0_husky = result.xOrigin;
        y0_husky = result.yOrigin;
        x1_husky = result.xTarget;
        y1_husky = result.yTarget;
        id_husky = result.ID;
    }
}

void toggleInter()
{
  InterStat = !InterStat;
}