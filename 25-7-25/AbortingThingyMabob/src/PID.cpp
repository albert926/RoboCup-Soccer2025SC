#include "PID.h"
#include "Arduino.h"

PID::PID()
    : kp(0), ki(0), kd(0), lastInput(0), integral(0),
      minOutput(-100), maxOutput(100), sampleTime(5), lastTime(0) {}

PID::PID(float kp, float ki, float kd)
    : kp(kp), ki(ki), kd(kd), lastInput(0), integral(0),
      minOutput(-100), maxOutput(100), sampleTime(5), lastTime(0) {}

void PID::setP(float kp){
  this->kp = kp;
}

void PID::setI(float ki){
  this->ki = ki;
}

void PID::setD(float kd){
  this->kd = kd;
}

void PID::setTunings(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void PID::setOutputLimits(float min, float max) {
    if (min >= max) return;
    minOutput = min;
    maxOutput = max;
}

void PID::setSampleTime(unsigned long newSampleTime) {
    sampleTime = newSampleTime;
}

float PID::compute(float setpoint, float input) {
    unsigned long now = millis();
    unsigned long timeChange = now - lastTime;
    
    if (timeChange >= sampleTime) {
        // Calculate error
        float error = setpoint - input;
      
        // Proportional term
        float proportional = kp * error;
        // Integral term
        integral += (ki * error * timeChange / 1000.0);
        if (integral > maxOutput) integral = maxOutput;
        else if (integral < minOutput) integral = minOutput;

        // Derivative term
        float derivative = kd * (input - lastInput) / (timeChange / 1000.0);

        // Compute PID output
        float output = proportional + integral - derivative;

        // Constrain output
        if (output > maxOutput) output = maxOutput;
        else if (output < minOutput) output = minOutput;

        // Remember current values for next iteration
        lastInput = input;
        lastTime = now;

        return output;
    } else{
     
    }
    return 0; // No computation needed yet
}

void PID::reset() {
    lastInput = 0;
    integral = 0;
}
