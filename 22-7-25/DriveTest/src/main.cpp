#include <Arduino.h>

const uint8_t ma1 = 10;
const uint8_t ma2 = 11;
const uint8_t mb1 = 7;
const uint8_t mb2 = 3;

void setup() {
    Serial.begin(9600);
    pinMode(ma1, OUTPUT);
    pinMode(ma2, OUTPUT);
    pinMode(mb1, OUTPUT);
    pinMode(mb2, OUTPUT);
}

void loop() {
    // Example of using Serial to send data
    Serial.println("Starting motor control...");

    // Control motor A
    digitalWrite(ma1, HIGH);
    digitalWrite(ma2, LOW);
    delay(200); // Run motor A for 1 second
    digitalWrite(ma1, LOW);
    digitalWrite(ma2, LOW);

    delay(2000);

    digitalWrite(ma1, LOW);
    digitalWrite(ma2, HIGH);
    delay(200); // Run motor A in reverse for 1 second
    digitalWrite(ma1, LOW);
    digitalWrite(ma2, LOW);

    // Control motor B
    digitalWrite(mb1, HIGH);
    digitalWrite(mb2, LOW);
    delay(1000); // Run motor B for 1 second
    digitalWrite(mb1, LOW);
    digitalWrite(mb2, LOW);

    delay(2000);

    digitalWrite(mb1, LOW);
    digitalWrite(mb2, HIGH);
    delay(200); // Run motor A in reverse for 1 second
    digitalWrite(mb1, LOW);
    digitalWrite(mb2, LOW);

    Serial.println("Motors stopped.");
    delay(2000); // Wait before next iteration
}