#include "PID.h"
#include "WebSocketHandler.h"

#define MODE_NORMAL 0
#define MODE_TEST 1  // normal movement disengaged


int MAX_PITCH_CONTROL = 5;
int MAX_TURN_CONTROL = 40;
bool REVERSE_TURNING = true;

long speedLimit = 4;

// Target Angle, find this by setting PRINTANGLE true
double pitchOffset = 0;

bool PRINTANGLE = false;

// Print MAC address for RC transmitter. If true, regular program doesn't run
bool PRINTMAC = false;

bool PRINTSPEED = false;

// Drive both motors forward, backward, and stop. Blocks regular program
bool TESTMOTORS = false;

bool powerOn = true;
bool resetPosition = false;

int counter = 0;
double currentPitchSet = 0;

long lastIntervalAction = 0;
long intervalActioninterval = 5000;

long lSpeed = 0;
long rSpeed = 0;
double avgSpeed = 0;

uint8_t mode = MODE_NORMAL;

PID pitchPID = PID();

void setup() {
  // put your setup code here, to run once:
  pinMode(15, OUTPUT);

  Serial.begin(115200);
  InitNVS();
  initializeWiFi();
  InitMotors();
  InitEncoders();
  InitIMU();

  while (PRINTMAC) {
    PrintMAC();
    delay(5000);
  }

  // for (int i = 0; i < 4; i++) {
  //   digitalWrite(15, !digitalRead(15));
  //   delay(500);
  // }

  SetDefaults(1, 0);
}

void loop() {
  UpdateIMU();

  if (mode == MODE_NORMAL) {
    lSpeed = GetSpeed(0);
    rSpeed = GetSpeed(1);
    avgSpeed = (lSpeed + rSpeed) / 2.0;
    if (avgSpeed < 0) {
      avgSpeed = -avgSpeed;
    }
    if (PRINTSPEED) {
      Serial.print("L: ");
      Serial.print(lSpeed);
      Serial.print("\tR: ");
      Serial.print(rSpeed);
      Serial.print("\tAVG: ");
      Serial.println(avgSpeed);
    }

    if (millis() > 3000 && !resetPosition) {
      resetPosition = true;
      ResetPosition();
    }
    if (millis() - lastIntervalAction > intervalActioninterval) {
      PrintMAC();
      lastIntervalAction = millis();
    }
    // put your main code here, to run repeatedly:

    // SetMotor(0, 0);

    int targetPitch = map(GetData(9), 1, 255, MAX_PITCH_CONTROL, -MAX_PITCH_CONTROL);

    if (avgSpeed > speedLimit) {
      targetPitch = 0;
    }

    if (currentPitchSet < targetPitch) {
      currentPitchSet += 0.25;
    } else if (currentPitchSet > targetPitch) {
      currentPitchSet -= 0.25;
    }
    //UpdateSetPoint(pitchOffset - currentPitchSet);
    // Serial.println(currentPitchSet);
    long position = GetMotorPosition(0);
    long tgtPitch = -pitchOffset - currentPitchSet;

    int turnAmount = map(GetData(8), 1, 255, MAX_TURN_CONTROL, -MAX_TURN_CONTROL);

    long v = pitchPID.compute(tgtPitch, GetPitch());
    if (!powerOn) {
      v = 0;
    }

    // Serial.print(v);
    // Serial.print("\t");
    // Serial.println(GetPitch());

    if (receivedPowerToggle) {
      receivedPowerToggle = false;
      powerOn = !powerOn;
    }
    // Serial.println(0+tgtPitch);
    //  Serial.print("\t");
    //  Serial.print(GetPitch());
    //  Serial.print("\t");
    //  Serial.println(v);
    // v = v*1.2;

    if (REVERSE_TURNING) {
      turnAmount = -turnAmount;
    }

    SetMotor(0, v + turnAmount);
    SetMotor(1, v - turnAmount);
  }
  if (PRINTANGLE) {
    PrintIMUData();
  }
  // PrintIMUData();
  delay(5);
}

void SetMode(uint8_t newMode) {
  mode = newMode;
}

void SetMaxPitchControl(float val) {
  MAX_PITCH_CONTROL = val;
}

void SetMaxTurnControl(float val) {
  MAX_TURN_CONTROL = val;
}

void SetReverseTurning(bool val) {
  REVERSE_TURNING = val;
}

void SetPitchOffset(float val){
  pitchOffset = val;
}

PID& GetPID(){
  return pitchPID;
}

void TestMotors(){
  for (int i = 0; i < 256; i++){
    SetMotor(0, i);
    delay(20);
  }
  for (int i = 255; i > -255; i--){
    SetMotor(0, i);
    delay(20);
  }
  for (int i = -255; i <= 0; i++){
    SetMotor(0, i);
    delay(20);
  }
  for (int i = 0; i < 256; i++){
    SetMotor(1, i);
    delay(20);
  }
  for (int i = 255; i > -255; i--){
    SetMotor(1, i);
    delay(20);
  }
  for (int i = -255; i <= 0; i++){
    SetMotor(1, i);
    delay(20);
  }
}