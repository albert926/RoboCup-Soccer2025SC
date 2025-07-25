/*
This is a code
*/

#include <Arduino.h>

const uint8_t startInterruptPin = 9;

volatile bool InterStat = 0;

void toggleInter();

void setup() {
    Serial.begin(115200);
    pinMode(startInterruptPin, INPUT_PULLUP);

    while(digitalRead(startInterruptPin) == HIGH){
    }
}

void loop() {
    // Serial.println(F("Waiting for interrupt..."));
    // while(!InterStat) {
    //   if (InterStat) {
    //       InterStat = false;
    //       Serial.println(F("Interrupt triggered!"));
    //   }
    // }
    Serial.println("HI dingdong!");
    delay(1000);
}

// void toggleInter()
// {
//   InterStat = !InterStat;
// }